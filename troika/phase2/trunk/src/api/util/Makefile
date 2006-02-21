################################################################################
## Compile utilities and run a test program under Valgrind (memcheck).

CC       = gcc
CFLAGS   = -g -Wall --ansi -D_GNU_SOURCE
VALGRIND = valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes


.PHONY : default
default :  test


.PHONY : clean
clean :
	-rm *.o


.PHONY : distclean
distclean :  clean
	-rm debugger


.PHONY : test
test :  debugger
	$(VALGRIND) debugger


debugger :  debugger.c p2_array.o p2_bunch.o p2_hash_table.o p2_set.o p2_term.o
	gcc $(CFLAGS) -o debugger debugger.c \
		p2_array.o p2_bunch.o p2_hash_table.o p2_set.o p2_term.o


p2_array.o :  p2_array.c p2_array.h
	gcc -c $(CFLAGS) p2_array.c


p2_bunch.o :  p2_bunch.c p2_bunch.h
	gcc -c $(CFLAGS) p2_bunch.c


p2_hash_table.o :  p2_hash_table.c p2_hash_table.h
	gcc -c $(CFLAGS) p2_hash_table.c


p2_set.o :  p2_set.c p2_set.h
	gcc -c $(CFLAGS) p2_set.c


p2_term.o :  p2_term.c p2_term.h
	gcc -c $(CFLAGS) p2_term.c

