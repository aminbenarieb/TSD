main: core.o main.o
	gcc -o main.amin core.o main.o
		
core.o: core.c core.h
	gcc -std=c99 -ggdb -Wall -pedantic -c core.c


main.o: main.c core.h 
	gcc -std=c99 -ggdb -Wall -pedantic -c main.c

clean:
	-rm *.o *.amin *.gv 