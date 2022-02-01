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
bool variable_expansion(char * command) { 

  bool result = false; 
  char pid_c[PID_LEN]; 
  int pid = getpid(); 
  memset(pid_c, '\0', sizeof(pid_c));

  char * buffer = NULL;
  char * current = command;
  char * temp = NULL;

  size_t dollar_len = strlen(VAR_EXPAND); 
  size_t pid_len;

  char * ret = strstr(command, VAR_EXPAND); 

  if (ret) { 
    result = true; 
    sprintf(pid_c, "%d", pid);
    buffer = malloc((strlen(command) + strlen(pid_c)) * sizeof(char));
    pid_len = strlen(pid_c);
    temp = &buffer[0];

    for (int i = 0; i < strlen(command) - 1; i++) { 


      char * ret_2 = strstr(current, VAR_EXPAND);

      if (ret_2 == NULL) {
        strcpy(temp, current);
      } else { 

        // Source - https://pubs.opengroup.org/onlinepubs/9699919799/strdup
        memcpy(temp, current, ret_2 - current); 
        temp += ret_2 - current; 

        memcpy(current, pid_c, pid_len);
        temp += pid_len; 

        temp = ret_2 + dollar_len;

      }
    }

    strcpy(command, buffer); 

  }

  return result; 

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
    struct arglist_t * arglist = create_arg_list();
    command_data->arguments = arglist; 

    /* 
     *  CITATION     
     *  Date - 1/30/2021 
     *  Adapted from: 
     *  Exploration - Strings 
     */ 
    char * token; 
    const char delim[4] = " ";

    // get the first token and save to command
    token = strtok(command, delim);
    command_data->command = calloc(strlen(token) + 1, sizeof(char)); 
    strcpy(command_data->command, token);
    token = strtok(NULL, delim);

    command_data->is_comment = check_comment(command_data);

    // reset token to next argument to buid list

    while (token != NULL && !command_data->is_comment) { 

      command_data->variablexpand = variable_expansion(command); 

      if (check_inputredirect(command_data, token)){ 
        command_data->input_redirect = true;
        add_argument(command_data->arguments, NULL);
      } else if (check_outputredirect(command_data, token)) { 
        command_data->output_redirect = true;
        add_argument(command_data->arguments, NULL);
      } else if(strcmp(token, " ") != 0) { 
        add_argument(command_data->arguments, token);
      }


      // // check if background process
      // if (strcmp(token, "&") == 0) { 
        // command_data->backgroundflat = true;
        // command_data->arglist[ctr] = NULL; 
        // break;
      // } 

      token = strtok(NULL, delim);
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

bool check_inputredirect(struct command_input_t * command_input, char * token) { 

  bool result = false;

  if(strcmp(token, "<") == 0){ 
    result = true;
  }

  return result;
  
}

bool check_outputredirect(struct command_input_t * command_input, char * token) { 

  bool result = false;

  if(strcmp(token, ">") == 0){ 
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
