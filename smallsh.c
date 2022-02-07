#include "smallsh.h"

bool BLOCKBACKGROUND = true; 
//pid_t spawnpid = -5;

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

  if(strcmp(token, "<") == -1){ 
    result = true;
  }

  return result;
  
}

bool check_outputredirect(struct command_input_t * command_input, char * token) { 

  bool result = false;

  if(strcmp(token, ">") == -1){ 
    result = true;
  }

  return result;
  
}

bool check_exit(char * token) { 

  bool result = false; 

  if(strcmp(token, "exit") == 0){ 
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
bool variable_expansion(struct command_input_t * command_input, char * token) { 

  bool result = false;
  char * found = NULL;

  found = strstr(token, "$$");

  if (found) { 
    result = true; 
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

    // get the first token and save to command
    token = strtok(command, " ");
    command_data->command = calloc(strlen(token) + 1, sizeof(char *)); 
    strcpy(command_data->command, token);
    token = strtok(NULL, delim);
    command_data->exit = check_exit(command_data->command);

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
        command_data->is_comment = check_comment(command_data);
        command_data->backgroundflag = check_background(command_data);
        command_data->variablexpand = variable_expansion(command_data, token); 

        if (!command_data->emptyargs && !command_data->is_comment) { 

          add_argument(command_data->arguments, token);

        }
      }


      token = strtok(NULL, delim);
    }

    return command_data;

}


void execute_built_in_command(struct command_input_t * command_input, int * status) { 

  if (strcmp(command_input->command, "cd") == 0){ 

    if (command_input->arguments->size > 1){ 

      chdir(command_input->arguments->head->value); 

    } else { 

      chdir(getenv("HOME"));

    }

  } 

  // else if (strcmp(command_input->command, "status") == 0) { 

    // if(WIFEXITED(&status)) { 

      // printf("Exit value from %d\n", WEXITSTATUS(&status)); 
      // fflush(stdout); 

    // } else { 

      // printf("Terminated by singal %d\n", WTERMSIG(&status)); 
      // fflush(stdout); 


    // }


  // }

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

void execute_command(struct command_input_t * command_input, int * status, int * background_processes, struct sigaction * sigint_action) { 

  char ** arguments = get_arguments(command_input->arguments); 
  pid_t spawnpid = fork(); 

  switch (spawnpid) { 

    case -1: 
      perror("fork() error\n");
      exit(1); 
      break;


    // child process
    case 0: 

      if (command_input->backgroundflag == false) { 

        sigint_action->sa_handler = SIG_DFL; 
        sigaction(SIGINT, sigint_action, NULL); 

      }

      if (command_input->input_redirect){ 

        int fileinput = open(command_input->infile, O_RDONLY); 

        if (fileinput == -1) {
          perror("cannot open file\n");
          exit(1); 

        }

        int result = dup2(fileinput, 0); 
        if(result == -1){ 
          perror("cannot redirect file\n");
          exit(2); 
        }

        fcntl(fileinput, F_SETFD, FD_CLOEXEC); 

      }

      if (command_input->output_redirect == true) { 

        int filewrite = open(command_input->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 

        if (filewrite == -1) { 

          perror("Cannot open file to write\n");
          exit(1); 
        }

        int result = dup2(filewrite, 1); 

        if(result == -1){ 
          perror("cannot redirect file\n");
          exit(2); 
        }

        fcntl(filewrite, F_SETFD, FD_CLOEXEC); 

      }

      execvp(command_input->command, arguments);
      perror("############### execvp error \n"); 
      exit(2); 
      break;

    default: 

      // background process: 
      if (command_input->backgroundflag == true) { 

        printf("background pid is %d\n", spawnpid);
        fflush(stdout); 
        spawnpid = waitpid(spawnpid, status, WNOHANG); 

        int i = 0; 
        while(background_processes[i] != NULL) { 
          i++; 
        }

        background_processes[i] = spawnpid; 

      }
      
      // foreground process
      else { 

        spawnpid = waitpid(spawnpid, status, 0); 

        if (WTERMSIG(*status)) { 

          printf("terminated by singal %d\n", WTERMSIG(*status)); 
          fflush(stdout); 
          
        }

        sigint_action->sa_handler = SIG_IGN; 
        sigaction(SIGINT, sigint_action, NULL);

      }

      while ((spawnpid = waitpid(-1, status, WNOHANG)) > 0) { 

        if (WIFEXITED(*status)) { 

          printf("Background pid %d is done: exit value %d\n", spawnpid, WEXITSTATUS(*status));
          fflush(stdout); 

        } else { 

          printf("Background pid %d is done: terminated by signal %d\n", spawnpid, WTERMSIG(*status)); 
          fflush(stdout); 

        }

        for (int i = 0; i < 200; i++) { 
          if (background_processes[i] == spawnpid){ 
            background_processes[i] = NULL; 
          }

        }         
      }
  }
  return; 
}


void signal_handler(int signal_no){ 

  if (BLOCKBACKGROUND == false) {  

    char * message = "Entering foreground only mode (& is now ignored)\n"; 
    write(STDOUT_FILENO, message, strlen(message)); 
    BLOCKBACKGROUND = true;

  } else { 

    char * message = "Exiting foreground only mode\n";
    write(STDERR_FILENO, message, strlen(message)); 
    BLOCKBACKGROUND = false; 
  }

  return; 
}