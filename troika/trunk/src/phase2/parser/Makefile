################################################################################
## Compile the (flex/bison) parser for the Phase2 command-line interface.
##
## Use "make debugger" for a test application.

NAME = p2_parser

CC   = gcc
LEX  = flex
YACC = bison


all: objects


clean:
	-rm lex.yy.c
	-rm *.tab.c *.tab.h *.output # *.vcg


distclean: clean
	-rm *.o $(NAME)-debug


debugger: objects $(NAME)-debug.c $(NAME)-macros.h
	$(CC) $(NAME)-debug.c lex.yy.o $(NAME).tab.o -o $(NAME)-debug


objects: lex.yy.o $(NAME).tab.o


lex.yy.o : lex.yy.c $(NAME).tab.h $(NAME)-macros.h
	$(CC) -c lex.yy.c


$(NAME).tab.o : $(NAME).tab.c
	$(CC) -c $(NAME).tab.c


lex.yy.c : $(NAME).l
	$(LEX) $(NAME).l


$(NAME).tab.c $(NAME).tab.h : $(NAME).y $(NAME)-macros.h
	$(YACC) -dv $(NAME).y

