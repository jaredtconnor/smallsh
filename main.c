#include "smallsh.c"


int main() { 

    char * command = read_input();

    printf("This is the received command: %s\n", command);

    return 0;
}
