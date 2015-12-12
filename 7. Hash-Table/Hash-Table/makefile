main: core.o binarytree.o hashmap.o main.o
	gcc -o main.amin core.o binarytree.o hashmap.o main.o
		
core.o: core.c core.h
	gcc -std=c99 -ggdb -Wall -pedantic -c core.c

binarytree.o: binarytree.c binarytree.h
	gcc -std=c99 -ggdb -Wall -pedantic -c binarytree.c

hashmap.o: hashmap.c hashmap.h
	gcc -std=c99 -ggdb -Wall -pedantic -c hashmap.c

main.o: main.c core.h binarytree.h hashmap.h
	gcc -std=c99 -ggdb -Wall -pedantic -c main.c

clean:
	-rm *.o *.amin *.gv 