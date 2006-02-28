################################################################################
## Makefile for a simple BNF grammar analyzer tool.
##
## Usage: ./bnftool < [valid Bison grammar file]

UTILS    = ../util

CC       = gcc
CFLAGS   = -Wall \
           --ansi -D_GNU_SOURCE \
           -g # temporary...
LEX      = flex \
           -p  # Get comments regarding features that lead to performance losses.
YACC     = bison


.PHONY : default
default :  bnftool


.PHONY : clean
clean :
	-rm *.o *.tab.* lex.yy.c bnftool *.output


.PHONY : distclean
distclean :  clean


################################################################################


bnftool :  lex.yy.c bnftool.tab.o p2_array.o p2_hash_table.o
	$(CC) $(CFLAGS) -o bnftool lex.yy.c bnftool.tab.o p2_array.o p2_hash_table.o


lex.yy.c : bnftool.l ../p2_flags.h
	$(LEX) bnftool.l


bnftool.tab.o : bnftool.tab.c
	$(CC) $(CFLAGS) -c bnftool.tab.c


bnftool.tab.c bnftool.tab.h : bnftool.y ../p2_flags.h
	$(YACC) -dv bnftool.y


################################################################################


p2_array.o : $(UTILS)/p2_array.c
	$(CC) $(CFLAGS) -c $(UTILS)/p2_array.c


p2_hash_table.o : $(UTILS)/p2_hash_table.c
	$(CC) $(CFLAGS) -c $(UTILS)/p2_hash_table.c

