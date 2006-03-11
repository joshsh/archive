################################################################################
## Compile utilities and/or run a test program under Valgrind (memcheck).

CC       = gcc

CFLAGS   = --ansi -pedantic \
	-W -Wall -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	-fshort-enums -fno-common \
	-g # -D_GNU_SOURCE

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
objects :  p2_array.o p2_bunch.o p2_dictionary.o p2_graph.o p2_hash_table.o p2_lookup_table.o p2_name.o p2_set.o p2_term.o


p2_%.o :  p2_%.c p2_%.h ../flags.h
	$(CC) $(CFLAGS) -c p2_$*.c


p2_collection.o : ../p2_collection.c ../p2_collection.h ../flags.h
	$(CC) $(CFLAGS) -c ../p2_collection.c

