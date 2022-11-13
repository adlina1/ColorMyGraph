#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h> 
#include <ctype.h>

#define NCOL 2

// N: number of nodes.
// M: number of edges
int N,M;
const int true = 1;
const int false = 0;


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


int isNeighbor(int Adj[][N+1], int site_a, int site_b){
    if (site_a > N || site_b > N){
        printf("Error: Greater than the number of nodes. Please give another try.\n");
        exit(EXIT_FAILURE);
    } 
    if (site_a == 0 || site_b == 0){
        printf("Error: A site can't be 0. It is at least 1. Please give another try.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < site_a + 1; i++) {
        for (int j = 1; j < site_b + 1; j++) {
                if(i == site_a && j == site_b){
                    if(Adj[i][j] == 1){
                        return true;
                    }
                    return false;
                }        
        }
    }
}



int main(int argc, char **argv) 
{

    if(argc < 2) {
        fputs("Need an argument.\n", stderr);
        return -1;
    } 

    int nbNodes = 0;   
    char *nbEdges;
    char *nbE;
    // real # of edges value
    int nb_row; 
    char *fileName = argv[1];
    char *result = preprocessing(fileName, &nbEdges, &nbE, &nbNodes, &nb_row);
    // Call of removeChar function
    removeChar(result, 'e');    
    int getNbEdges = atoi(nbEdges);
    int getNbE = atoi(nbE);
    N = nbNodes;
    M = nb_row; 


    printf("Found %d nodes.\n",nbNodes);
    printf("Found %d edges.\n",nb_row);


    int arr[nb_row][2];
    int len = sizeof arr / sizeof *arr;
    arrayConstruction(&len, arr, result);
  
  /* Display numbers as 2D array */
  // int i;
  // for (i = 0; i < nb_row; i++) {
  //   printf("%d---%d\n", arr[i][0], arr[i][1]);
  // }

    // Adjacency matrix
    int Adj[N+1][N+1];
    createAdjMatrix(Adj, arr);
    showAdjMatrix(Adj);

    /* Test - Check whether they are neighbors */
    // int val = isNeighbor(Adj, 2, 3);
    // printf("----> %d\n", val);

    free(result);

    return 0;

} // END MAIN



