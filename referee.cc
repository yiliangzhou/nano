#include "referee.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

referee::referee (const int nanomuncher_num, const char *input,
  const char * port) : 
    file_name (input), 
    port (port),
    red_score(0),
    blue_score(0),
    red_left(nanomuncher_num), 
    blue_left(nanomuncher_num),
    graph_m_ (20,10), 
    prepare_done (false) { 
  // read input and generate graph
  prepare ();
}

referee::~referee () {
  // destruct the server
  if (ptr_server != NULL) {
    delete ptr_server;
  }
}

void referee::prepare () {
  // read input from local input file
  std::ifstream inputfile (file_name.c_str());
  std::map<int, std::pair<int, int> > id2coord; 
  std::vector< std::pair<int, int> > edges;
  if ( !inputfile.is_open() ) { prepare_done = false; return; }

  std::string line;
  int phase = 0;
  const int parsing_nodes = phase + 1;
  const int parsing_edges = phase + 2;
  while ( inputfile.good() ) {
    std::getline (inputfile, line);
    if ( line.empty() ) { continue; } 
    if (line.find ("nodeid,xloc,yloc") != std::string::npos) { 
        phase++; 
        continue; 
    } 
    if (line.find ("nodeid1,nodeid2") != std::string::npos) { 
        phase++; 
        continue; 
    }

    if (parsing_nodes == phase) {
      std::istringstream iss (line);
      char not_used;
      int nodeid, x, y;
      iss >> nodeid >> not_used >> x >> not_used >> y;
      id2coord.insert (std::pair< int, std::pair<int, int> > 
              (nodeid, std::pair<int, int> (x, y)));
      continue;
    }

    if (parsing_edges == phase) {
      std::istringstream iss (line);
      char not_used;
      int nodeid1, nodeid2;
      iss >> nodeid1 >> not_used >> nodeid2;
      edges.push_back (std::pair<int, int> (nodeid1, nodeid2) );
      continue;
    }
  }

  inputfile.close();
  graph_m_.set_nodes(id2coord).set_edges(edges); 
  graph_msg = protocol::generate_graph_msg (id2coord, edges);
  
  // new delete pair 
  ptr_server = new conn::socket_server(port.c_str(), 10, "\n"); 

  if ( !ptr_server->is_good () ) { prepare_done = false; return; }
  // set file descriptors
  fd_for_red = ptr_server->accept_client();
  fd_for_blue = ptr_server->accept_client(); 

  if (fd_for_red < 0 || fd_for_blue < 0) { prepare_done = false; return; } 
 
  srand ( time(NULL) );

  prepare_done = true;
  return;
}

bool referee::is_prepare_done () {
  return prepare_done;  
}

void referee::game_init () {
  // read team names from both player
  std::string red_join_msg;
  std::string blue_join_msg;
  ptr_server->read (fd_for_red, red_join_msg);
  ptr_server->read (fd_for_blue, blue_join_msg); 
  red_team_name = protocol::parse_join (red_join_msg);
  blue_team_name = protocol::parse_join (blue_join_msg);

  std::string role_msg_red = protocol::generate_role_msg ("RED");
  std::string role_msg_blue = protocol::generate_role_msg ("BLUE");

  ptr_server->write (fd_for_red, role_msg_red);
  ptr_server->write (fd_for_blue, role_msg_blue);

  // send GRAPH message 
  ptr_server->write(fd_for_red, graph_msg);
  ptr_server->write(fd_for_blue, graph_msg);
}

void referee::game_loop () {
  int i = 0;
  do {
    std::cout << "[Refere] " << std::endl << graph_m_.str();
    state s (get_current_state ());
    std::string play_msg = protocol::generate_play_msg ( s );
    ptr_server->write (fd_for_red, play_msg);
    ptr_server->write (fd_for_blue, play_msg);

    std::string msg_from_red;
    std::string msg_from_blue;
    // receive ADD message from players
    ptr_server->read (fd_for_red, msg_from_red);
    ptr_server->read (fd_for_blue, msg_from_blue);   
    std::cout << "[RED  player] " << msg_from_red << std::endl;
    std::cout << "[BLUE player] " << msg_from_blue << std::endl;

    // play all nanomunchers ( old and new )
    std::vector<muncher> new_munchers_red = 
        protocol::parse_add (1, msg_from_red);
    std::vector<muncher> new_munchers_blue = 
        protocol::parse_add (2, msg_from_blue);
    
    // Send ACK message to player
    ptr_server->write (fd_for_red, protocol::generate_ack_msg ("OK"));
    ptr_server->write (fd_for_blue, protocol::generate_ack_msg ("OK"));
    
    // Let all nanomunchers, along with new ones, to make one
    // move.
    play_one_round (new_munchers_red, new_munchers_blue);
     
    // TODO: add stopping criteria below 
  } while ( i++ < 4 );
}

