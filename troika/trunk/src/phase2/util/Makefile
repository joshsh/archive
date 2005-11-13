################################################################################
## Compile and run a test program under valgrind (memcheck).

NAME    = p2-util-debugger
CC      = gcc
CFLAGS  = -g


all :  exec-valgrind

clean :
	-rm *.o

distclean :  clean
	-rm $(NAME)


exec-valgrind :  $(NAME)
	-valgrind --tool=memcheck --leak-check=full $(NAME)

$(NAME) :  $(NAME).c p2_array.o p2_bunch.o p2_hash_table.o p2_set.o p2_term.o
	gcc $(CFLAGS) -o $(NAME) $(NAME).c \
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

