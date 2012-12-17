#ifndef __SOCKET_CLIENT_H_INCLUDED__
#define __SOCKET_CLIENT_H_INCLUDED__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "connection.h"
#include "conn_protocol.h"

namespace conn {
    const int MAX_BUFF_LEN = 4096;

    class socket_client : public connection {
      private:
	// host name and port number 
	const std::string hostname;
	const std::string service;
	// valid flag
	bool valid;

	// socket descriptor
	// sockfd is assigned when socket() is called
	int sockfd;
	struct addrinfo *res;

	int old_len;
	 
	//std::string old_buff;
	// char *old_buff[MAX_BUFF_LEN];
	char * old_buff;
      public:
	socket_client(const char* hostname, const char* service, 
                const std::string &delimiter);
	~socket_client();

	int write(const char* str, size_t len) const;
	int read(char *buf, size_t max_len);
    };
}

#endif
