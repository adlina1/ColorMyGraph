#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h> 
#include <ctype.h>

char *read_from_file(const char *filename)
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

    if(fread(result, 1, size, file) != size) {
        fputs("Read error.\n", stderr);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return result;
}


int main(int argc, char **argv) 
{

    regex_t regex;
    int pattern = regcomp(&regex, "p edge ", 0);

    if(argc < 2) {
        fputs("Need an argument.\n", stderr);
        return -1;
    }

    char *fileName = argv[1];
    char *result = read_from_file(fileName);
    //char *occurrence = strchr(result, getNext);

    int matching = regexec(&regex, result, 0, NULL, 0);
    if(matching == 0){
        printf("Pattern found.\n");
    } else {
        printf("Pattern not found.\n");
    }


    if(!result) return -1;


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

    printf("Found %d nodes.",nbNodes);

    //fputs(result, stdout);
    free(result);

    return 0;
}
