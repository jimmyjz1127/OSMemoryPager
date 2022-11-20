all:
	gcc -std=c99 -o test paging.c test.c

main : main.o paging.o 
	clang -Wall -Wextra main.o paging.o -o main

main.o : main.c paging.h
	clang -Wall -Wextra -c main.c 

paging.o : paging.c paging.h
	clang -Wall -Wextra -c paging.c 

clean:
	rm -f *.o test main
