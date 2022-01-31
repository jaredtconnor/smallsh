target:
	@echo "Compiling..."
	gcc -std=gnu99 -g -Wall -o smallsh main.c

run:
	./smallsh

clear:
	rm -f smallsh 
	rm -f mytestresults

valgrind:
	valgrind -s --leak-check=yes --track-origins=yes --show-reachable=yes --log-file="valgrind.txt" ./smallsh

test: 
	./p3testscript > mytestresults 2>&1
