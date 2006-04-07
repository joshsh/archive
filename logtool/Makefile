################################################################################
## Makefile for a simple BNF grammar analyzer tool.
##
## Usage: ./logtool < <valid Bison grammar file>

UTILS    = ../util

export CFLAGS = -O2 -g --ansi -pedantic -W -Wall -fshort-enums -fno-common
	# -Werror \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

INCLUDE  = -I../

CC       = gcc $(INCLUDE)

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


utils = $(UTILS)/Array.o $(UTILS)/Dictionary.o $(UTILS)/Hash_Table.o


logtool :  lex.yy.o logtool.tab.o $(utils) ../Type.o
	$(CC) $(CFLAGS) -o logtool lex.yy.o logtool.tab.o $(utils) ../Type.o


lex.yy.o : logtool.tab.h

lex.yy.c : logtool.l ../defs.h ../settings.h
	$(LEX) logtool.l


logtool.tab.c logtool.tab.h : logtool.y ../defs.h ../settings.h
	$(YACC) -dv logtool.y


################################################################################


$(utils) ../Type.o : ../defs.h ../settings.h
