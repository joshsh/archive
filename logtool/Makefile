################################################################################
## Makefile for a simple BNF grammar analyzer tool.
##
## Usage: ./logtool < <valid Bison grammar file>

UTILS    = ../util

export CFLAGS = -O2 -g --ansi -W -Wall -fshort-enums -fno-common
	# -Werror \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

INCLUDE  = -I../troika/phase2/trunk/src/api
LIBS     = -lphase2 `xml2-config --libs`

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


logtool :  lex.yy.o logtool.tab.o
	$(CC) $(CFLAGS) $(LIBS) -o logtool lex.yy.o logtool.tab.o


lex.yy.o : logtool.tab.h

lex.yy.c : logtool.l
	$(LEX) logtool.l


logtool.tab.c logtool.tab.h : logtool.y
	$(YACC) -dv logtool.y

