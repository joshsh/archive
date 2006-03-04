################################################################################
## Compile utilities and/or run a test program under Valgrind (memcheck).

CC       = gcc
CFLAGS   = -g -Wall --ansi -D_GNU_SOURCE
VALGRIND = valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes


.PHONY : default
default :  objects


.PHONY : clean
clean :
	-rm *.o debugger


.PHONY : test
test :  debugger
	$(VALGRIND) debugger


debugger :  debugger.c p2_array.o p2_bunch.o p2_hash_table.o p2_set.o p2_term.o p2_collection.o
	$(CC) $(CFLAGS) -o debugger debugger.c \
		p2_array.o p2_bunch.o p2_hash_table.o p2_set.o p2_term.o p2_collection.o


.PHONY : objects
objects :  p2_array.o p2_bunch.o p2_hash_table.o p2_name.o p2_set.o p2_term.o


p2_%.o :  p2_%.c p2_%.h ../p2_flags.h
	$(CC) $(CFLAGS) -c p2_$*.c


p2_collection.o : ../p2_collection.c ../p2_collection.h ../p2_flags.h
	$(CC) $(CFLAGS) -c ../p2_collection.c