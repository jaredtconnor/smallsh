target:
	@echo "Compiling..."
	gcc -std=gnu99 -c -Wall -o arglist.o arglist.c
	gcc -std=gnu99 -c -Wall -o smallsh.o smallsh.c
	gcc -std=gnu99 -c -Wall -o main.o main.c
	gcc -std=gnu99 -Wall -o smallsh *.c
	rm -rf *.o

run:
	./smallsh

clear:
	rm -f smallsh 
	rm -f mytestresults

valgrind:
	valgrind -s --leak-check=yes --track-origins=yes --show-reachable=yes --log-file="valgrind.txt" ./smallsh

test: 
	./p3testscript > mytestresults 2>&1
