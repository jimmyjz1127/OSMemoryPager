all:
	gcc -std=c99 -o test paging.c test.c

#For testing
main : main.o paging.o 
	clang -Wall -Wextra main.o paging.o -o main

#For testing
main.o : main.c paging.h
	clang -Wall -Wextra -c main.c 

#For testing
paging.o : paging.c paging.h
	clang -Wall -Wextra -c paging.c 

clean:
	rm -f *.o test main
