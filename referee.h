#ifndef __REFEREE_H_INCLUDED__
#define __REFEREE_H_INCLUDED__

#include <vector>
#include <string>
#include "graph.h"
#include "muncher.h"
#include "state.h"

#include "socket/socket_server.h"
#include "socket/conn_protocol.h"

#include "protocol.h"

/**
 * referee mointers the game, dealing with two players.
 */
class referee {
  public:
    referee (const int, const char *, const char *);
    ~referee (); 
    bool is_prepare_done ();
    void game_init (); 
    void game_loop ();
    std::string declare_result ();

  private:
    void prepare ();
    void play_one_round (std::vector<muncher> &, std::vector<muncher> &);
    state get_current_state () const; 

    bool prepare_done;
    const std::string file_name;
    conn::socket_server * ptr_server;
    std::string port;

    std::string red_team_name;
    std::string blue_team_name;

    int fd_for_red;
    int fd_for_blue;

    graph graph_m_;
    std::string graph_msg;
    // records how many nanomunchers each player has left.
    int red_left;
    int blue_left;

    // records the number of nanomunchers has been munched by 
    // current player.
    int red_score;
    int blue_score;

    // records alive nanomunchers at a specific round for current player
    std::vector< muncher > red_munchers;
    // records alive nanomunchers at a spcific for opponent 
    std::vector< muncher > blue_munchers;
    // -----------------------------------------------------------------
};

#endif