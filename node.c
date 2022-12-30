#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include<stdint.h>
#define PORT 8888
#define TRUE 1


typedef struct{
	int id;
	char *ip;

}Node;



Node rec_struct(int sock , Node c){
	char buff[8096] = {'\0'};	

	if(recv(sock , &c.id, sizeof(c.id) , 0) <0)
	{
		perror("receiving id failed");
            exit(EXIT_FAILURE);
	}

	if(recv(sock , &buff, sizeof(buff)-1,0) < 0) 
	{
		perror("receiving ip failed");
            exit(EXIT_FAILURE);
	}	
	c.ip = buff;  	
	return c;
}


int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd;
	//int valread;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client ";
	char buffer[8196] = { 0 };
	int id;
	int num_neighbors;
	const char* server_ip;
	int node_fd;
	Node neighbors[num_neighbors];
	
	if(argc < 2){
		printf("Usage : %s Enter server ip address\n", argv[0]);
		exit(1);
	} 
	
	server_ip = argv[1];
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(server_ip);
	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	/*if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}*/
	if ((client_fd
		= connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	
	send(sock, hello, strlen(hello), 0);
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));
	

	// Receive id and num. of neighbors from server
	recv(sock, &id, sizeof(int), 0);
	printf("Your id : %d\n", id);
	recv(sock, &num_neighbors, sizeof(int), 0);
	printf("Number of neighbors of node %d : %d\n", id, num_neighbors);

	for (int i = 0; i < num_neighbors; i++){
		const int res = recv(sock, &neighbors[i].id, sizeof(int), 0);
		if(res == -1){
			printf("error receiving\n");
			// exit(EXIT_FAILURE);
		}
		else if(res == 0){
			printf("disconnected\n");
		}
		printf("ID of neighbor#%d of node #%d: %d\n", i, id, neighbors[i].id);
		// printf("IP of neighbor #%d : %s\n", i, neighbors[i].ip);
	}


	for (int i = 0; i < num_neighbors; i++){
		const int res = recv(sock, &buffer, sizeof(buffer)-1, 0);
		if(res == -1){
			printf("error receiving\n");
			// exit(EXIT_FAILURE);
		}
		else if(res == 0){
			printf("disconnected\n");	
		}
		neighbors[i].ip = buffer;
		
		printf("ip of neighbor #%d of node#%d : %s\n", i, id, neighbors[i].ip);
		memset(&buffer[0], 0, sizeof(buffer));
	}

	// for (int i = 0; i < num_neighbors; i++){
	// 	printf("ID of neighbor #%d : %d\n", i, neighbors[i].id);
	// 	printf("IP of neighbor #%d : %s\n", i, neighbors[i].ip);
	// }

	close(client_fd);


	return 0;
}

