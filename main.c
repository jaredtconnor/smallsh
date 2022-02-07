#include "smallsh.h"

int main() { 

    struct command_input_t * command_data;
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
    bool shell_running = true;

    while (shell_running) { 

        command_data = parse_arguments(command); 
        command_data->builtin = check_built_in_command(command_data);

        if (command_data->is_comment == true) { 
            continue; 
        } 

        else if (command_data->exit == true) { 

            shell_running = false; 

            for (int i = 0; i < 200; i++) { 

                if(child_processes[i]) { 

                    kill(child_processes[i], SIGTERM); 
                    child_processes[i] = waitpid(child_processes[i], &status, WNOHANG); 
                }
            }
        }

        else if (command_data->builtin == true) { 

            execute_built_in_command(command_data, &status);

        }

        else { 
            execute_command(command_data, &status, child_processes, &SIGINT_action); 
        }

        command = read_input();
    } 

    // signal(SIGQUIT, SIG_IGN); 
    // kill(-1*getpid(), SIGQUIT); 

    free(command);
    
    return 0; 

}


