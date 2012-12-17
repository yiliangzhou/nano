#include "connection.h"
#include <cstring>
#include <iostream>
namespace conn {
    connection::connection(const std::string &delimiter) :
      delimiter (delimiter) {
    
    }

    connection::~connection() {
    
    }

    int connection::write (const std::string & buf) {
      std::string to_send (buf + delimiter);
      return this->write(to_send.c_str(), to_send.length());
    }

    int connection::read (std::string & buf) {
      buf.clear();  
      const int BUF_SIZE = 4096;
      char *tmp_buf = new char[BUF_SIZE];
      memset (tmp_buf, 0, 4096);
      int ret = this->read(tmp_buf, BUF_SIZE);
      buf.assign(tmp_buf);
      buf = buf.substr (0, buf.find (delimiter));
      delete [] tmp_buf;
      return ret;
    }
}
