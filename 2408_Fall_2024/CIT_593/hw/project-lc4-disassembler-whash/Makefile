# you must implement this makefile to create the following 8 targets for part 2 of the assignment:
# all, clean, clobber
# lc4
# lc4_dissassembler.o, lc4_hash.o, lc4_loader.o, lc4_memory.o

all: lc4_memory.o lc4_hash.o lc4_loader.o lc4_disassembler.o lc4

lc4_memory.o: lc4_memory.h lc4_memory.c
	clang -c lc4_memory.c

clean_lc4_memory.o:
	rm -rf lc4_memory.o

lc4_hash.o: lc4_memory.o lc4_hash.h lc4_hash.c
	clang -c lc4_hash.c

clean_lc4_hash.o:
	rm -rf lc4_hash.o

lc4_loader.o: lc4_memory.o lc4_hash.o lc4_loader.h lc4_loader.c
	clang -c lc4_loader.c

clean_lc4_loader.o:
	rm -rf lc4_loader.o

lc4_disassembler.o: lc4_hash.o lc4_memory.o lc4_disassembler.h lc4_disassembler.c
	clang -c -g lc4_disassembler.c

clean_lc4_disassembler.o:
	rm -rf lc4_disassembler.o

lc4: lc4.c lc4_memory.o lc4_hash.o lc4_loader.o lc4_disassembler.o
	clang -Wall -g lc4.c lc4_memory.o lc4_hash.o lc4_loader.o lc4_disassembler.o -o lc4

clean_lc4:
	rm -rf lc4

clobber: clean_lc4_memory.o clean_lc4_hash.o clean_lc4_loader.o clean_lc4_disassembler.o clean_lc4