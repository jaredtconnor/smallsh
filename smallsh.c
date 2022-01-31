#include "smallsh.h"


/* #######################################################
 * Function: variable_expansion   
 * Searches a command line to confirm if there is '$$' 
 * requesting a variable expansion
 * 
 * params: 
 *  1 - char * command - pointer to command line being passed
 *
 * output: 
 *  1 - int - int indicating result, 0 correct and 1 error
 *
 * ######################################################## */
int variable_expansion(char * command) { 

  char * ret = strstr(command, "$$"); 

  if (ret) { 
    char pid[PID_LEN]; 
    sprintf(pid, "%d", getpid());
  }
  
  return 0;
}

/* #######################################################
 * Function:  read_input 
 *
 * Reads in stdin and returns a char * to be parsed
 * 
 * params: void
 *
 * output: char *
 *
 * ######################################################## */
char * read_input(){ 
	
	printf(": ");
	fflush(stdout);
	
	char * command = NULL;
	size_t len = 0; 
	
	getline(&command, &len, stdin);
	
	//check variable expansion
  variable_expansion(command); 
	
  return command;
}



/* #######################################################
 * Function: parse_arguments 
 * Parses the arguments from a command line and returns a 
 * struct of input that contains the arguments and details 
 * passed
 * 
 * params: 
 *  1 - char * command - pointer to the command passed
 *
 * output: c
 *
 * ######################################################## */
struct command_input_t * parse_arguments(char * command) { 
    
    struct command_input_t * command_data = malloc(sizeof(struct command_input_t)); 
    int ctr = 0;

    /* 
     *  CITATION     
     *  Date - 1/30/2021 
     *  Adapted from: 
     *  Exploration - Strings 
     */ 
    char * token; 

    token = strtok(command, TOKEN_DELIM);
    command_data->command = calloc(strlen(token) + 1, sizeof(char)); 
    strcpy(command_data->command, token);

    command_data->is_comment = check_comment(command_data);

    while (token != NULL && !command_data->is_comment) { 

      // check built in
      if(check_built_in_command(command_data)) { 
          printf("Built in command detected\n");
      }

      // check if redirect
      // if (strcmp(token, ">") == 0) { 

        // command_data->input_redirect = true;
        // command_data->arglist[ctr] = NULL; 

      // } else if (strcmp(token, "<") == 0) { 

        // command_data->output_redirect = true;
        // command_data->arglist[ctr] = NULL; 

      // } else { 
        // command_data->input_redirect = false; 
        // command_data->output_redirect = false; 
        // continue; 
      // }


      // // check if background process
      // if (strcmp(token, "&") == 0) { 
        // command_data->backgroundflat = true;
        // command_data->arglist[ctr] = NULL; 
        // continue;
      // } 

      command_data->arglist[ctr] = token;

      token = strtok(NULL, TOKEN_DELIM);
    }

    return command_data;

}

bool check_comment(struct command_input_t * command_input){ 

    bool result = false;
    char * found = NULL;

    found = strstr(command_input->command, "#");
 
    if (found) { 
     result = true; 
    } 

    return result; 
}

bool check_built_in_command(struct command_input_t * command_input) {

  bool result = false; 

  if (strcmp(command_input->command, "cd") == 0 || strcmp(command_input->command, "status") == 0){ 
    result = true;
  }

  return result;
}
