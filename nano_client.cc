#include <iostream>
#include "protocol.h"
#include "player.h"
#include "graph.h"
#include "state.h"
#include "socket/socket_client.h"
#include "socket/connection.h"

int main (int argc, const char *args[]) {
  using namespace conn;
  if (argc < 3) {
    std::cout << "usage: " << args[0]
        << " <team_name> <port>" << std::endl;
    return 0;
  }
  connection *ptr_conn = new socket_client ("localhost", args[2], "\n");  
  // send team name to server
  std::string team_name (args[1]);
  std::string join_msg = protocol::generate_join_msg (team_name);
  ptr_conn->write (join_msg);
  // receive role from server 
  std::string role_msg;
  ptr_conn->read ( role_msg );
  std::string role = protocol::parse_role (role_msg);
  std::cout << "[Server] " << role_msg << std::endl;

  // receive graph from server
  std::string graph_msg;
  ptr_conn->read ( graph_msg );
  std::cout << "[Server] " << graph_msg << std::endl; 
  graph g = protocol::parse_graph (graph_msg);
  std::cout << "[Graph] " << std::endl << g.str();
  
  player p ( role, g );  
  std::string msg;
  do {
    ptr_conn->read ( msg );
    if ( protocol::is_gameover_msg( msg ) ) { break; } 
    std::cout << "[Server] " << msg << std::endl;
    state s( protocol::parse_play( msg ) );
    msg.assign( protocol::generate_add_msg ( p.play(s) ));
    std::cout << "[" << team_name << "] " << msg << std::endl;
    ptr_conn->write( msg );

    // read ACK message, here simply ignore it.
    ptr_conn->read ( msg );
    std::cout << "[Server] " << msg << std::endl;
  } while (true); 

  // print out result received from server
  std::cout << "[Server] " <<  msg << std::endl;
  return 0;
}
