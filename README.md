nano
====

Reference Implementation for Adversarial Nanomunchers Protocol


build and test
==

to build the solution, follow the following two commands
`make server`
`make client`

to start server, follow following command:
`./server nanomuncher_num input.txt port_no`
a example would looks like:
`./server 10 input.txt 2008`

to start a client, follow following command: 
`./client team_name port_no`
a example would looks like:
`./client my_team_name 2008`


what have been done
==

* inter processes communication over socket, following our defined protocol.
* defined and implemented various entity classes to represent our game.
* the main logic, or control flow, for both server and client.
* at server side, play all nanomunchers for one round, resolve conflicts if any.
* at client side, give a random strategy.
* output a command line graph of the board.

todo list
==

* simplify nano_client.cc, move logic to player class.
* may be add multithread into server side, and add time constraint. 
* enhance the socket package.


other notes
==

This implementation relys on boost library.
