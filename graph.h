#ifndef __GRAPH_INCLUDED_H__
#define __GRAPH_INCLUDED_H__

#include "node.h"

#include <map>
#include <string>
#include <vector>
#include <utility>

class graph {
  public:
    // To construct a graph object, you should supply all 
    // necessary informations.
    graph (int, int);
    graph& set_nodes (std::map<int, std::pair<int, int> > &);
    graph& set_edges (std::vector< std::pair<int, int> > &);

    // Constructor and Assignment overloading
    graph (const graph & rhs);
    graph& operator=(const graph & rhs);
        
    // Destructor
    ~graph ();
    
    // Update the graph by eaten nodes. 
    void update (const std::vector<int> &);
    std::string str (); 

    void munch (int node_id, int player);
    
    // Test if a nanomuncher could move in the direction of dir 
    // from node_id
    bool is_adjacent_available (int node_id, char dir) const;
    // Get the the adjecent node's id
    int get_adjacent_node_id (int node_id, char dir) const;    

    std::vector< int > get_eaten_nodes () const;

    // Player can utilize this method to generate
    // random strategy
    int get_random_uneaten_node () const;

  private:
    // Store the dimension of the board, within which
    // our graph locates.
    int x_size;
    int y_size; 

    // The map below maps from the id of a node to its coordinate
    std::map< int, std::pair<int, int> > id2coord;
    std::vector< std::pair<int, int> > edges;     
    // The two dimensinal array is used to store the graph 
    // information. Each element is either a node or not a node.
    node ** board;
};

#endif
