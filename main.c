#include "smallsh.h"














int main() { 

    struct sigaction SIGTSTP_action = {0}; 
    struct sigaction SIGINT_action = {0}; 

    SIGTSTP_action.sa_handler = signal_handler;
    SIGTSTP_action.sa_flags = SA_RESTART; 
    sigfillset(&SIGTSTP_action.sa_mask); 
    sigaction(SIGTSTP, &SIGTSTP_action, NULL); 


    // 1 - Set Signals 
    bool shell_running = true;
    char * command;
    struct command_input_t * command_data;

    while (shell_running) { 

        // 2 - Check background processes
         

        // 3 - Read input
        command = read_input();

        // 4 - Parse arguments 
        command_data = parse_arguments(command); 
        
        // 5 - Execute built in commands
        if(command_data->builtin) { 
            execute_built_in_command(command_data);
        }



        // 6 - Execute general commands



        // 7 - Input/output redirection

    

        
    }
    
    destroy_list(command_data->arguments);
    exit(0);
    return 1; 

}


