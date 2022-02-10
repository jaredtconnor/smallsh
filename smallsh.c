#include "smallsh.h"

bool FOREGROUND_ONLY = false; 
pid_t spawnpid = -5; 

/* #######################################################
 * Function: check_comment 
 * checks if the current token is comment "#"
 * 
 * params: 
 *  1 - char * token
 *
 * output:
 *  1 - true - comment found
 *  2 - false - no comment
 * ######################################################## */
bool check_comment(char * token){ 

  bool result = false;
  char * found = NULL;

  found = strstr(token, "#");

  if (found) { 
    result = true; 
  } 

  return result; 
}

/* #######################################################
 * Function: check_background 
 * checks if the current token contains a background flags
 * 
 * params: 
 *  1 - char * token
 *
 * output:
 *  1 - true - is background
 *  2 - false - is not background
 * ######################################################## */
bool check_background(char * token){ 

  bool result = false;
  char * found = NULL;

  found = strstr(token, "&");

  if (found) { 
    result = true; 
  } 

  return result; 
}

/* #######################################################
 * Function: check_inputredirect 
 * checks if the current command is meant to be redirected
 * 
 * params: 
 *  1 - char * token
 *
 * output:
 *  1 - true - is redirecting input
 *  2 - false - is not redirecting input
 * ######################################################## */
bool check_inputredirect(char * token) { 

  bool result = false;

  if(strcmp(token, "<") == 0){ 
    result = true;
  }

  return result;
  
}

/* #######################################################
 * Function: check_outputredirect 
 * checks if the current command is meant to be redirected
 * 
 * params: 
 *  1 - char * token
 *
 * output:
 *  1 - true - is redirecting output
 *  2 - false - is not redirecting output 
 * ######################################################## */
bool check_outputredirect(char * token) { 

  bool result = false;

  if(strcmp(token, ">") == 0){ 
    result = true;
  }

  return result;
  
}


/* #######################################################
 * Function: check_exit
 * checks if the current command is to exit the shell
 * 
 * params: 
 *  1 - char * token
 *
 * output:
 *  1 - true - is exit
 *  2 - false - is not exit
 * ######################################################## */
bool check_exit(char * token) { 

  bool result = false; 

  if(strcmp(token, "exit") == 0){ 
    result = true;
  }

  return result; 

}

/* #######################################################
 * Function: check_built_in_command
 * checks if the current command is a built in command 
 * 
 * params: 
 *  1 - struct command_input_t * command_input
 *
 * output:
 *  1 - true - is built in
 *  2 - false - is not built in
 * ######################################################## */
bool check_built_in_command(char * token) {

  bool result = false; 

  if (strcmp(token, "cd") == 0 || strcmp(token, "status") == 0){ 
    result = true;
  }

  return result;
}

/* #######################################################
 * Function: check_variable_expansion   
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
bool check_variable_expansion(char * token) { 

  bool result = false;
  char * found = NULL;

  found = strstr(token, "$$");

  if (found) { 
    result = true; 
  } 

  return result;
}


/* #######################################################
 * Function: expand_pid   
 * Searches a command line to confirm if there is '$$' 
 * requesting a variable expansion
 * 
 * params: 
 *  1 - char * command - pointer to command line being passed
 *
 * output: 
 *  1 - int - int indicating result, 0 correct and 1 error
 *
 * Ciation - https://stackoverflow.com/questions/8137244/best-way-to-replace-a-part-of-string-by-another-in-c
 * 
 * ######################################################## */
char * expand_pid(char * command)
{

  static char temp[4096];
  static char buffer[4096];
  char *p;
  char pid_c[PID_LEN]; 
  int pid = getpid(); 
  memset(pid_c, '\0', sizeof(pid_c));
  sprintf(pid_c, "%d", pid);

  strcpy(temp, command);

  if(!(p = strstr(temp, "$$")))
    return temp;

  strncpy(buffer, temp, p-temp); // Copy characters from 'temp' start to 'orig' str
  buffer[p-temp] = '\0';

  sprintf(buffer + (p - temp), "%s%s", pid_c, p + strlen("$$"));
  sprintf(command, "%s", buffer);    

  return command;
}




/* #######################################################
 * Function: read_input 
 * Reads in stdin and returns a char * to be parsed
 * 
 * params: void
 *
 * output: char *
 *
 * ######################################################## */
char * read_input(){ 
 
  printf(": ");   // prints out prompt
  fflush(stdout);
  
  char * command = NULL;
  size_t len = 0; 
  
  getline(&command, &len, stdin);    // gets entire line 

  // CITATION 
  // https://www.freebsd.org/cgi/man.cgi?query=strcspn&sektion=3 
  // used to remove the null/line feed/etc. extra characters
  command[strcspn(command, "\r\n")] = 0;

  return command;

}

