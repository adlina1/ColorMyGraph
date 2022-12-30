#include <stdio.h>
#include<stdint.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include<inttypes.h>
#include<stdint.h>
#include <regex.h> 
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define PORT 8888
#define NCOL 2
int nbNodes;

void createAdjMatrix(int n, int e, int Adj[][n+1], int arr[][2]);
void showAdjMatrix(int n , int Adj[][n + 1]);
int isNeighbor(int n, int Adj[][n+1], int site_a, int site_b);
void PortAllocation(int * p , int n);
void ReturnNeighbors(int id, int Adj[][nbNodes+1]);
void showMenu();

// Will be used to remove the 'e'
void removeChar(char* s, char c) {
    int j, n = strlen(s);
    for (int i = j = 0; i < n; i++)
        if (s[i] != c)
            s[j++] = s[i];
 
    s[j] = '\0';
}

// Get only the part of the file we want
char *preprocessing(char *filename, char **nbEdges, char **nbE, int *nbNodes, int *nb_row) {

    long int size = 0;
    FILE *file = fopen(filename, "r");
    
    if(!file) {
        fputs("File error.\n", stderr);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    char *result = (char *) malloc(size);
    if(!result) {
        fputs("Memory error.\n", stderr);
        fclose(file);
        return NULL;
    }


    char *line = NULL;  
    size_t len = 0;
    ssize_t read;  
    const char * separator = " ";

    regex_t regex;
    int pattern = regcomp(&regex, "p col ", 0);
    
    // Will be used to store M for col graphs 
    static char buffer[8];
    // strcpy(*nbE, &buffer[0]);
    *nbE = &buffer[0];


     // This allows us to read the number of nodes.
    char *str = filename, *p = str;
    while (*p) { // While there's still characters to process
        if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) {
            // We found a number 
            long val = strtol(p, &p, 10); // We read the number
            *nbNodes = val;
            break;
        } else {
            // Otherwise we look at the next character
            p++;
        }
    }


    while ((read = getline(&line, &len, file)) != -1) {

        if (strstr(line, "p edge ") == line || (strstr(line, "p col ") == line)){

            // Get the last token of a string. Avoid using strtok
            char *last = strrchr(line, ' ');
            if (last != NULL) {
                *nbEdges = last+1;
            }

            strcpy(buffer, *nbEdges);

            while ((read = getline(&line, &len, file)) != -1) {
                // printf("%s",line);
                strcat(result, line);
            }
            break; // we break the outer loop 
        }
    }

    // If we are dealing with one of those two files, then we divide M by a factor of 2.
    if (!strcmp(filename, "Graphs/dsjc250.5.col") | !strcmp(filename, "Graphs/dsjc500.9.col")){
        *nb_row = atoi(*nbEdges) / 2;
        printf("(1) nb edges : %d\n", *nb_row);
    } else {
        // Special treatment for these files (nbE)
        if ((!strcmp(filename, "Graphs/r250.5.col") | !strcmp(filename, "Graphs/r1000.1c.col") | 
            !strcmp(filename, "Graphs/r1000.5.col") | !strcmp(filename, "Graphs/C2000.5.col") |
            !strcmp(filename, "Graphs/C4000.5.col"))){
            *nb_row = atoi(*nbE); 
            printf("(2) nb edges : %d\n", *nb_row);     
        } else {
            *nb_row = atoi(*nbEdges);
            printf("(3) nb edges : %d\n", *nb_row); 
        }            
    } // end else

    fclose(file);
    return result;
}


void arrayConstruction( int *nb_rows, int givenArr[][2], char *input )
{
    const char * separator = " ";
    char * strToken;
    char *outt;
    int i;

    outt = strdup(input);

    strToken = strtok (outt, separator);

    for (i = 0; i < *nb_rows && strToken != NULL; i++)
    {
        for (int j = 0; j < 2 && strToken != NULL; j++)
        {
            givenArr[i][j] = strtol(strToken, NULL, 10);
            strToken = strtok(NULL, separator);
        }
    }
    free(outt);

    *nb_rows = i;

}


void createAdjMatrix(int n, int e, int Adj[][n+1], int arr[][2])  {

    // Intializing all values to 0
    for (int i=0; i<n+1; i++) {
        for (int j=0; j<n+1; j++) {
            Adj[i][j] = 0;
        }
    }
    // Creation of the adjacency matrix
    for (int i=0; i<e; i++) {
        int x = arr[i][0];     
        int y = arr[i][1];
        Adj[x][y] = 1;
        Adj[y][x] = 1;
    }

}

void showAdjMatrix(int n , int Adj[][n + 1])
{

    for (int i=1; i<n+1; i++) {
        for (int j=1; j<n+1; j++) {
            printf("%d ", Adj[i][j]);
        }
        printf("\n");
    }
}


typedef struct{
	int id;
	char *ip;

}Node;


