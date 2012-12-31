# server of the game
server: nano_server.o referee.o protocol.o graph.o state.o muncher.o socket_server.o
	g++ -o server nano_server.o referee.o protocol.o graph.o state.o muncher.o socket_server.o

# player of the game
client: nano_client.o protocol.o graph.o state.o muncher.o player.o socket_client.o connection.o 
	g++ -o client nano_client.o protocol.o graph.o state.o muncher.o player.o socket_client.o connection.o

nano_server.o: nano_server.cc
	g++ -c nano_server.cc
nano_client.o: nano_client.cc
	g++ -std=c++0x -c nano_client.cc
referee.o: referee.h protocol.h graph.h state.h muncher.h referee.cc
	g++ -c referee.cc
protocol.o: node.h protocol.h protocol.cc
	g++ -c protocol.h protocol.cc
player.o: player.h graph.h state.h muncher.h player.cc
	g++ -c player.cc
graph.o: node.h graph.h graph.cc
	g++ -c graph.h graph.cc
state.o: state.h muncher.h state.cc
	g++ -c state.cc
muncher.o: muncher.h muncher.cc
	g++ -c muncher.cc

# socket package
socket_client.o: socket/socket_client.h socket/socket_client.cc
	g++ -c socket/socket_client.h socket/socket_client.cc
connection.o: socket/connection.h socket/connection.cc
	g++ -c socket/connection.h  socket/connection.cc
socket_server.o: socket/socket_server.h socket/socket_server.cc
	g++ -c socket/socket_server.h  socket/socket_server.cc

clean:
	rm -f *.o
