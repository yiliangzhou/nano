#include "socket_client.h"
#include "conn_protocol.h"
#include <iostream>

using namespace std;
using namespace conn;

int main(int argc, char** argv) {
    
	if(argc < 3) {
	    std::cout<<"usage: "<<argv[0]
		<<" "<<"<host> <portno>"<<std::endl;
		return -1;
    }
    
	socket_client sc(argv[1], argv[2]);
    char buff[50];
	memset(buff, 0, sizeof buff);
	sc.read(buff[0], 50);
	std::cout<<"msg from server: "<<buff<<std::endl;
    sc.write("Thank you!", 10);

    std::cout<<"done!"<<std::endl;
}