/* #######################################################
 * Function: parse_argments   
 * Parses the arguments from a command line and returns a 
 * struct of input that contains the arguments and details 
 * passed
 * 
 * params: 
 *  1 - char * command - pointer to command line being passed
 *
 * output: 
 *  1 - struct command_input_t * 
 *
 * ######################################################## */
struct command_input_t * parse_arguments(char * command) { 

    // allocate sizes for command input
    struct command_input_t * command_data = malloc(sizeof(struct command_input_t)); 
    struct arglist_t * arglist = create_arg_list(); // populate arg list
    command_data->arguments = arglist;  // point head to arglist

    char * token; 
    char * saveptr; 

    // get the first token and save to command_data->command
    token = strtok_r(command, " ", &saveptr);
    command_data->command = calloc(strlen(token) + 1, sizeof(char *)); 
    strcpy(command_data->command, token);

    command_data->builtin = check_built_in_command(token); 

    // add argument for execvp argv[0]
    if (!command_data->builtin) { 
      add_argument(command_data->arguments, token);
    }

    // check required commands before parsing extra args
    command_data->exit = check_exit(token);
    command_data->is_comment = check_comment(token);

    token = strtok_r(NULL, " ", &saveptr);

    while (token != NULL && !command_data->is_comment) { 

      if (check_inputredirect(token)){  // check for input redirect
        command_data->input_redirect = true;
        token = strtok_r(NULL, " ", &saveptr);
        command_data->infile = calloc(strlen(token) + 1, sizeof(char)); 
        strcpy(command_data->infile, token); 
      } 
      
      else if (check_outputredirect(token)) {  // check for output redirect
        command_data->output_redirect = true;
        token = strtok_r(NULL, " ", &saveptr);
        command_data->outfile = calloc(strlen(token) + 1, sizeof(char)); 
        strcpy(command_data->outfile, token); 
      } 
      
      else if (check_background(token) == true){  // check for background flag
        command_data->backgroundflag = true;

        if(!command_data->input_redirect == true) { 
          command_data->infile = calloc(strlen("/dev/null") + 1, sizeof(char));
          strcpy(command_data->infile, "/dev/null"); 
        }

        if(!command_data->output_redirect == true) { 
          command_data->outfile = calloc(strlen("/dev/null") + 1, sizeof(char));
          strcpy(command_data->outfile, "/dev/null"); 
        }
      } 
      
      else {  // else add to the argument list

        command_data->variablexpand = check_variable_expansion(token);

        if (command_data->variablexpand) { 
          token = expand_pid(token);
        }

        add_argument(command_data->arguments, token);
      }

      token = strtok_r(NULL, " ", &saveptr);
    }

    // specify as background flag
    if ((FOREGROUND_ONLY) == true) { 
      command_data->backgroundflag = false; 
    }

    // command_data->emptyargs = checkempty_list(command_data->arguments);
    return command_data;

}

/* #######################################################
 * Function: execute_built_in_commad   
 * Executes a given input if it is either "cd" or "status'"
 * 
 * params: 
 *  1 - struct command_input_t *
 *  2 - int * 
 *
 * output: 
 *  void
 *
 * ######################################################## */
void execute_built_in_command(struct command_input_t * command_input, int * status) { 

  // check if cd
  if (strcmp(command_input->command, "cd") == 0){ 

    if (command_input->arguments->size > 0){ 
      chdir(command_input->arguments->head->value); 
    } 
    
    else { 
      chdir(getenv("HOME"));
    }

  } 
  
  else if (strcmp(command_input->command, "status") == 0) { 

    if(WIFEXITED(*status)) { 
      printf("[STATUS] Exit value of %d\n", WEXITSTATUS(*status));
      fflush(stdout); 

    } 
    
    else if(WIFSIGNALED(*status)) { 

      printf("[STATUS] Signal terminated with value of %d\n", WTERMSIG(*status));
      fflush(stdout); 
    }
  }

  return; 

}

/* #######################################################
 * Function: show_status   
 * Shows a status of exiting child processes
 * 
 * params: 
 *  1 - struct command_input_t *
 *  2 - int *
 *  3 - bool * 
 *
 * output: 
 *  void
 *
 * ######################################################## */
void show_status(struct command_input_t * command_input, int* exit_code, bool * foreground_permitted) { 

  if(*foreground_permitted) { 
    printf("[STATUS] Terminated by signal %d\n", *exit_code);
    fflush(stdout); 
  } 
  
  else { 
    printf("[STATUS] Exit value %d\n", *exit_code);
    fflush(stdout); 
  }

  return;

}


void execute_fork(struct command_input_t, int * processes, ) { 




}


/* #######################################################
 * Function: execute_command   
 * Main execution process to execute non - built in commands
 * 
 * params: 
 *  1 - struct command_input_t *
 *  2 - int *
 *  3 - bool * 
 *
 * output: 
 *  void
 *
 * ######################################################## */
