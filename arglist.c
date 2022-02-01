#include "arglist.h" 

struct arglist_t * create_arg_list() { 

    struct arglist_t * argument_list = malloc(sizeof(struct arglist_t)); 
    argument_list->size = 0; 
    argument_list->head = NULL;  

    return argument_list; 

}

void add_argument(struct arglist_t * arguments, char * argument) { 

    struct argnode_t * newNode = malloc(sizeof(struct argnode_t));
    newNode->value = argument; 

    if (arguments->size == 0){ 
        arguments->head = newNode; 
    } else { 
        arguments->head->next = newNode; 
    }

    return;

}






