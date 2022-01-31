#ifndef SMALLSH_H
#define SMALLSH_H

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
#include <stdbool.h>

#define MAX_ARGS      		512
#define MAX_INPUT_LENGTH    2048
#define PID_LEN   50 
#define TOKEN_DELIM " "

/*
Source - https://markkevinbaltazar.medium.com/c-header-files-and-prototypes-170ecdf526d
*/


/* 
#########################################
Data Members
#########################################
*/

/* 
commaind_input_t - Main command input struct

Data: 
    1 - command: char *
    2 - args: char * [MAX_ARGS]
    3 - backgroundProcess: bool
    4 - exit:  bool

*/ 
struct command_input_t { 

    char * command; 
    char * arglist[MAX_ARGS];
    bool exit;
    bool input_redirect; 
    bool output_redirect;
    char * source;
    char * destination;
    bool is_comment;
    bool backgroundflat; 

};

/* 
#########################################
Function Prototypes
#########################################
*/

struct command_input_t * parse_arguments(char * command);
int variable_expansion(char * command);
char * read_input();




#endif /* SMALLSH_H */
