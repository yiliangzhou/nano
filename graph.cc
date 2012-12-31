#include "graph.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

graph::graph (int x_size, int y_size) {
  this->x_size = x_size;
  this->y_size = y_size;
  this->board = new node*[y_size];
  for (int i = 0; i < y_size; i++) {
    this->board[i] = new node[x_size];
  }

  srand ( time(NULL) );
}

graph& graph::set_nodes (std::map<int, std::pair<int, int> > &id2coord) {
  this->id2coord = id2coord;
  std::map< int, std::pair<int, int> >::iterator iter;
  for (iter = id2coord.begin(); iter != id2coord.end(); iter++) {
    const int c_x = (*iter).second.first;
    const int c_y = (*iter).second.second;
    board[c_y][c_x].set_valid( (*iter).first );
  }
  return *this;
}

graph& graph::set_edges (std::vector< std::pair<int, int> > &edges ) {
  this->edges = edges;
  for (int i = 0; i < edges.size(); i++) {
    const int node1 = edges[i].first;
    const int node2 = edges[i].second;
    int x1 = id2coord[node1].first;
    int x2 = id2coord[node2].first;
    int y1 = id2coord[node1].second;
    int y2 = id2coord[node2].second;
    // add edge to both node1 and node2
    board[y1][x1].connect_to (x1-x2, y1-y2, node2);
    board[y2][x2].connect_to (x2-x1, y2-y1, node1);
  }

  return *this;
}


// Constructors
graph::graph (const graph & rhs) {
  this->x_size = rhs.x_size;
  this->y_size = rhs.y_size;
  this->id2coord = rhs.id2coord;
  this->edges = rhs.edges; 
  this->board = new node*[y_size];
  for (int i = 0; i < y_size; i++) {
    this->board[i] = new node[x_size];
  }
  for (int i = 0; i < y_size; i++) {
    for (int j = 0; j < x_size; j++) {
      this->board[i][j] = rhs.board[i][j];
    }
  }
}

graph& graph::operator=(const graph & rhs) {
  if (this == &rhs) { return *this; }

  this->x_size = rhs.x_size;
  this->y_size = rhs.y_size;
  this->id2coord = rhs.id2coord;
  this->edges = rhs.edges; 
  this->board = new node*[y_size];
  for (int i = 0; i < y_size; i++) {
    this->board[i] = new node[x_size];
  }
  for (int i = 0; i < y_size; i++) {
    for (int j = 0; j < x_size; j++) {
      this->board[i][j] = rhs.board[i][j];
    }
  }
}
 
graph::~graph () {
  for (int i = 0; i < y_size; i++) {
    delete [] board[i];
  }
  delete [] board;
}

// Construct a command line string to represent the graph
std::string graph::str () {

  std::vector< std::vector<char> > board_str ( y_size * 2, 
          std::vector<char>(x_size * 2, ' ') );
  int red_count = 0;
  int blue_count = 0; 
  for (int y = 0; y < y_size; y++) {
    for (int x = 0; x < x_size; x++) {
      int str_x = 2*x + 1;
      int str_y = 2*y + 1;

      // board_str[str_y][str_x] = ( board[y][x].is_valid() ? (
      //        board[y][x].is_munched() ? 'X' : 'O' ) : ' ' );

      if ( board[y][x].is_valid() ) {
        if ( board[y][x].is_munched() && board[y][x].munched_by() == 1) {
          board_str[ str_y ][ str_x ] = 'R'; 
          red_count++;
        } else if ( board[y][x].is_munched() && board[y][x].munched_by() == 2) {
          board_str[ str_y ][ str_x ] = 'B';
          blue_count++;
        } else {
          board_str[ str_y ][ str_x ] = 'O';
        }
      } else {
        board_str[ str_y ][ str_x ] = ' ';
      }    

      // fill edges
      if ( board[y][x].is_valid() ) {
        if ( board[y][x].has_left_edge() ) { board_str[str_y][str_x - 1] = '-'; } 
        if ( board[y][x].has_right_edge() ) { board_str[str_y][str_x + 1] = '-'; } 
        if ( board[y][x].has_up_edge() ) { board_str[str_y - 1][str_x] = '|'; } 
        if ( board[y][x].has_down_edge() ) { board_str[str_y + 1][str_x] = '|'; } 
      }
    }
  } 
   
  // Construct string based on intermediate data structure
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  // No up edges exist for the first row of nodes, so skip the first
  // intermediate row.
  // The same for the first coloum.
  for (int y = 1; y < y_size * 2; y++) {
      for (int x = 1; x < x_size * 2; x++) {
          ss << board_str[y][x];
      }
      ss << std::endl;
  }

  ss << "R: Node munched by red player." << std::endl; 
  ss << "B: Node munched by blue player." << std::endl;
  ss << "O: Unmunched node." << std::endl;
  return ss.str();
}

