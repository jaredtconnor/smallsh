#include "smallsh.h"

int main() { 

    char * STARTDIR;
    struct command_input_t * command_data;
    int exitcode = 0;
    bool foregroundMode = false; 

    // 1 - Set Signals 
    struct sigaction SIGTSTP_action = {0}; 
    SIGTSTP_action.sa_handler = signal_handler;
    sigfillset(&SIGTSTP_action.sa_mask); 
    SIGTSTP_action.sa_flags = SA_RESTART; 
    sigaction(SIGTSTP, &SIGTSTP_action, NULL); 

    struct sigaction SIGINT_action = {0};
    SIGINT_action.sa_handler = SIG_IGN; 
    sigfillset(&SIGINT_action.sa_mask); 
    sigaction(SIGINT, &SIGINT_action, NULL);  

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
        execute_foreground(command_data, &exitcode); 

        command = read_input();
        shell_running = check_exit(command);
    } 

    // signal(SIGQUIT, SIG_IGN); 
    // kill(-1*getpid(), SIGQUIT); 

    free(command);
    
    return 0; 

}


