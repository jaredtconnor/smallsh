#include "smallsh.h"

int main() { 

    // 1 - Set Signals 
    bool shell_running = true;
    char * command;
    struct command_input_t * command_data;
    bool built_in = false;

    while (shell_running) { 

        // 2 - Check background processes

        // 3 - Read input
        command = read_input();

        // 4 - Parse arguments 
        command_data = parse_arguments(command); 
        
        // 5 - Execute built in commands
        built_in = check_built_in_command(command_data);

        print_args(command_data->arguments);
        if (built_in) { 
            printf("This is a built in command\n");
        }

        // 6 - Execute general commands



        // 7 - Input/output redirection

    

        
    }
    
    destroylist(command_data->arguments);
    return 1; 

}
