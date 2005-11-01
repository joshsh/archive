################################################################################
## Compile the (flex/bison) parser for the Phase2 command-line interface.
##
## Use "make debugger" for a test application.

NAME = p2_parser

CC   = gcc
LEX  = flex
YACC = bison

UTILS = ../util


################################################################################

all: objects


clean:
	-rm lex.yy.c
	-rm *.tab.c *.tab.h *.output p2_term.o # *.vcg


distclean: clean
	-rm *.o $(NAME)-debugger


debugger: objects $(NAME)-debugger.c $(NAME).h p2_term.o
	$(CC) $(NAME)-debugger.c $(NAME)-aux.o lex.yy.o $(NAME).tab.o p2_term.o -o $(NAME)-debugger


objects: lex.yy.o $(NAME).tab.o $(NAME)-aux.o


################################################################################

lex.yy.o : lex.yy.c $(NAME).tab.h $(NAME).h
	$(CC) -c lex.yy.c


$(NAME).tab.o : $(NAME).tab.c
	$(CC) -c $(NAME).tab.c


lex.yy.c : $(NAME).l
	$(LEX) $(NAME).l


$(NAME).tab.c $(NAME).tab.h : $(NAME).y $(NAME).h
	$(YACC) -dv $(NAME).y


################################################################################

$(NAME)-aux.o : $(NAME)-aux.c $(NAME).h
	$(CC) -c $(NAME)-aux.c


p2_term.o : $(UTILS)/p2_term.c
	$(CC) -c $(UTILS)/p2_term.c -o p2_term.o

