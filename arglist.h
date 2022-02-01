#pragma once

#define MAX_ARGS      		512
#define MAX_INPUT_LENGTH    2048

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdbool.h>


/* 
#########################################
Data Members
#########################################
*/
struct argnode_t { 
    char * value;
    struct argnode_t * next;
};


struct arglist_t { 
    int size; 
    struct argnode_t * head;
    struct argnode_t * tail; 
};


/* 
#########################################
Function Prototypes
#########################################
*/

struct arglist_t * create_arg_list(); 
void add_argument(struct arglist_t*, char *); 
char ** get_argument(struct arglist_t *);
void print_args(struct arglist_t *);
void destroylist(struct arglist_t *);
void destroynode(struct argnode_t *);