#include "state.h"
#include <sstream>

// construct an initial state
state::state (int nano_count) {
  this->red_left = nano_count;
  this->blue_left = nano_count;
  this->red_score = 0;
  this->blue_score = 0;
}

// contruct an intermediate state from another state
state::state (
        int red_score,
        int blue_score,
        int red_left,
        std::vector<muncher> red_nanomunchers,
        int blue_left,
        std::vector<muncher> blue_nanomunchers,
        std::vector<int> eaten_nodes ) {
  this->red_score = red_score;
  this->blue_score = blue_score;
  this->red_left = red_left;
  this->red_nanomunchers = red_nanomunchers;
  this->blue_left = blue_left;
  this->blue_nanomunchers = blue_nanomunchers;
  this->eaten_nodes = eaten_nodes;
}

state& state::operator= (state &rhs) {
  if (this == &rhs) { return *this; }

  this->red_score = rhs.red_score;
  this->blue_score = rhs.blue_score;
  this->red_left = rhs.red_left;
  this->blue_left = rhs.blue_left;
  this->red_nanomunchers = rhs.red_nanomunchers;
  this->blue_nanomunchers = rhs.blue_nanomunchers;
  this->eaten_nodes = rhs.eaten_nodes;
}

state::state (const state &rhs) {
  this->red_score = rhs.red_score;
  this->blue_score = rhs.blue_score;
  this->red_left = rhs.red_left;
  this->blue_left = rhs.blue_left;
  this->red_nanomunchers = rhs.red_nanomunchers;
  this->blue_nanomunchers = rhs.blue_nanomunchers;
  this->eaten_nodes = rhs.eaten_nodes;
}

// construct a STATE message
std::string state::str () { 
  std::stringstream ss (std::stringstream::in | std::stringstream::out); 
  std::string delim ("; ");
  ss << "red score: " << red_score << delim;
  ss << "blue score: " << blue_score << delim;
  ss << "red left: " << red_left << delim;
  ss << "blue left: " << blue_left << delim;
  ss << "red nanomunchers: ";
  for (int i = 0; i < red_nanomunchers.size(); i++) {
    ss << red_nanomunchers[i].get_nodeid () << " ";
    ss << red_nanomunchers[i].get_program () << " ";
    ss << red_nanomunchers[i].get_count (); 
    if (red_nanomunchers.size() - 1 == i)
      ss << delim;
    else
      ss << ", ";
  }
  ss << "blue nanomunchers: ";
  for (int i = 0; i < blue_nanomunchers.size(); i++) {
    ss << blue_nanomunchers[i].get_nodeid () << " ";
    ss << blue_nanomunchers[i].get_program () << " ";
    ss << blue_nanomunchers[i].get_count (); 
    if (blue_nanomunchers.size() - 1 == i)
      ss << delim;
    else
      ss << ", ";
  }
  
  ss << "eaten nodes: ";
  for (int i = 0; i < eaten_nodes.size(); i++) {
    ss << eaten_nodes[i];
    if (eaten_nodes.size() - 1 != i)
      ss << ",";
  }

  return ss.str(); 
}

int state::get_red_score () const { return red_score; }
int state::get_blue_score () const { return blue_score; }
int state::get_red_left () const { return red_left; }
int state::get_blue_left () const { return blue_left; }
std::vector<muncher> state::get_red_munchers () const { return red_nanomunchers; }
std::vector<muncher> state::get_blue_munchers () const { return blue_nanomunchers; }
std::vector<int> state::get_eaten_nodes () const { return eaten_nodes; }
