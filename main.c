#include "smallsh.h"

int main() { 

    // struct sigaction SIGTSTP_action = {0}; 
    // SIGTSTP_action.sa_handler = signal_handler;
    // SIGTSTP_action.sa_flags = SA_RESTART; 
    // sigfillset(&SIGTSTP_action.sa_mask); 
    // sigaction(SIGTSTP, &SIGTSTP_action, NULL); 


    // 1 - Set Signals 
    char * command = read_input();
    bool shell_running = check_exit(command);
    struct command_input_t * command_data;
    int exitcode = 0;
    bool foregroundMode; 

    while (shell_running) { 

        // 4 - Parse arguments 
        command_data = parse_arguments(command); 
        
        // 5 - Execute built in commands
        // 7 - Input/output redirection
        // if(command_data->builtin && !command_data->is_comment) { 

            // execute_built_in_command(command_data, &exitcode, &foregroundMode);


        // } else if (!command_data->is_comment) { 

                    // execution_fork(command_data,
                                   // &exitcode, 
                                   // &foregroundMode);
        // }

        execute_foreground(command_data, &exitcode, &foregroundMode); 

        free(command); 
        destroy_list(command_data->arguments);


        print_background_process(); 


        command = read_input();
        shell_running = check_exit(command);
    } 

    // signal(SIGQUIT, SIG_IGN); 
    // kill(-1*getpid(), SIGQUIT); 

    free(command);
    
    return 0; 

}


