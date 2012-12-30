#include "muncher.h"

// TODO: Replace contructors by builders
muncher::muncher (int nodeid, std::string program,
        int initial_count) :
    nodeid(nodeid),
    program(program),
    count(initial_count) {
}

muncher::muncher (int nodeid, std::string program, 
        int initial_count, int player) : 
    nodeid(nodeid),
    program(program),
    count(initial_count),
    player(player) {
}

int muncher::get_nodeid () const { return nodeid; }
std::string muncher::get_program () const { return program; }
int muncher::get_count () const { return count; }
int muncher::get_player () const { return player; }

int muncher::munch (graph &g) {
  g.munch (nodeid, player);
}

int muncher::move_to_next_node (const graph &g) {
  for (int i = 0; i < 4; i++) {
    int curr_count = (i + count) % 4;
    char dir = program[ curr_count ];
    if ( g.is_adjacent_available (nodeid, dir) ) {
      nodeid = g.get_adjacent_node_id (nodeid, dir);
      count = (curr_count + 1) % 4;
      return 0; 
    }
  }
  return -1;
}

int muncher::get_last_movement_priority () {
  int last_count = ((count - 1) + 4) % 4; 
  char last_dir = program[ last_count ];
  switch (last_dir) {
    case 'U':
    case 'u':
      return 4;
    case 'L':
    case 'l':
      return 3;
    case 'D':
    case 'd':
      return 2;
    case 'R':
    case 'r':
      return 1;
    default:
      break;
  }  
  return -1;
}
