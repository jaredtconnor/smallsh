#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#ifndef SMALLSH_H
#define SMALLSH_H

/*
Source - https://markkevinbaltazar.medium.com/c-header-files-and-prototypes-170ecdf526d
*/

#define MAX_ARGS      		512
#define MAX_INPUT_LENGTH    2048
#define PID_LEN   50 

/* 
#########################################
Data Members
#########################################
*/
struct input { 

    char * command; 
    char * args[MAX_ARGS];

};


/* 
#########################################
Function Prototypes
#########################################
*/

int variable_expansion(char * command);
char * read_input();


#endif /* SMALLSH_H */
