#ifndef __SOCKET_SERVER_H_INCLUDED__
#define __SOCKET_SERVER_H_INCLUDED__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "conn_protocol.h"

namespace conn {

    class socket_server {
        public:
            socket_server(const char* service, const int backlog,
                   const std::string &delimiter);
            ~socket_server();

            int accept_client ();
            bool is_good();

            int write(const int client_fd, const char* msg, size_t len) const;
            int read(const int client_fd, char* buf, size_t max_len);

            int write(const int client_fd, const std::string & msg);
            int read(const int client_fd, std::string & buf);
         private:
            // host name and port number 
            // const std::string hostname;
            const std::string service;
            const int backlog;
            const std::string delimiter;
            // std::vector<int> client_fds;
            // valid flag
            bool valid;

            // socket descriptor
            // sockfd is assigned when socket() is called
            int sockfd;
            struct addrinfo *res;
    };
}

#endif
