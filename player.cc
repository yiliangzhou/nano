#include "player.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>

player::player (const std::string & r, const graph & g) : my_role (r),
    graph_m_(g) {  
  srand ( time(NULL) );
}

std::vector<muncher> player::play (const state &s) {
  // Update graph
  update (s);
  // Play  
  std::vector<muncher> munchers = random_play ();

  return munchers; 
}

void player::update (const state &s) {
  graph_m_.update ( s.get_eaten_nodes () );
  if (my_role.compare ("RED") == 0) {
    my_score_ = s.get_red_score ();
    my_nano_count_ = s.get_red_left ();
    my_munchers_ = s.get_red_munchers ();
    
    opponent_score_ = s.get_blue_score ();
    opponent_nano_count_ = s.get_blue_left ();
    opponent_munchers_ = s.get_blue_munchers (); 
  } else {
    my_score_ = s.get_blue_score ();
    my_nano_count_ = s.get_blue_left ();
    my_munchers_ = s.get_blue_munchers ();
    
    opponent_score_ = s.get_red_score ();
    opponent_nano_count_ = s.get_red_left ();
    opponent_munchers_ = s.get_red_munchers ();    
  }
}

std::vector<muncher> player::random_play () {
  std::vector<muncher> munchers;

  // pick up some random un-eaten nodes, as long as we still have
  // nanomunchers left.
  if (my_nano_count_ > 0) {  
    int nano_count = ( rand () % my_nano_count_ ) + 1;
    for (int i = 0; i < nano_count; i++) {
      char raw_program[4] = {'L', 'R', 'U', 'D'};
      //std::vector<char> seed_program (4) = {'L', 'R', 'U', 'D'};
      std::vector<char> seed_program ( &raw_program[0], &raw_program[0] + 4);
      std::random_shuffle (seed_program.begin(), seed_program.end());
      std::string program(seed_program.begin(), seed_program.end());
      int node_id = graph_m_.get_random_uneaten_node ();
      if (node_id >= 0) {
        muncher m ( node_id, program, 0 );
        munchers.push_back ( m );
      } else {
        break;
      }
    }
  }

  return munchers;
}
