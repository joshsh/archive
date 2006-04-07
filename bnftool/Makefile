################################################################################
## Makefile for a simple BNF grammar analyzer tool.
##
## Usage: ./bnftool < <valid Bison grammar file>

UTILS    = ../util

INCLUDE  = -I../troika/phase2/trunk/src/api
LIBS     = -lphase2 `xml2-config --libs`

CC       = gcc $(INCLUDE)

export CFLAGS = -O2 -g --ansi -W -Wall -fshort-enums -fno-common
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


bnftool :  lex.yy.o bnftool.tab.o
	$(CC) $(CFLAGS) $(LIBS) -o bnftool lex.yy.o bnftool.tab.o


lex.yy.o : bnftool.tab.h


lex.yy.c : bnftool.l
	$(LEX) bnftool.l


bnftool.tab.c bnftool.tab.h : bnftool.y
	$(YACC) -dv bnftool.y

