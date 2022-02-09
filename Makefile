target:
	@echo "Compiling..."
	gcc -std=gnu99 -c -Wall -o arglist.o arglist.c
	gcc -std=gnu99 -c -Wall -o smallsh.o smallsh.c
	gcc -std=gnu99 -c -Wall -o main.o main.c
	gcc -std=gnu99 -g -Wall -o smallsh *.c
	rm -rf *.o

run:
	./smallsh

clean:
	rm -f smallsh 
	rm -f mytestresults
	rm -rf *.o
	rm junk
	rm test*

valgrind:
	valgrind -s --leak-check=yes --track-origins=yes --show-reachable=yes --log-file="valgrind.txt" ./smallsh

test: 
	./p3testscript > mytestresults 2>&1 

ps: 
	ps -o ppid,pid,pgid,sid,euser,stat,%cpu,rss,args | head -n 1; ps -eH -o ppid,pid,pgid,sid,euser,stat,%cpu,rss,args | grep jaredconnor	
