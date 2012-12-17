#ifndef __PROTOCOL_H_INCLUDED__
#define __PROTOCOL_H_INCLUDED__

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "node.h"
#include "graph.h"
#include "state.h"

// This class provides the interface for client and server to communicate
// with each other.
// author <liangzhou.yi@nyu.edu>
class protocol {
  public:
    // given a string representation of GRAPH message, return a graph object
    // for future operation.
    static bool is_gameover_msg (const std::string &);
    static bool is_play_msg (const std::string &);

    static std::string parse_join (const std::string &);
    static std::string generate_join_msg (const std::string &);

    static std::string parse_role (const std::string &);
    static std::string generate_role_msg (const std::string &);

    static graph parse_graph (const std::string &);
    static std::string generate_graph_msg (std::map<int, std::pair<int, int> > &,
            std::vector< std::pair<int, int> > &);

    static state parse_play (const std::string &); 
    static std::string generate_play_msg (const state &);

    static std::vector<muncher> parse_add (const std::string &);
    static std::string generate_add_msg (const std::vector<muncher> &);

    static std::string parse_ack (const std::string &);
    static std::string generate_ack_msg (const std::string &);

    static std::string generate_gameover_msg (int, int);
};


#endif
