#include <iostream>
#include <sstream>
#include "graph.h"

graph::graph (int x_size, int y_size) {
  this->x_size = x_size;
  this->y_size = y_size;
  this->board = new node*[y_size];
  for (int i = 0; i < y_size; i++) {
    this->board[i] = new node[x_size];
  }
}

graph& graph::set_nodes (std::map<int, std::pair<int, int> > &id2coord) {
  this->id2coord = id2coord;
  std::map< int, std::pair<int, int> >::iterator iter;
  for (iter = id2coord.begin(); iter != id2coord.end(); iter++) {
    const int c_x = (*iter).second.first;
    const int c_y = (*iter).second.second;
    board[c_y][c_x].set_valid();
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
    board[y1][x1].connect_to (x1-x2, y1-y2);
    board[y2][y1].connect_to (x2-x1, y2-y1);
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
}

// TODO: Build a commanline output to also relect the edges
std::string graph::str () {
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  for (int i = 0; i < y_size; i++) {
      for (int j = 0; j < x_size; j++) {
          ss << (board[i][j].is_valid() ? (board[i][j].is_munched() ? "X" : "O") : " ");
      }
      ss << std::endl;
  }
  return ss.str();
}
