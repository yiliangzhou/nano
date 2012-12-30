#ifndef __STATE_INCLUDED_H__
#define __STATE_INCLUDED_H__

#include <vector>
#include <string>

#include "muncher.h"

class state {
  public:
    state (int); 
    state (const state &);
    state (
      int red_score,
      int blue_score,
      int red_left,
      std::vector<muncher> red_nanomunchers,
      int blue_left,
      std::vector<muncher> blue_nanomunchers,
      std::vector<int> eaten_nodes );

    state& operator= (state &);

    int get_red_score () const;
    int get_blue_score () const;
    int get_red_left () const;
    int get_blue_left () const;
    std::vector<muncher> get_red_munchers () const;
    std::vector<muncher> get_blue_munchers () const; 
    std::vector<int> get_eaten_nodes () const;

    std::string str () const;    
  private:    
    int red_score;
    int blue_score;

    int red_left;
    std::vector<muncher> red_nanomunchers;

    int blue_left;
    std::vector<muncher> blue_nanomunchers;

    std::vector<int> eaten_nodes; 
};

#endif
