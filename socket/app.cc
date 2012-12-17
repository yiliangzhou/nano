#include "socket_server.h"
#include "conn_protocol.h"
#include <iostream>

using namespace std;
using namespace conn;

int main(int argc, char** argv) {

    if(argc < 2) {
        std::cout<<"usage: "<<argv[0]
            <<" "<<"<portno>"<<std::endl;
        return -1;
    }

    socket_server ss(argv[1], 10);
    int fd = ss.accept_client();
    ss.write(fd, "you are okay", 12);
    char buff[50];
    memset(buff, 0, sizeof buff);
    ss.read(fd, buff[0], 50);
    std::cout<<"msg from "<<fd<<": "<<buff<<std::endl;

    std::cout<<"done!"<<std::endl;	

}
