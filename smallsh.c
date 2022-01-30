#include "smallsh.h"


/* #######################################################
 * Function:   
 *
 * Reads in stdin and returns a char * to be parsed
 * 
 * params: void
 *
 * output: char *
 *
 * ######################################################## */
int variable_expansion(char * command) { 

  char * ret = strstr(command, "$$"); 

  if (ret) { 
    char pid[PID_LEN]; 
    sprintf(pid, "%d", getpid());
    printf("\nVariable expansion found - %s\n", pid);
    // replace pid

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



