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
    // And other information, such as edges.
    node ** board;
};

#endif
