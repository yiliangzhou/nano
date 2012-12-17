#include "referee.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
        protocol::parse_add (msg_from_red);
    std::vector<muncher> new_munchers_blue = 
        protocol::parse_add (msg_from_blue);

    // send ACK message to player
    ptr_server->write (fd_for_red, protocol::generate_ack_msg ("OK"));
    ptr_server->write (fd_for_blue, protocol::generate_ack_msg ("OK"));
    
    // TODO: let all nanomunchers, along with new ones, to make one
    // move.
    play_one_round (new_munchers_red, new_munchers_blue);
     
    // TODO: add stopping criteria below 
  } while ( i++ < 4 );
}

// TODO: implement below method, play all nanomunchers for one round.
// resolve any conflict.
void referee::play_one_round(std::vector<muncher> &reds,
        std::vector<muncher> &blues) {
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
  // TODO: constuct a state class to reflect current state
  // Below return a sample state 
  std::vector<muncher> ms;
  muncher m (10, "LRDU", 1);
  ms.push_back(m);
  ms.push_back(m);
  std::vector<int> en(5, 1);
  state s (5, 4, 10, ms, 5, ms, en);
  return s;
}
