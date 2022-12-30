cgraph: program1 program2

program1: node.c
	gcc -o enode node.c

program2: server_central.c
	gcc -o eserver_central server_central.c
