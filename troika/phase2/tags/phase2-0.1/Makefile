# Makefile for the "Phase2" programming environment.
# author: Joshua Shinavier
# last edited: 4/23/2005
#
# Use "make", "make clean"



CC=gcc

UTIL_PATH = ./include/util
PARSER_PATH = .
LIBXML2_PATH = /usr/include/libxml2

PARSER_OBJECTS = \
	lex.yy.o \
	p2_parse.tab.o \

UTIL_OBJECTS = \
	array.o \
	hash_table.o \



#############################################################
# Build the command line interface.

phase2 : $(PARSER_PATH)/p2_itf.c $(PARSER_OBJECTS) $(UTIL_OBJECTS) xml.o
	$(CC) -o phase2 $(PARSER_PATH)/p2_itf.c $(PARSER_OBJECTS) $(UTIL_OBJECTS)



#############################################################
# Compile "xml.c".

xml.o : xml.c
	$(CC) -c xml.c -I $(LIBXML2_PATH)



#############################################################
# Compile helper utilities.

array.o : $(UTIL_PATH)/array.c
	$(CC) -c $(UTIL_PATH)/array.c

hash_table.o  : $(UTIL_PATH)/hash_table.c
	$(CC) -c $(UTIL_PATH)/hash_table.c



#############################################################
# Compile the (flex/bison) parser.

FLEX_FILES = lex.yy.c
BISON_FILES = p2_parse.tab.c p2_parse.tab.h p2_parse.output #p2_parse.vcg

lex.yy.o : lex.yy.c p2_parse.tab.h p2_debug.h
	$(CC) -c lex.yy.c

p2_parse.tab.o : p2_parse.tab.c
	$(CC) -c p2_parse.tab.c

lex.yy.c : p2_parse.l
	flex p2_parse.l

p2_parse.tab.c p2_parse.tab.h : p2_parse.y p2_debug.h
	bison -dv p2_parse.y



#############################################################
# "make clean" (Note: doesn't remove the executable)

.PHONY : clean
clean : 
	-rm \
	$(UTIL_OBJECTS) \
	$(PARSER_OBJECTS) \
	$(FLEX_FILES) \
	$(BISON_FILES) \




