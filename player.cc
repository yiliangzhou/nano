#include "player.h"

player::player (const std::string & r, const graph & g) : my_role (r),
    graph_m_(g){
    
}

std::vector<muncher> player::play (const state &s) {
  std::vector<muncher> munchers;
  // pick up some random un-eaten nodes, as long as we still have
  // nanomunchers left.


  return munchers; 
}
