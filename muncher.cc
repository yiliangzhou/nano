#include "muncher.h"

muncher::muncher (int nodeid, std::string program, 
        int initial_count) : 
    nodeid(nodeid), program(program), count(initial_count) {
}

int muncher::get_nodeid () const { return nodeid; }
std::string muncher::get_program () const { return program; }
int muncher::get_count () const { return count; }
