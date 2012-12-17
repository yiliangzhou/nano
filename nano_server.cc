#include <iostream>
#include <sstream>

#include "referee.h"

#include "socket/socket_server.h"
#include "socket/conn_protocol.h"

/**
 *  Author: liangzhou.yi@nyu.edu
 *  Last updated: 12/10/2012
 */
int main (int argc, const char* argv[]) {
  if (argc < 4) {
    std::cout << "usage: " << argv[0] 
        << " <num_of_munchers> <input_fil> <port_no>" << std::endl;
    return -1;
  }
  
  std::istringstream iss (argv[1]);
  int nanomuncher_num = 0;
  iss >> nanomuncher_num;
  referee r ( nanomuncher_num, argv[2], argv[3]);

  if (!r.is_prepare_done ()) { std::cout << "init error..." << std::endl; return -1; }

  r.game_init ();
  r.game_loop ();
  r.declare_result ();

  return 0;
}
