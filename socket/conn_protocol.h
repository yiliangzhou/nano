#ifndef __CONNECTION_PROTOCOL_H_INCLUED__
#define __CONNECTION_PROTOCOL_H_INCLUED__

namespace conn {

    class conn_protocol { 
      public:
        enum ret {OK, READ_ERR, WRITE_ERR, ERROR};
    };

}

#endif
