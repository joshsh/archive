################################################################################
## Compile the (flex/bison) parser for the Phase2 command-line interface.
## Completely ANSI-compliant.
##
## Use "make debugger" for a test application.

NAME     = p2_parser
UTILS    = ../util

CC       = gcc
CFLAGS   = -Wall \
           --ansi \
           -g # temporary...
LEX      = flex \
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


debugger :  objects debugger.c p2_array.o p2_term.o p2_parser.h p2_name.o ../p2_flags.h
	$(CC) $(CFLAGS) -o debugger debugger.c p2_ast.o lex.yy.o $(NAME).tab.o p2_name.o p2_array.o p2_term.o


################################################################################

.PHONY : objects
objects: lex.yy.o $(NAME).tab.o p2_ast.o ##$(NAME)-aux.o


lex.yy.o : lex.yy.c $(NAME).tab.h
	$(CC) $(CFLAGS) -c lex.yy.c


$(NAME).tab.o : $(NAME).tab.c
	$(CC) $(CFLAGS) -c $(NAME).tab.c


lex.yy.c : $(NAME).l p2_parser.h ../p2_flags.h
	$(LEX) $(NAME).l


$(NAME).tab.c $(NAME).tab.h : $(NAME).y p2_parser.h ../p2_flags.h
	$(YACC) -dv $(NAME).y


################################################################################

##$(NAME)-aux.o : $(NAME)-aux.c $(NAME)-aux.h
##	$(CC) $(CFLAGS) -c $(NAME)-aux.c


p2_ast.o : p2_ast.c ../p2_flags.h
	$(CC) $(CFLAGS) -c p2_ast.c


p2_name.o : $(UTILS)/p2_name.c ../p2_flags.h
	$(CC) $(CFLAGS) -c $(UTILS)/p2_name.c


p2_array.o : $(UTILS)/p2_array.c
	$(CC) $(CFLAGS) -c $(UTILS)/p2_array.c -o p2_array.o


p2_term.o : $(UTILS)/p2_term.c
	$(CC) $(CFLAGS) -c $(UTILS)/p2_term.c -o p2_term.o

