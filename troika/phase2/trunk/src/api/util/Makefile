################################################################################
## Compile utilities and/or run a test program under Valgrind (memcheck).

INCLUDE = -I../

CFLAGS = -O2 -g --ansi -pedantic -W -Wall -fshort-enums -fno-common
	# -Werror \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

CC      = gcc $(INCLUDE)

VALGRIND = valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes


.PHONY : default
default :  objects


.PHONY : clean
clean :
	-rm *.o debugger


.PHONY : test
test :  debugger
	$(VALGRIND) debugger


objects = Array.o Bunch.o Dictionary.o Graph.o Hash_Table.o Lookup_Table.o Name.o Set.o Term.o ../Collection.o

debugger :  debugger.o $(objects)
	$(CC) -o debugger debugger.o $(objects)


$(objects) : ../defs.h ../settings.h
