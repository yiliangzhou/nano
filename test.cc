#include <iostream>
#include "protocol.h"

int main (int argc, char * argv[]) {
  // TODO: Build tests for all protocol methods
  std::string msg = "GRAPH: 10 20;0 1 1,2 3 3,4 4 4;0 2,1 4";
  graph g = protocol::parse_graph (msg);
  std::cout << g.str() << std::endl;
}
