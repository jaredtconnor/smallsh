target:
	@echo "Compiling..."
	gcc -std=gnu99 -g -Wall -o smallsh main.c

run:
	./smallsh

clear:
	rm -f smallsh 

valgrind:
	valgrind -s --leak-check=yes --track-origins=yes --show-reachable=yes --log-file="valgrind.txt" ./smallsh

