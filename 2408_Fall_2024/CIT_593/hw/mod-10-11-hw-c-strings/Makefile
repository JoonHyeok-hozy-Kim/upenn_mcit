all: my_string.o program1 program2 program3 program4

my_string.o: my_string.c my_string.h
	clang -c my_string.c

program1: program1.c my_string.o my_string.h
	clang -Wall -g -o program1 program1.c my_string.o

program2: program2.c my_string.o my_string.h
	clang -Wall -g -o program2 program2.c my_string.o

program3: program3.c
	clang -Wall -g -o program3 program3.c

program4: program4.c my_string.o my_string.h
	clang -Wall -g -o program4 program4.c my_string.o

clean:
	rm -rf my_string.o

clobber: clean
	rm -rf program1 program2 program3 program4