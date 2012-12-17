#ifndef __MUNCHER_INCLUDED_H__
#define __MUNCHER_INCLUDED_H__

#include <string>
#include <vector>

class muncher {
  public:
    muncher (int, std::string, int);
    int get_nodeid () const;
    std::string get_program () const;
    int get_count () const;
  private:
    int nodeid;
    // std::vector<char> program;
    std::string program;
    int count;
};

#endif
