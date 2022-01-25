target:
	@echo "Compiling..."
	gcc -std=gnu99 -g -Wall -o smallsh smallsh.c

run: 
	smallsh	

clear: 
	rm -f smallsh 

debug: 
	valgrind -s --leak-check=yes --track-origins=yes --show-reachable=yes ./smallsh