// Play all nanomunchers for one round.
// Resolve any conflict.
void referee::play_one_round(std::vector<muncher> &reds,
        std::vector<muncher> &blues) {
  // Update red_left and blue_left
  int new_red_size = reds.size();
  int new_blue_size = blues.size();
   
  // Truncate if give more nanomunchers allowed. 
  if ( new_red_size > red_left ) {
    new_red_size = red_left;  
    reds.resize(new_red_size);
  }
  if ( new_blue_size > blue_left) {
    new_blue_size = blue_left;  
    blues.resize(new_blue_size);
  }

  red_left -= new_red_size;
  blue_left -= new_blue_size;

  // Phase one, apply new nanomunchers before move
  deploy_new_nanomunchers (reds, blues);
  // Phase two, run all nanomunchers. 
  run_nanomunchers ();
}

void referee::deploy_new_nanomunchers (std::vector<muncher> &reds, 
        std::vector<muncher> &blues) {
  std::map<int, std::vector<muncher> > id2old_munchers;
  std::map<int, std::vector<muncher> > id2new_munchers;  
  
  for (int i = 0; i < red_munchers.size(); i++) {
    id2old_munchers[ red_munchers[i].get_nodeid() ].push_back(red_munchers[i]);
  }
  for (int i = 0; i < blue_munchers.size(); i++) {
    id2old_munchers[ blue_munchers[i].get_nodeid() ].push_back(blue_munchers[i]);
  }
  for (int i = 0; i < reds.size(); i++) {
    id2new_munchers[ reds[i].get_nodeid() ].push_back(reds[i]);
  }
  for (int i = 0; i < blues.size(); i++) {
    id2new_munchers[ blues[i].get_nodeid() ].push_back(blues[i]);
  }

  // red_munchers.clear();
  // blue_munchers.clear();

  // Resolve conflict when adding new nanomunchers
  std::map<int, std::vector<muncher> >::iterator it;
  for (it = id2new_munchers.begin(); it != id2new_munchers.end(); it++) {
    // Old nanomuncher kill new nanomunchers.
    if (id2old_munchers.find( (*it).first ) != id2old_munchers.end()) continue;

    // Randomly pick one nanomuncher.
    int index = 0;
    if ( (*it).second.size() > 1) {
      index = rand () % (*it).second.size(); 
    }
    if ( (*it).second[index].get_player() == 1 ) {
      // a red nanomuncher
      red_munchers.push_back( (*it).second[index] );
    } else {
      // a blue nanomuncher
      blue_munchers.push_back ( (*it).second[index] );
    }
  } 
}

void referee::run_nanomunchers () {
  
  // For each nanomuncher, munch the node at current position
  for (int i = 0; i < red_munchers.size(); i++) {
    red_munchers[i].munch ( graph_m_ );
    red_score++;
    // Temporarily move to next moveable node.
    // Otherwise, remove it.
    if ( -1 == red_munchers[i].move_to_next_node ( graph_m_ ) ) 
      red_munchers.erase (red_munchers.begin() + i);
  }
  
  for (int i = 0; i < blue_munchers.size(); i++) {
    blue_munchers[i].munch ( graph_m_ );
    blue_score++;
    if ( -1 == blue_munchers[i].move_to_next_node ( graph_m_ ) )
      blue_munchers.erase (blue_munchers.begin() + i);
  }
  
  // Resolve conflict when more than one nanomunchers move into
  // a single node. 
  std::map< int, std::vector<muncher> > id2munchers;
  for (int i = 0; i < red_munchers.size(); i++) {
    id2munchers[ red_munchers[i].get_nodeid() ].push_back (red_munchers[i]);
  }
  for (int i = 0; i < blue_munchers.size(); i++) {
    id2munchers[ blue_munchers[i].get_nodeid() ].push_back (blue_munchers[i]);
  }
  
  red_munchers.clear();
  blue_munchers.clear();

  std::map<int, std::vector<muncher> >::iterator it;
  for (it = id2munchers.begin(); it != id2munchers.end(); it++) {
    int index = 0;
    if ( (*it).second.size() > 1 ) {
      int max_priority = -1;
      // up > left > down > right
      for (int i = 0; i < (*it).second.size(); i++) {
        int current_priority = (*it).second[i].get_last_movement_priority ();
        if ( current_priority > max_priority ) {
          max_priority = current_priority;
          index = i;
        } 
      } 
    }
    if ( (*it).second[index].get_player() == 1 ) {
      // a red nanomuncher
      red_munchers.push_back( (*it).second[index] );
    } else {
      // a blue nanomuncher
      blue_munchers.push_back ( (*it).second[index] );
    }
  } 
}

std::string referee::declare_result () {
  // send result to two clients
  std::string gameover_msg = protocol::generate_gameover_msg (red_score, blue_score);
  ptr_server->write (fd_for_red, gameover_msg);
  ptr_server->write (fd_for_blue, gameover_msg);

  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  std::cout << "[Referee] " << gameover_msg << std::endl;
  return gameover_msg;
}

state referee::get_current_state () const {
  // constuct a state class to reflect current state
  std::vector<int> eaten_nodes = graph_m_.get_eaten_nodes();
  state s (red_score, blue_score, red_left, red_munchers, blue_left, blue_munchers,
          eaten_nodes);
  return s;
}
