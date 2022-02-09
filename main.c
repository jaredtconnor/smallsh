#include "smallsh.h"

int main() { 

    // driver variables
    struct command_input_t * command_data;
    int processes[NUM_PROCESSES] = {0};
    int status = 0;

    // setup of signals
    struct sigaction SIGINT_action = {0};
    SIGINT_action.sa_handler = SIG_IGN; 
    sigfillset(&SIGINT_action.sa_mask); 
    SIGINT_action.sa_flags = 0; 
    sigaction(SIGINT, &SIGINT_action, NULL);  

    struct sigaction SIGTSTP_action = {0}; 
    SIGTSTP_action.sa_handler = signal_handler;
    sigfillset(&SIGTSTP_action.sa_mask); 
    SIGTSTP_action.sa_flags = 0;
    sigaction(SIGTSTP, &SIGTSTP_action, NULL); 

    // read in commands
    char * command = read_input();
    bool shell_running = true;

    // main shell process
    while (shell_running) { 

        // parse arguments
        command_data = parse_arguments(command); 

        // skip if comment
        if (command_data->is_comment == true) { 
            command = read_input();
            continue;
        } 

        // exit shell and kill child processes
        else if (command_data->exit == true) { 

            shell_running = false; 
            kill_shell(processes, &status);

        }

        // execute built in commands - TODO
        else if (command_data->builtin == true) { 

            execute_built_in_command(command_data, &status);

        }

        // execute other commands
        else { 

            execute_command(command_data, &status, processes, &SIGINT_action); 

        }

        // re-read command input
        if (shell_running) { 
            command = read_input();
        }

    } 
    
    //free(command_data);
    destroy_list(command_data->arguments);
    free(command_data);
    
    return 0; 

}


