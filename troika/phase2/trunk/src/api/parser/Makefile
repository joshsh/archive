################################################################################
## Compile the (flex/bison) parser for the Phase2 command-line interface.
## Completely ANSI-compliant.
##
## Use "make debugger" for a test application.

NAME     = p2_parser
UTILS    = ../util

INCLUDE  = -I../

CFLAGS = -O2 -g --ansi -pedantic -W -Wall -fshort-enums -fno-common
	# -Werror \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

CC       = gcc $(INCLUDE)

LEX      = flex --header=foo.h \
           -p  # Get comments regarding features that lead to performance losses.

YACC     = bison

VALGRIND = valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes


################################################################################

.PHONY : default
default: objects


.PHONY : clean
clean:
	-rm lex.yy.c
	-rm *.o debugger *.tab.c *.tab.h *.output *.o # *.vcg


.PHONY : test
test :  debugger
	$(VALGRIND) ./debugger < debugger.c
##	$(VALGRIND) echo "\exit;" | ./debugger


.PHONY : valgrind-session
valgrind-session :  $(DEBUGGER)
	$(VALGRIND) debugger


utils = $(UTILS)/Array.o $(UTILS)/Name.o $(UTILS)/Term.o

objects = debugger.o ../Type.o p2_ast.o $(NAME).tab.o


debugger :  $(objects) $(utils)
	$(CC) -o debugger $(objects) $(utils) lex.yy.o 

$(objects) $(utils) : ../defs.h ../settings.h


################################################################################

.PHONY : objects
objects: lex.yy.o $(NAME).tab.o p2_ast.o ##$(NAME)-aux.o


lex.yy.o : lex.yy.c $(NAME).tab.h
	$(CC) $(CFLAGS) -D_POSIX_SOURCE -c lex.yy.c


lex.yy.c : $(NAME).l p2_parser.h ../defs.h ../settings.h
	$(LEX) $(NAME).l


$(NAME).tab.c $(NAME).tab.h : $(NAME).y p2_parser.h ../defs.h ../settings.h
	$(YACC) -dv $(NAME).y