void graph::munch (int node_id, int player) {
  std::pair<int, int> coord = id2coord[ node_id ];  
  board[coord.second][coord.first].munch (player);
}

bool graph::is_adjacent_available (int node_id, char dir) const{
  // std::pair<int, int> coord = id2coord[ node_id ];
  std::pair<int, int> coord = id2coord.find ( node_id )->second;
  switch (dir) {
    case 'L':
    case 'l':
      if ( board[coord.second][coord.first].has_left_edge() ) {
        int x = coord.first - 1;
        int y = coord.second;
        if ( !board[y][x].is_munched() ) return true; 
      }
      break;
    case 'R':
    case 'r':
      if ( board[coord.second][coord.first].has_right_edge() ) {
        int x = coord.first + 1;
        int y = coord.second;
        if ( !board[y][x].is_munched() ) return true; 
      }
      break;
    case 'U':
    case 'u':
      if ( board[coord.second][coord.first].has_up_edge() ) {
        int x = coord.first;
        int y = coord.second - 1;
        if ( !board[y][x].is_munched() ) return true; 
      }
      break;
    case 'D':
    case 'd':
      if ( board[coord.second][coord.first].has_down_edge() ) {
        int x = coord.first;
        int y = coord.second + 1;
        if ( !board[y][x].is_munched() ) return true; 
      }
      break;    
      default:
      break;
  }
  return false;
}

int graph::get_adjacent_node_id (int node_id, char dir) const{
    // std::pair<int, int> coord = id2coord[node_id];
    std::pair<int, int> coord = id2coord.find( node_id )->second;
    switch (dir) {
      case 'L':
      case 'l':
        return board[coord.second][coord.first].get_l_id();
      case 'R':
      case 'r':
        return board[coord.second][coord.first].get_r_id();
      case 'U':
      case 'u':
        return board[coord.second][coord.first].get_u_id();
      case 'D':
      case 'd':
        return board[coord.second][coord.first].get_d_id();
      default:
        break;
    }
    return -1;
}

void graph::update (const std::vector<int> &eaten_nodes) {
  for (int i = 0; i < eaten_nodes.size(); i++) {
    std::pair<int, int> coord = (id2coord.find(eaten_nodes[i]))->second ;
    board[coord.second][coord.first].munch ();
  } 
}

// return a randomly picked uneaten node if available, otherwise
// return -1.
int graph::get_random_uneaten_node () const {
  std::vector<int> uneaten_nodes;
  for (int x = 0; x < x_size; x++) {
    for (int y = 0; y < y_size; y++) {
      if ( board[y][x].is_valid() && !board[y][x].is_munched() )
        uneaten_nodes.push_back ( board[y][x].get_node_id() );
    }
  }  

  if ( uneaten_nodes.size() == 0 ) return -1;

  int random_idx = rand() % uneaten_nodes.size(); 
  return uneaten_nodes[random_idx];
}

std::vector<int> graph::get_eaten_nodes () const {
  std::vector<int> eaten_nodes;
  for (int x = 0; x < x_size; x++) {
    for (int y = 0; y < y_size; y++) {
      if ( board[y][x].is_valid() && board[y][x].is_munched() )
        eaten_nodes.push_back ( board[y][x].get_node_id() );
    }
  } 
  return eaten_nodes;
}