void send_struct(int socket , Node c ){
	char buff[8096] ={'\0'};

	if(send(socket , &c.id, sizeof(int) , 0) <0)
	{
		perror("sending id failed");
            exit(EXIT_FAILURE);
	}
	// memset(&buffer[0], 0, sizeof(buffer));
	strcpy(buff,c.ip);

	send(socket, &buff, strlen(buff),0);
	printf("Struct sent succesfully\n");
}


// ********************************* MAIN *******************************************

int main(int argc , char *argv[])
{
    if(argc < 2) {
        fputs("Need the path of the graph as an argument.\n", stderr);
        return -1;
    } 

    nbNodes = 0;     
    char *nbEdges;
    char *nbE;
	int nb_row; 
    char *fileName = argv[1];
    char *result = preprocessing(fileName, &nbEdges, &nbE, &nbNodes, &nb_row);
    // Call of removeChar function
    removeChar(result, 'e');    
    int getNbEdges = atoi(nbEdges);
    int getNbE = atoi(nbE);
	printf("Found %d nodes.\n",nbNodes);
    printf("Found %d edges.\n",nb_row);

    int arr[nb_row][2];
    int len = sizeof arr / sizeof *arr;
    arrayConstruction(&len, arr, result);

    // Adjacency matrix
    int Adj[nbNodes+1][nbNodes+1];
    createAdjMatrix(nbNodes, nb_row, Adj, arr);
	// showAdjMatrix(nbNodes, Adj);


	int opt = TRUE;
	int srv_socket , addrlen , new_socket , client_socket[nbNodes] , i , valread , sd;
	int n = 0 ;
	int max_sd;
	struct sockaddr_in address;
	char buffer[4096]; 

	Node c [nbNodes];

	fd_set readfds;

	char *message = "Server is sending this msg \r\n";

	//initialise all client_socket[] to 0 
	for (i = 0; i < nbNodes; i++)
	{
		client_socket[i] = 0;
	}
	
	if( (srv_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	// main socket to allow multiple connections , works even without this
	if( setsockopt(srv_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
		
	if (bind(srv_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listening on port %d \n", PORT);

	if (listen(srv_socket, nbNodes) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	addrlen = sizeof(address);
	puts("Waiting for connections ...");


	while(TRUE)
	{	
		FD_ZERO(&readfds);				
		FD_SET(srv_socket, &readfds);   

		max_sd = srv_socket;
		
		//add sockets to set
		for ( i = 0 ; i < nbNodes ; i++)
		{
			sd = client_socket[i];
				
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			if(sd > max_sd)
				max_sd = sd;
		}

		if((select( max_sd + 1 , &readfds , NULL , NULL , NULL)< 0) && (errno!=EINTR))
		{
			printf("select error\n");
		}

		if (FD_ISSET(srv_socket, &readfds))
		{
			if ((new_socket = accept(srv_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" ,
			 new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

			//send new connection message
			if(send(new_socket, message, strlen(message), 0) != strlen(message))
			{
				perror("send");
			}
			puts("Welcome message sent");

	// Allocating id to each node that connects and save its id and ip in struct 
			if(n < nbNodes){
			client_socket[n] = new_socket;
			printf("Adding to list of sockets as %d\n" , n);
			c[n] = (Node){ .id = n+1 , .ip = inet_ntoa(address.sin_addr)};
			n++;
			}
		}
//************************************************************************************

	else{
		if( n == nbNodes)
		{ 

		for (i = 0; i < nbNodes; i++)
		{
			sd = client_socket[i];
    // calculate nb of its neighbors and send to node
			send(sd, &c[n].id, sizeof(int), 0);
			int k ;
			for(int j = 0; j < nbNodes; j++){
				k = 0;
				for (int j = 0; j < nbNodes; j++){
				if(Adj[i][j]==1){
					k++;
				}
				}
			}
			send(sd, &k , sizeof(int), 0);
	
	// send IDs of neighbors to corresponding node
			for (int j = 0; j < nbNodes; j++){
				if(Adj[i][j]==1){
							
				const int res = send(sd, &c[j].id , sizeof(int), 0);
						if(res == -1){
					printf("sending failed\n");
						exit(EXIT_FAILURE);
							}
						else if(res == 0){
						printf("disconnected\n");
							exit(EXIT_FAILURE);

						}
						printf("neighbor #%d sent to node %d\n", j , c[i].id);
						}

				}
	// send IPs of neighbors to corresponding node
		
			 for (int j = 0; j < nbNodes; j++){
				if(Adj[i][j]==1){
							strcpy(buffer, c[j].ip );
							send(client_socket[i], &buffer, strlen(buffer), 0);
							// printf("neighbors's ip sent to node %d\n", c[i].id);
							memset(&buffer[0], 0, sizeof(buffer));
						}
					}
				// 	//Somebody disconnected , get his details and print
					// getpeername(sd , (struct sockaddr*)&address , \
					// 	(socklen_t*)&addrlen);
					// printf("Host disconnected , ip %s , port %d \n" ,
					// 	inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
				close( sd );
				client_socket[i] = 0;  // FD_CLR(client_socket[i], &readfds);
			// }	
		}

	}  
  } // fin else

}

	return 0;
}