void execute_command(struct command_input_t * command_input, int * status, int * processes, int * process_ctr, struct sigaction * sigint_action) { 


  // get the arguments for the current arg list
  char ** arguments = get_arguments(command_input->arguments); 

  spawnpid = fork();  // fork off child process

  switch (spawnpid) { 
    case -1: 
      perror("[FORK ERROR] fork() failure\n");
      exit(1); 
      break;

    // if fork successful - child process
    case 0: 


      // input redirect
      if (command_input->input_redirect){ 
        int fileinput = open(command_input->infile, O_RDONLY); 

        if (fileinput == -1) {
          perror("[FILE ERROR] Cannot open file\n");
          exit(1); 
        }

        int result = dup2(fileinput, 0); 

        if(result == -1){ 
          perror("[FILE ERROR] Cannot open file\n");
          exit(2); 
        }

        fcntl(fileinput, F_SETFD, FD_CLOEXEC); 
      }

      // output redirect 
      if (command_input->output_redirect == true) { 
        int filewrite = open(command_input->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 

        if (filewrite == -1) { 
          perror("[FILE ERROR] Cannot open file\n");
          exit(1); 
        }

        int result = dup2(filewrite, 1); 

        if(result == -1){ 
          perror("[FILE ERROR] Cannot open file\n");
          exit(2); 
        }

        fcntl(filewrite, F_SETFD, FD_CLOEXEC); 

      }

      // Allowing for SIGINT
      if (command_input->backgroundflag == false) { 
        sigint_action->sa_handler = SIG_DFL; 
        sigaction(SIGINT, sigint_action, NULL); 
      }
      // Execute actual command with execvp
      // CITATION - https://web.stanford.edu/class/archive/cs/cs110/cs110.1196/static/lectures/05-Execvp/lecture-05-understanding-execvp.pdf
      execvp(command_input->command, arguments);
      perror("\n[EXEC ERROR]\n"); 
      exit(2); 
      break;

    // PARENT PROCESS
    default: 

      // current background process
      if (command_input->backgroundflag == true) { 
        printf("[BACKGROUND] PID - %d\n", spawnpid);
        fflush(stdout); 
        spawnpid = waitpid(spawnpid, status, WNOHANG); 

        // add to current process list
        add_background_process(processes, process_ctr, spawnpid);
      }
      
      // current foreground process
      else { 
        spawnpid = waitpid(spawnpid, status, 0); 

        if (WTERMSIG(*status)) { 
          printf("[SINGAL] Current child terminated by singal %d\n", WTERMSIG(*status)); 
          fflush(stdout); 
        }

        sigint_action->sa_handler = SIG_IGN; 
        sigaction(SIGINT, sigint_action, NULL);
      }

      // Waiting for current foreground processes to finish
      while ((spawnpid = waitpid(-1, status, WNOHANG)) > 0) { 

        if (WIFEXITED(*status)) { 
          printf("[BACKGROUND] PID - %d is done | EXIT - %d\n", spawnpid, WEXITSTATUS(*status));
          fflush(stdout); 
        } 
        
        else { 
          printf("[BACKGROUND] PID - %d is done | SIGNAL - %d\n", spawnpid, WEXITSTATUS(*status));
          fflush(stdout); 
        }

        // Clean bg processes
        clean_background_processes(processes, process_ctr, spawnpid);
      }
  }
  return; 
}


void add_background_process(int * processes, int * processes_ctr, int child_pid){ 

  processes[*processes_ctr] = child_pid; 
  processes_ctr++; 

  return; 

}


void clean_background_processes(int * processes, int * process_ctr, int child_pid){ 

  for (int i = 0; i < *process_ctr; i++){ 

    if (processes[i] = child_pid){ 
      processes[i] = NULL;
    }
  }

  return; 
}

void print_background_processes(int * processes, int * process_ctr, int * status) { 




}




/* #######################################################
 * Function: singal_handler   
 * 
 * params: 
 *  1 - int
 *
 * output: 
 *  void
 *
 * ######################################################## */
void exec_mode_signal_handler(int signal_no){ 

  if (FOREGROUND_ONLY == false) {  
    char * message = "\n----FOREGROUND MODE----\n"; 
    write(STDOUT_FILENO, message, strlen(message)); 
    FOREGROUND_ONLY = true;
  } 
  
  else { 
    char * message = "\n----ALLOWING BACKGROUND PROCESSES----\n";
    write(STDERR_FILENO, message, strlen(message)); 
    FOREGROUND_ONLY = false; 
  }

  return; 
}

/* #######################################################
 * Function: kill_shell   
 * iterates through existing background pids and kills them
 * 
 * params: 
 *  1 - int * 
 *  2 - int *
 *
 * output: 
 *  void
 *
 * ######################################################## */
void kill_shell(int * processes, int * status) { 

    for (int i = 0; i < NUM_PROCESSES; i++) { 
        if(processes[i]) { 
            kill(processes[i], SIGTERM); 
            processes[i] = waitpid(processes[i], status, WNOHANG); 
        }
    }

}