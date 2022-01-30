#include "smallsh.h"

char * get_input(){ 

    char *buffer = malloc(MAX_INPUT_LENGTH * sizeof(char));
    char *line = malloc(MAX_INPUT_LENGTH * sizeof(char));
    int buffer_empty = 0;

    memset(line, '\0', sizeof(strlen(line))); 

    fflush(stdout); 
    printf(": ");

    do {

        fgets(buffer, MAX_INPUT_LENGTH, stdin); 

        strcat(line, buffer); 

        if (line[0] == 0) { 
            buffer_empty = 1; 
        } else if (line[strlen(line) - 1] == '\n') { 
            line[strlen(line) - 1] = '\0'; 
            buffer_empty = 1;
        }

    } while (buffer_empty != 1);

    free(buffer); 
    return line; 
}