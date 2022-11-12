#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h> 
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define NCOL 2

int N,M;

// Will be used to remove the 'e'
void removeChar(char* s, char c)
{
    int j, n = strlen(s);
    for (int i = j = 0; i < n; i++)
        if (s[i] != c)
            s[j++] = s[i];
 
    s[j] = '\0';
}

// Get only the part of the file we want
char *preprocessing(const char *filename, char **nbEdges)
{
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




while ((read = getline(&line, &len, file)) != -1) {

        if (strstr(line, "p edge ") == line){

            // Get the last token of a string. Avoid using strtok
            char *last = strrchr(line, ' ');
            if (last != NULL) {
                *nbEdges = last+1;
            }

            while ((read = getline(&line, &len, file)) != -1) {
                // printf("%s",line);
                strcat(result, line);
        }
        break; // we break the outer loop 
    }
}

    fclose(file);
    return result;
}


// Adj: Cr_GFK
void createAdjMatrix(int Adj[][N+1], int arr[][2]) {

    // Intializing all values to 0
    for (int i=0; i<N+1; i++) {

        for (int j=0; j<N+1; j++) {
            Adj[i][j] = 0;
        }
    }

    // Creation of the adjacency matrix
    for (int i=0; i<M; i++) {

        int x = arr[i][0];
        int y = arr[i][1];

        Adj[x][y] = 1;
        Adj[y][x] = 1;
    }
}


void showAdjMatrix(int Adj[][N + 1])
{

    for (int i=1; i<N+1; i++) {
        for (int j=1; j<N+1; j++) {
            printf("%d", Adj[i][j]);
        }
        printf("\n");
    }
}



int main(int argc, char **argv) 
{

    char *nbEdges;


    if(argc < 2) {
        fputs("Need an argument.\n", stderr);
        return -1;
    }

    char *fileName = argv[1];
    char *result = preprocessing(fileName, &nbEdges);
    // printf("nb edges of the graph = %s", nbEdges);

    int getNbEdges = atoi(nbEdges);

    if(!result) return -1;


    // in the fileName, reading number of nodes.
    int nbNodes = 0;
    char *str = fileName, *p = str;
    while (*p) { // Tant qu'il y a des caractères à traiter
        if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) {
            // Si on rentre dans le if, on a trouvé un nombre
            long val = strtol(p, &p, 10); // On lit le nombre
            nbNodes = val;
            break;
        } else {
            // Sinon on regarde le prochain caractère
            p++;
        }
    }

    printf("Found %d nodes.\n",nbNodes);

    removeChar(result, 'e');

    const char * separator = " ";
    char * strToken;
    char *outt;
    char *end;

    /* 1D array */
    int nb_row = (getNbEdges/2);

    int arrBuf[nb_row*NCOL];
    /* 2D array */
    int (*myArr)[NCOL] = (int(*)[NCOL])arrBuf;
    int i;
    outt = strdup(result);
    /* Store numbers as 1D array */
    strToken = strtok ( outt, separator );
    for (i = 0; i < nb_row*NCOL && strToken != NULL ; i++) {
    arrBuf[i] = (int)strtol(strToken, &end, 10);
    strToken = strtok ( NULL, separator);
    }
  
  /* Display numbers as 2D array */
  // for (i = 0; i < nb_row; i++) {
  //   printf("%d---%d\n", myArr[i][0], myArr[i][1]);
  // }

    /* Test - Check for a certain val */
  // printf("CHECK: %d\n", myArr[9][1]);
    
    N = nbNodes;
    M = nb_row; // M = # Edges

    // Adjacency matrix
    int Adj[N + 1][N + 1];


    createAdjMatrix(Adj, myArr);
    showAdjMatrix(Adj);


    free(outt);
    free(result);

    return 0;


} // END MAIN



