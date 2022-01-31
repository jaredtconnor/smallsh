#include "smallsh.c"


int main() { 

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
        if (command_data->is_comment) { 
            printf("Comment command\n");
        } else { 
            printf("Current command: %s\n", command_data->command);
        }

        // 6 - Execute general commands



        // 7 - Input/output redirection

    

        
    }
    



}
