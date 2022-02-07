#include "smallsh.h"


bool BACKGROUND_PROCESSES = true; 
pid_t spawnpid = -5;

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

  char * ret = NULL; 
  ret = strstr(command, VAR_EXPAND); 

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
    command_data->command = calloc(strlen(token) + 1, sizeof(char *)); 
    strcpy(command_data->command, token);
    token = strtok(NULL, delim);

    // reset token to next argument to buid list

    while (token != NULL && !command_data->is_comment) { 

      if (check_inputredirect(command_data, token)){ 

        command_data->input_redirect = true;
        command_data->infile = strtok(NULL, delim); 
        token = strtok(NULL, delim); 

      } else if (check_outputredirect(command_data, token)) { 

        command_data->output_redirect = true;
        command_data->outfile = strtok(NULL, delim); 
        token = strtok(NULL, delim); 

      } else { 

        command_data->emptyargs = checkempty_list(command_data->arguments);
        command_data->variablexpand = variable_expansion(token); 
        command_data->is_comment = check_comment(command_data);
        command_data->backgroundflag = check_background(command_data);

        if (!command_data->emptyargs && !command_data->is_comment) { 

          add_argument(command_data->arguments, token);

        }
      }


      token = strtok(NULL, delim);
    }

    return command_data;

}

void signal_handler(int signal_no){ 

  bool current_process = false; 
  char message[50]; 
  memset(message, '\0', sizeof(strlen(message)));


  char * fg_message = "Entering forground mode"; 
  char * bg_messgae = "Enabling background mode";

  while(waitpid(spawnpid, NULL, WNOHANG) == 0) { 
    current_process = true; 
  };

  if(BACKGROUND_PROCESSES && current_process) { 
    strcpy(message, fg_message); 
    BACKGROUND_PROCESSES = false; 
  } else if (BACKGROUND_PROCESSES && !current_process) { 

      strcpy(message, fg_message); 
      strcat(message, ": "); 
      BACKGROUND_PROCESSES = true; 
  } else if (!BACKGROUND_PROCESSES && current_process) { 
      strcpy(message, bg_messgae); 
      BACKGROUND_PROCESSES = true;
  } else { 
      strcpy(message, bg_messgae); 
      strcat(message, ": ");
      BACKGROUND_PROCESSES = true;
  }

  fflush(stdout); 
  write(STDOUT_FILENO, message, strlen(message)); 

  return;

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

bool check_background(struct command_input_t * command_input){ 

  bool result = false;
  char * found = NULL;

  found = strstr(command_input->command, "&");

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

bool check_exit(char * token) { 

  bool result = true; 

  if(strcmp(token, "exit") == 0){ 
    result = false;
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

void execute_built_in_command(struct command_input_t * command_input, int * exit_code, bool * foreground_permitted) { 

  if (strcmp(command_input->command, "cd") == 0){ 
    change_directory(command_input);
  } else if (strcmp(command_input->command, "status") == 0) { 

    if(command_input->arguments->size > 1 && command_input->backgroundflag) { 

      *foreground_permitted = true; 
      show_status(command_input, exit_code, foreground_permitted); 
      *foreground_permitted = false; 

    } else { 
      show_status(command_input, exit_code, foreground_permitted); 
    }
  }

  return; 

}

void change_directory(struct command_input_t * command_input){

  if (command_input->arguments->size > 1){ 
    chdir(command_input->arguments->head->value); 
  } else { 
    chdir(getenv("HOME"));
  }

  return;

}

void show_status(struct command_input_t * command_input, int* exit_code, bool * foreground_permitted) { 

  fflush(stdout); 

  if(*foreground_permitted) { 
    printf("Terminated by signal %d\n", *exit_code);
  } else { 
    printf("exit value %d\n", *exit_code);
  }


  return;

}

void execution_fork(struct command_input_t * command_input, int * exit_code, bool * foreground_permited) { 

  if(command_input->backgroundflag && BACKGROUND_PROCESSES) { 

      pid_t background_id = fork(); 

      switch (background_id) { 
        case -1: 
          perror("Error - failed process");

        case 0: 
          execute_background(command_input);
          break;  


        default: 
          fflush(stdout); 
          printf("Background pid -%d", background_id);
          break; 
      }
  } else { 

    //execute_foreground(command_input, exit_code, foreground_permited);
    //execute_foreground(command_input, exit_code);

  }

  return; 

}

void execute_foreground(struct command_input_t * command_input, int * status, int * bg_processes, struct sigaction * sigint_action){ 

  char ** arguments = get_arguments(command_input->arguments); 
  pid_t spawnpid;
  spawnpid = fork(); 

  switch (spawnpid)
  {
  case -1:

    perror("ERROR - fork() failed \n");
    exit(1);

    break;

  case 0: 

    if (command_input->backgroundflag == false) { 
      sigint_action->sa_handler = SIG_DFL;
      sigaction(SIGINT, sigint_action, NULL);
    }  

    printf("Command is %s\n", command_input->command);
    execvp(command_input->command, arguments);
    perror("excevp error\n");
    break;

  default: // parent process

    // background process 
    if (command_input->backgroundflag) { 
      
      printf("background pid is %d\n", spawnpid);
      fflush(stdout); 
      spawnpid = waitpid(spawnpid, status, WNOHANG); 

      int i = 0; 
      while(bg_processes[i] != NULL) { 
        i++; 
      }

      bg_processes[i] = spawnpid;

    } else { // foreground process

      spawnpid = waitpid(spawnpid, status, 0); 


      if(WTERMSIG(*status)) { 
        printf("Terminated by signal %d\n", WTERMSIG(*status)); 
        fflush(stdout); 
      }


      sigint_action->sa_handler = SIG_IGN; 
      sigaction(SIGINT, sigint_action, NULL);

    }


    while ((spawnpid = waitpid(-1, status, WNOHANG)) > 0) { 

      if (WIFEXITED(*status)) { 
        printf("background pid %d is done: exit value was %d\n", spawnpid, WEXITSTATUS(*status)); 
        fflush(stdout); 

      } else { 
        printf("background pid %d is done: terminated by signal %d\n", spawnpid, WTERMSIG(*status)); 
      }

      for (int i = 0; i<200; i++) { 

        if(bg_processes[i] == spawnpid) { 
          bg_processes[i] = NULL; 
        }
      }
    }
  }
  return;
}

void execute_background(struct command_input_t * command_input) { 


  // signal(sigint, sig_ign); 
  // signal(sigtstp, sig_ign); 

  int * pid_process = malloc(sizeof(int)); 
  * pid_process = getpid(); 

  // file * file_in; 
  // FILE * file_out; 
  char ** arguments;

  char * exit_message = malloc(256 * sizeof(char)); 
  memset(exit_message, '\0', sizeof(strlen(exit_message)));

  arguments = get_arguments(command_input->arguments);


  execvp(command_input->command, arguments); 

  return;

}

void print_background_process() { 

  int childstatus = -1; 
  int child_pid =  waitpid(-1, &childstatus, WNOHANG); 

  while (child_pid > 0) { 

    if(WIFEXITED(childstatus)) { 

      int exit_code = WEXITSTATUS(childstatus); 
      fflush(stdout); 
      printf("Background pid %d is done, exit value %d\n", 
        child_pid, 
        exit_code);

    } else if (WIFSIGNALED(childstatus)) { 
      int termsig = WTERMSIG(childstatus); 
      fflush(stdout); 
      printf("Background pid %d is done, exit value %d\n", 
        child_pid, 
        termsig 
      );
    }


    child_pid = waitpid(-1, &childstatus, WNOHANG); 
  }

  return; 

}

