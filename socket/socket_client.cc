#include "socket_client.h"
#include "conn_protocol.h"
#include <iostream>
#include <cstring>
#include <errno.h>

namespace conn {
    socket_client::socket_client(const char * hostname, 
            const char * service, const std::string &delimiter):
                                     connection (delimiter), 
		                     hostname(hostname),
				     service(service),
				     old_len(0) {
        memset(&old_buff, 0, sizeof old_buff);
	// set up hints for getaddrinfo() to fill up the 
	// follwing infomation.
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;

	// set up the structs we will need later on       
	int error;
	error = getaddrinfo(hostname, service, 
			&hints, &res); 
	if(error) {
	    // errx(1, "%s", gai_strerror(error));
	    // /* NOT_REACHED*/
	    std::cout<<"getaddrinfo error"<<std::endl;
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
	    std::cout<<"socket error"<<std::endl;
	    valid = false;
	    return;
	 }

	int status;
	status = connect(sockfd, res->ai_addr, res->ai_addrlen); 
	if(status == -1) {
	    // erro
	    std::cout<<"connect error "<<errno<<std::endl;
	    valid = false;
	    return;
	}
        old_buff = new char[MAX_BUFF_LEN];
        valid = true;
    }

    socket_client::~socket_client() {
	close(sockfd);
	freeaddrinfo(res);
	delete [] old_buff;
    }

    // len should be the length of msg preceding null termianted
    // character
    int socket_client::write(const char* msg, size_t len) const {
        int ret = conn_protocol::OK;
        
	// flags is set to 0, see man send for more info about
	// flags.
	int bytes_sent = send(sockfd, msg, len, 0); 
        if(bytes_sent == -1) {
            // error occurs
	    ret = conn_protocol::ERROR;
	}else if(bytes_sent < len) {
	    // resend the rest of the msg
	}

	return ret; 
    }
    
    int socket_client::read(char * buf, size_t max_len) {
        int ret = conn_protocol::OK;
	ssize_t byte_received = 0;
        // char tmp_buff[max_len];
	char * tmp_buff = new char[max_len];
	memset(tmp_buff, 0, max_len);
	
	// copy old buff into buf
	int index = 0;
	while(index<old_len) {
            
            buf[index] = old_buff[index];
            index++;
	    if(buf[index-1] == '\n') {
                // ready to return;
		if(index == old_len) {
		    // memset(&old_buff, 0, sizeof old_buff);
		    memset(old_buff, 0, MAX_BUFF_LEN);
		    old_len = 0;
		}else{
		    // re organzie old buff
		    int remain_c = old_len - index;
                    for(int i=0; i<remain_c; i++) {
		        old_buff[i] = old_buff[index++];
		    }
	            // forget to reset the old_len 
                    // sizeof should be used carefully
	            memset(old_buff+remain_c, 0,  MAX_BUFF_LEN - remain_c);	    
                    old_len = remain_c;
		}
                delete [] tmp_buff;
		return ret;
	    }
	}
        // release old buff
	memset(old_buff, 0, MAX_BUFF_LEN);
	old_len = 0;

        while((byte_received = recv(sockfd, tmp_buff, max_len, 0)) > 0) {
	    // append new data
	    int inx = 0;
            for(; inx<byte_received; inx++) {
		buf[index++] = tmp_buff[inx];
                if(tmp_buff[inx] == '\n') { break; }    
	    }
             
            // std::cout << buf << std::endl;
	    // see if we reach a newline character
	    if(inx != byte_received) {
		// '\n' found
		inx++;  // move to next character
		old_len = byte_received - inx;
		for(int j=0; j<old_len; j++) {
		    old_buff[j] = tmp_buff[inx++];
		}
		break;
	    }
	}
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

        delete tmp_buff;
	return ret;
    }
	
}
