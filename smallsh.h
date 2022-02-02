
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
#include "arglist.h"

#pragma once

#define PID_LEN   50 
#define VAR_EXPAND "$$"


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
    bool exit;
    bool input_redirect; 
    bool output_redirect;
    char * source;
    char * destination;
    bool is_comment;
    bool backgroundflag; 
    bool variablexpand; 
    bool emptyargs;
    bool builtin; 
    struct arglist_t * arguments; 
};

/* 
#########################################
Function Prototypes
#########################################
*/

struct command_input_t * parse_arguments(char *);
char * read_input();
bool check_comment(struct command_input_t *);
bool variable_expansion(char *);
bool check_background(struct command_input_t *);
bool check_inputredirect(struct command_input_t *, char *);
bool check_outputredirect(struct command_input_t *, char *);
bool check_built_in_command(struct command_input_t *);
bool check_background(struct command_input_t *);
void execute_built_in_command(struct command_input_t *);
void change_directory(struct command_input_t *);

//void change_directory(struct command_intput_t *);
