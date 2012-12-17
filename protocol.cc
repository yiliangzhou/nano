#include "protocol.h"
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>
        

bool protocol::is_gameover_msg (const std::string &msg) {
  return msg.find ("GAMEOVER") != std::string::npos;
}

bool protocol::is_play_msg (const std::string &msg) {
  return msg.find ("PLAY") != std::string::npos;
}

// A GRAPH message has the following format:
// GRAPH: <dimension of board>; <list of node_info separated by 
// comma> ; <list of edge_info separated by comma>
// dimension of board is x_d y_d 
graph protocol::parse_graph (const std::string &msg) {
  int x_d;
  int y_d;
  std::map< int, std::pair<int, int> > id2coord;
  std::vector< std::pair<int, int> > edges;

  std::string msg_ = msg.substr (msg.find (":") + 1);
  std::vector<std::string> strs;
  boost::split(strs, msg_, boost::is_any_of(";"));
  
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  // strs[0] corresponds to dimension
  ss << strs[0];
  ss >> x_d >> y_d;
  // strs[1] corresponds to node_infos
  std::vector<std::string> node_strs;
  boost::split(node_strs, strs[1], boost::is_any_of(","));
  for (int i = 0; i < node_strs.size(); i++) {
    // if I reuse previous ss, it will generate wrong result for me.
    std::stringstream sss (std::stringstream::in | std::stringstream::out);
    sss << node_strs[i]; 
    int node_id, x, y;
    sss >> node_id >> x >> y;
    id2coord.insert (std::pair< int, std::pair<int, int> > (node_id, std::pair<int, int> (x, y)) );
  }
  // strs[2] corresponds to edges
  std::vector<std::string> edge_strs;
  boost::split (edge_strs, strs[2], boost::is_any_of(","));
  for (int i = 0; i< edge_strs.size(); i++) {
    std::stringstream sss (std::stringstream::in | std::stringstream::out);
    sss.str(edge_strs[i]);
    std::pair<int, int> edge;
    sss >> edge.first >> edge.second;
    edges.push_back (edge);
  } 
  
  graph ret (x_d, y_d);
  ret.set_nodes (id2coord);
  ret.set_edges (edges);
  return ret;
}

std::string protocol::generate_graph_msg ( std::map <int, std::pair<int, int> > &nodes,
        std::vector< std::pair<int, int> > &edges ) {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  ss << "20 10;";
  std::map<int, std::pair<int,int> >::iterator iter;
  for (iter = nodes.begin(); iter != nodes.end(); ) {
    ss << iter->first << " " << iter->second.first << " " << iter->second.second;
    iter++;
    if ( nodes.end() != iter ) { ss << ","; }
  }
  ss << ";";

  for (int i = 0; i < edges.size(); i++) {
    ss << edges[i].first << " " << edges[i].second;
    if ( (edges.size() - 1) != i ) { ss << ","; }
  }
  
  return ss.str();
}


// PLAY: <red_score>; <blue_score>; <red_left>; <red_nanomunchers>;
// <blue_left>; <blue_nanomunchers>; <eaten_nodes>
state protocol::parse_play (const std::string &msg) {
  int red_score = 0;
  int blue_score = 0;
  int red_left = 0;
  int blue_left = 0;
  std::vector<muncher> red_munchers;
  std::vector<muncher> blue_munchers;
  std::vector<int> eaten_nodes;
  
  // preprocess
  std::string play_msg = msg.substr(msg.find(":") + 1);
  boost::algorithm::trim (play_msg); 
  std::vector<std::string> strs;
  boost::split(strs, play_msg, boost::is_any_of(";"));
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  // strs[0] stores red score
  ss << strs[0];
  ss >> red_score;
  // strs[1] stores blue score
  ss << strs[1];
  ss >> blue_score;
  // strs[2] stores red left
  ss << strs[2];
  ss >> red_left;
  // strs[3] stores alive red nanomunchers if any
  std::vector<std::string> red_muncher_strs;
  boost::split(red_muncher_strs, strs[3], boost::is_any_of(",")); 
  for (int i = 0; i < red_muncher_strs.size(); i++) {
      if (red_muncher_strs[i].empty() ) continue;
      std::istringstream iss (red_muncher_strs[i]);
      int node_id, count;
      std::string program;
      iss >> node_id >> program >> count;
      muncher m (node_id, program, count);
      red_munchers.push_back (m);
  }

  // strs[4] stores blue left
  ss << strs[4];
  ss >> blue_left;
  // strs[5] stores allive blue nanomunchers if any
  std::vector<std::string> blue_muncher_strs;
  boost::split(blue_muncher_strs, strs[5], boost::is_any_of(","));  
  for (int i = 0; i< blue_muncher_strs.size(); i++) {
      if (blue_muncher_strs[i].empty() ) continue;
      std::istringstream iss (blue_muncher_strs[i]);
      int node_id, count;
      std::string program;
      iss >> node_id >> program >> count;
      muncher m (node_id, program, count);
      blue_munchers.push_back (m);
  }

  // strs[6] stores eaten nodes
  std::vector<std::string> eaten_node_strs;
  boost::split(eaten_node_strs, strs[6], boost::is_any_of(","));
  for (int i = 0; i < eaten_node_strs.size(); i++) {
    if (eaten_node_strs[i].empty()) continue;
    std::istringstream iss (eaten_node_strs[i]);
    int node_id = -1;
    iss >> node_id;
    eaten_nodes.push_back (node_id);
  } 

  state new_state ( red_score, blue_score, red_left, red_munchers,
        blue_left, blue_munchers, eaten_nodes );  
  return new_state;
}

