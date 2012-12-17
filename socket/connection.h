#ifndef __CONNECTION_H_INCLUDED__
#define __CONNECTION_H_INCLUDED__


// included head files
#include "conn_protocol.h"
#include <stdlib.h>
#include <string>

namespace conn {
    // this class provides an abstraction of sending and retreiving 
    // plain test  
    class connection {
      public:  
	connection(const std::string &delimiter);
        virtual ~connection();
        // all string should be \n terminated.
        virtual int write (const char* str, size_t len) const = 0;
	// read in a new line terminated string into buf
        virtual int read (char *buf, size_t max_len) = 0;	
        int write ( const std::string & );
        int read ( std::string & );
      private:
        std::string delimiter;
    };
}

#endif
