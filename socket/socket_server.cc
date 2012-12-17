#include "socket_server.h"
#include "conn_protocol.h"
#include <iostream>
#include <cstring>
#include <errno.h>

namespace conn {

  socket_server::socket_server(const char * service,
      const int backlog,
      const std::string &delimiter) :
        service(service),
        backlog(backlog),
        delimiter(delimiter),
        sockfd(-1) {
    // set up hints for getaddrinfo() to fill up the 
    // follwing infomation.
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // set up the structs we will need later on       
    int error;
    error = getaddrinfo(NULL, service, 
            &hints, &res); 
    if(error) {
      /* NOT_REACHED*/
      std::cout<<"[Socket] getaddrinfo error"<<std::endl;
      valid = false;
      return;
    }

    // create a socket using infomation in structs we get
    // from calling getaddrinfo.
    sockfd = socket(res->ai_family, res->ai_socktype,
            res->ai_protocol);
    // assert(sockfd >= 0);
    if(sockfd == -1) {
        // a error occurs, return the error info
        std::cout<<"[Socket] socket error"<<std::endl;
        valid = false;
        return;
    }

    int status;
    status = bind(sockfd, res->ai_addr, res->ai_addrlen); 
    if(status == -1) {
        // error
        std::cout<<"[Socket] bind error "<<errno<<std::endl;
        valid = false;
        return;
    }

    status = listen(sockfd, backlog);
    if(status == -1) {
        // a error occurs, return the error info
        std::cout<<"[Socket] listen error"<<std::endl;
        valid = false;
        return;
    }
    std::cout<<"[Socket] server is listening on "<<service<<std::endl;
    valid = true;
  }

  socket_server::~socket_server() {
      /*
      for (int i = 0; i < client_fds.size(); i++) {
          close (client_fds[i]);
      }*/
      if (sockfd > 0) { close(sockfd); }
      freeaddrinfo(res);
  }

  bool socket_server::is_good () {
      return valid;
  }
  
  int socket_server::accept_client() {
      std::string hostName;
      struct sockaddr_storage their_addr;
      socklen_t addr_size = sizeof their_addr;
      int client_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
      if ( client_fd < 0 ) { return -1; }
      
      std::cout<<"[Socket] new client accecpted!"<<std::endl; 
      // client_fds.push_back (client_fd);
      // store addr in a data structure 
      return client_fd;
  }

  // len should be the length of msg preceding null termianted
  // character
  int socket_server::write(const int client_fd, const char* msg, size_t len) const {
      int ret = conn_protocol::OK;

      // flags is set to 0, see man send for more info about
      // flags.
      int bytes_sent = send(client_fd, msg, len, 0); 
      if(bytes_sent == -1) {
          // error occurs
          ret = conn_protocol::ERROR;
      }else if(bytes_sent < len) {
          // resend the rest of the msg
          // TODO: add resend below
          std::cout << "[Socket] less bytes sent!!!" << std::endl;
      }
      return ret; 
  }

  int socket_server::read(const int client_fd, char* buf, size_t max_len) {
      int ret = conn_protocol::OK;

      int byte_received = recv(client_fd, buf, max_len, 0);
      switch (byte_received) {
          case -1:
              // error
              ret = conn_protocol::ERROR;
              break;
          case 0:
              // remote side has closed the connection
              ret = conn_protocol::ERROR;
              break;
          default:
              // success
              break;
      }

      return ret;
  }

  int socket_server::write(const int client_fd, const std::string & msg) {
      std::string to_send (msg + delimiter);
      return write(client_fd, to_send.c_str(), to_send.length());
  }

  int socket_server::read(const int client_fd, std::string & buf) {
      char to_receive[4096];  
      int ret = read(client_fd, &to_receive[0], 4096);

      buf.assign(to_receive); 
      buf.assign( buf.substr(0, buf.find(delimiter)) );    
      return ret;
  }
}
