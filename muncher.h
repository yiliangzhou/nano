#ifndef __MUNCHER_INCLUDED_H__
#define __MUNCHER_INCLUDED_H__

#include <string>
#include <vector>

#include "graph.h"

class muncher {
  public:
    muncher (int, std::string, int);
    muncher (int, std::string, int, int);
    muncher () {}

    // get current node id where the current nanomuncher locates.
    int get_nodeid () const;
    int set_nodeid ();

    std::string get_program () const;
    // get the program count of current nanomuncher
    int get_count () const;
    void set_player (int);
    int get_player () const;

    int munch (graph &);
    int move_to_next_node (const graph &); 
    // prioirty for up is 4, for left is 3, for down is 2,
    // for right is 1.
    int get_last_movement_priority ();
  private:
    int nodeid;
    // std::vector<char> program;
    std::string program;
    int count;
    // 1: red player, 2: blue player
    int player; 
};

#endif