std::string protocol::generate_play_msg (const state & s) {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  ss << "PLAY: " << s.get_red_score() << ";" << s.get_blue_score() << ";";
  ss << s.get_red_left() << ";";
  std::vector<muncher> red_munchers = s.get_red_munchers();
  for (int i = 0; i < red_munchers.size(); i++) {
    ss << red_munchers[i].get_nodeid() << " "
        << red_munchers[i].get_program() << " "
        << red_munchers[i].get_count();
    if ( (red_munchers.size() - 1) != i) { ss << ","; }
  }
  ss << ";";
  
  ss << s.get_blue_left() << ";";
  std::vector<muncher> blue_munchers = s.get_blue_munchers();
  for (int i = 0; i < blue_munchers.size(); i++) {
    ss << blue_munchers[i].get_nodeid() << " "
        << blue_munchers[i].get_program() << " "
        << blue_munchers[i].get_count();
    if ( (blue_munchers.size() - 1) != i) { ss << ","; }
  }
  ss << ";";
  
  std::vector<int> eaten_nodes = s.get_eaten_nodes();
  for (int i = 0; i < eaten_nodes.size(); i++) {
    ss << eaten_nodes[i];
    if ( (eaten_nodes.size() - 1) != i ) { ss << ","; }
  }

  return ss.str();
}

std::vector<muncher> protocol::parse_add (const std::string &msg) {
  std::string add_msg = msg.substr(msg.find(":") + 1);
  std::vector<muncher> munchers;
  std::vector<std::string> muncher_strs;
  boost::split(muncher_strs, add_msg, boost::is_any_of(",")); 
  for (int i = 0; i < muncher_strs.size(); i++) {
    std::istringstream iss (muncher_strs[i]);
    int nodeid, count;
    std::string program;
    iss >> nodeid >> program >> count;
    muncher alive_muncher (nodeid, program, count);
    munchers.push_back (alive_muncher);
  }  
  return munchers;
}

std::string protocol::generate_add_msg(const std::vector<muncher> & munchers) {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  ss << "ADD: ";
  for (int i = 0; i < munchers.size(); i++) {
    ss << munchers[i].get_nodeid() << " "
        << munchers[i].get_program() << " "
        << munchers[i].get_count();
    if (munchers.size() - 1 != i) { ss << ","; } 
  }
  return ss.str();
}

std::string protocol::generate_gameover_msg (int red_score, int blue_score) {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  std::string result;
  if (red_score > blue_score) { result.assign ("RED"); }
  if (red_score < blue_score) { result.assign ("BLUE"); }
  if (red_score == blue_score) { result.assign ("TIE"); }
  ss << "GAMEOVER: " << result << " " << red_score << " " << blue_score;
  return ss.str(); 
}

std::string protocol::parse_role (const std::string &msg) {
  std::string role_msg = msg.substr (msg.find(":") + 1);
  return role_msg;
}

std::string protocol::generate_role_msg (const std::string &role_str) {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  ss << "ROLE: " << role_str;
  return ss.str();
}

std::string protocol::parse_join (const std::string &msg) {
  std::string join_msg = msg.substr (msg.find(":") + 1);
  return join_msg; 
}

std::string protocol::generate_join_msg (const std::string & teamname) {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  ss << "JOIN: " << teamname;
  return ss.str();

}


std::string protocol::parse_ack (const std::string &msg) {
  std::string ack_msg = msg.substr (msg.find(":") + 1);
  return ack_msg; 
}

std::string protocol::generate_ack_msg (const std::string &msg) {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  ss << "ACK: " << msg;
  return ss.str();

}


