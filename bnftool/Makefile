################################################################################
## Makefile for a simple BNF grammar analyzer tool.
##
## Usage: ./bnftool < <valid Bison grammar file>

UTILS    = ../util

INCLUDE  = -I../

CC       = gcc $(INCLUDE)

export CFLAGS = -O2 -g --ansi -pedantic -W -Wall -fshort-enums -fno-common
	# -Werror \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

LEX      = flex \
           -p  # Get comments regarding features that lead to performance losses.
YACC     = bison


.PHONY : default
default :  bnftool


.PHONY : install
install :  bnftool
	-rm -f /usr/bin/bnftool
	cp bnftool /usr/bin


.PHONY : clean
clean :
	-rm *.o *.tab.* lex.yy.c bnftool *.output


################################################################################


utils = $(UTILS)/Array.o $(UTILS)/Dictionary.o $(UTILS)/Hash_Table.o

bnftool :  lex.yy.o bnftool.tab.o $(utils) ../Type.o
	$(CC) $(CFLAGS) -o bnftool lex.yy.o bnftool.tab.o $(utils) ../Type.o


lex.yy.o : bnftool.tab.h


lex.yy.c : bnftool.l ../defs.h ../settings.h
	$(LEX) bnftool.l


bnftool.tab.c bnftool.tab.h : bnftool.y ../defs.h ../settings.h
	$(YACC) -dv bnftool.y


################################################################################


$(utils) ../Type.o : ../defs.h ../settings.h


