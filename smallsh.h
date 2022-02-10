
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
#define NUM_PROCESSES 50


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
    char * infile;
    bool output_redirect;
    char * outfile;
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
void exec_mode_signal_handler(int);
bool check_comment(char *);
bool check_variable_expansion(char *);
char * expand_pid(char *);
bool check_background(char *);
bool check_inputredirect(char *);
bool check_outputredirect(char *);
bool check_built_in_command(char *);
bool check_exit(char *);
void execute_built_in_command(struct command_input_t *, int *);
void show_status(struct command_input_t *, int *, bool *);
void execute_fork(struct command_input_t *, int *, struct sigaction *);
void execute_foreground(struct command_input_t *, int *,  struct sigaction *);
void execute_background(struct command_input_t *, int *, struct sigaction *);
void print_background();
