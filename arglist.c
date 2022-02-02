#include "arglist.h" 

struct arglist_t * create_arg_list() { 

    struct arglist_t * argument_list = NULL; 
    argument_list = malloc(sizeof(struct arglist_t)); 
    argument_list->size = 0; 
    argument_list->head = NULL;  

    return argument_list; 

}

void add_argument(struct arglist_t * arguments, char * argument) { 

    struct argnode_t * newNode = malloc(sizeof(struct argnode_t));
    newNode->next = NULL; 
    newNode->value = malloc(sizeof(char *));
    newNode->value = argument; 

    if (arguments->size == 0){ 
        arguments->head = newNode; 
        arguments->tail = newNode; 
    } else { 
        arguments->tail->next = newNode;
        arguments->tail = arguments->tail->next;
    }

    arguments->size += 1;

    return;

}


void print_args(struct arglist_t * arguments){ 

    struct argnode_t * current = arguments->head;  

    while (current != NULL) { 

        printf("Current arg - %s\n", current->value);
        current = current->next;
    }

    return;

}

void destroy_list(struct arglist_t * arguments){ 

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

    return; 

}

void destroy_node(struct argnode_t * argument){ 

    free(argument->value);
    free(argument);

    argument->value = NULL; 
    argument->next = NULL;

    return; 
}


bool checkempty_list(struct arglist_t * arguments){ 

    bool result = false;

    if (arguments->size == 0) { 
        result = true;
    }

    return result;
}

