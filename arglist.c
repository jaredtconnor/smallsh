#include "arglist.h" 

/* #######################################################
 * Function: create_arg_list()
 * Creates an empty and null list 
 * 
 * params: 
 *  void
 *
 * output: 
 *  struct arglist_t 
 *
 * ######################################################## */
struct arglist_t * create_arg_list() { 

    struct arglist_t * argument_list = NULL; 
    argument_list = malloc(sizeof(struct arglist_t)); 
    argument_list->size = 0; 
    argument_list->head = NULL;  

    return argument_list; 

}

/* #######################################################
 * Function: add_argument()
 * Adds an argument to a list
 * 
 * params: 
 *  1 - struct arglist_t * 
 *  2 - char * 
 *
 * output: 
 *  struct arglist_t 
 *
 * ######################################################## */
void add_argument(struct arglist_t * arguments, char * argument) { 

    // create and allocated memory for new node
    struct argnode_t * newNode = malloc(sizeof(struct argnode_t));
    newNode->next = NULL; 
    newNode->value = malloc(sizeof(char *));
    newNode->value = argument; 

    // append new node if needed 
    if (arguments->size == 0){ 
        arguments->head = newNode; 
        arguments->tail = newNode; 
    } else { 
        arguments->tail->next = newNode;
        arguments->tail = arguments->tail->next;
    }

    // using a size counter to improve speed
    arguments->size += 1;

    return;

}


/* #######################################################
 * Function: print_args()
 * Prints the arguments from the list
 * 
 * params: 
 *  1 - struct arglist_t * 
 *
 * output: 
 *  void
 *
 * ######################################################## */
void print_args(struct arglist_t * arguments){ 

    // temp pointer
    struct argnode_t * current = arguments->head;  

    while (current != NULL) { 
        printf("Current arg - %s\n", current->value);
        current = current->next;
    }

    return;

}

/* #######################################################
 * Function: destory_list()
 * Destroys the list and frees used memory
 * 
 * params: 
 *  1 - struct arglist_t * 
 *
 * output: 
 *  void
 *
 * ######################################################## */
void destroy_list(struct arglist_t * arguments){ 

    if(arguments->size == 0 ) { 

    } 
    
    else { 
        struct argnode_t * current = arguments->head; 
        struct argnode_t * temp = current; 

        while(current) { 
            temp = current; 
            current = current->next;
            destroy_node(temp);
        }

        current = NULL; 
        temp = NULL; 
        free(arguments);

    }

    return; 

}

/* #######################################################
 * Function: destroy_node()
 * Destorys or frees a node's data
 * 
 * params: 
 *  1 - struct arglist_t * 
 *
 * output: 
 *  void
 *
 * ######################################################## */
void destroy_node(struct argnode_t * argument){ 

    free(argument->value);
    free(argument);

    argument->value = NULL; 
    argument->next = NULL;

    return; 
}

/* #######################################################
 * Function: get_argument()
 * Gets the arguments
 * 
 * params: 
 *  1 - struct arglist_t * 
 *
 * output: 
 *  void
 *
 * ######################################################## */
char ** get_arguments(struct arglist_t * arguments) { 

    int list_size = arguments->size; 
    int list_size_terminated = arguments->size + 2; 
    char ** argument_list = malloc(list_size_terminated * sizeof(char *)); 

    struct argnode_t * current = arguments->head; 

    for (int i = 0; i < list_size; i++){ 

        argument_list[i] = current->value; 
        current = current->next; 

    }

    argument_list[list_size] = NULL; 

    return argument_list;

}
