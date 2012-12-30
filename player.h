#ifndef __PLAYER_H_INCLUDED__
#define __PLAYER_H_INCLUDED__

#include <vector>
#include <string>
#include "graph.h"
#include "muncher.h"
#include "state.h"

class player {
  public:
    // constructor of a player.
    // to construct a player, you should tell what's the role of current
    // player in the game.
    // You should also tell what the graph looks like at construction time.
    player (const std::string &,const graph &);

    // given a state of the game before a round, the player
    // should return a vector of nanomunchers to play in current
    // round. 
    std::vector< muncher > play (const state &);

  private:
    // update information using state parsed from PLAY message
    void update (const state &);

    // strategy for playing nanomunchers
    std::vector< muncher > random_play ();

    // either RED or BLUE
    const std::string my_role;

    // information below will be updated each time a play get called. 
    // -----------------------------------------------------------------

    // internal representation of a graph state. 
    graph graph_m_;

    // records how many nanomunchers a player can play.
    int my_nano_count_;
    int opponent_nano_count_; 

    // records the number of nanomunchers has been munched by 
    // current player.
    int my_score_;
    int opponent_score_;

    // records alive nanomunchers at a specific round for current player
    std::vector< muncher > my_munchers_;
    // records alive nanomunchers at a spcific for opponent 
    std::vector< muncher > opponent_munchers_;
    // -----------------------------------------------------------------
};

#endif
