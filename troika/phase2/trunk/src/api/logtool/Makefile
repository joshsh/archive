################################################################################
## Makefile for a simple BNF grammar analyzer tool.
##
## Usage: ./logtool < <valid Bison grammar file>

UTILS    = ../util

CFLAGS   = -Wall \
           --ansi -D_GNU_SOURCE \
           -g # temporary...

INCLUDE  = -I../

CC       = gcc $(CFLAGS) $(INCLUDE)

LEX      = flex \
           -p  # Get comments regarding features that lead to performance losses.
YACC     = bison


.PHONY : default
default :  logtool


.PHONY : install
install :  logtool
	-rm -f /usr/bin/logtool
	cp logtool /usr/bin


.PHONY : clean
clean :
	-rm *.o *.tab.* lex.yy.c logtool *.output


################################################################################


logtool :  lex.yy.c logtool.tab.o p2_array.o p2_hash_table.o p2_dictionary.o p2_type.o
	$(CC) $(CFLAGS) -o logtool lex.yy.c logtool.tab.o p2_array.o p2_hash_table.o p2_dictionary.o p2_type.o


lex.yy.c : logtool.l ../settings.h
	$(LEX) logtool.l


logtool.tab.o : logtool.tab.c
	$(CC) $(CFLAGS) -c logtool.tab.c


logtool.tab.c logtool.tab.h : logtool.y ../settings.h
	$(YACC) -dv logtool.y


################################################################################


p2_type.o : ../p2_type.c
	$(CC) $(CFLAGS) -c ../p2_type.c

p2_array.o : $(UTILS)/p2_array.c
	$(CC) $(CFLAGS) -c $(UTILS)/p2_array.c


p2_dictionary.o : $(UTILS)/p2_dictionary.c
	$(CC) $(CFLAGS) -c $(UTILS)/p2_dictionary.c


p2_hash_table.o : $(UTILS)/p2_hash_table.c
	$(CC) $(CFLAGS) -c $(UTILS)/p2_hash_table.c

