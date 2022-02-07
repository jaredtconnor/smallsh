#include "smallsh.h"

int main() { 

    struct command_input_t * command_data;
    bool foregroundMode = false; 
    int child_processes[200];
    int status = 0;

    // 1 - Set Signals 
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

    char * command = read_input();
    bool shell_running = check_exit(command);

    while (shell_running) { 

        command_data = parse_arguments(command); 
        
        // if(command_data->builtin && !command_data->is_comment) { 

            // execute_built_in_command(command_data, &exitcode, &foregroundMode);


        // } else if (!command_data->is_comment) { 

                    // execute_foreground(command_data, &exitcode); 

                    // //execution_fork(command_data,
                                   // //&exitcode, 
                                   // //&foregroundMode);
        // }

        // free(command); 
        // destroy_list(command_data->arguments);

        // print_background_process(); 
        execute_foreground(command_data, &status, child_processes, &SIGINT_action); 

        command = read_input();
        shell_running = check_exit(command);
    } 

    // signal(SIGQUIT, SIG_IGN); 
    // kill(-1*getpid(), SIGQUIT); 

    free(command);
    
    return 0; 

}


