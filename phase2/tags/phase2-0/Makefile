# Makefile for the "Phase2" programming environment.
# author: Joshua Shinavier
# last edited: 4/20/2005
#
# Use "make", "make clean"



parser_objects = \
	lex.yy.o \
	p2_parse.tab.o \

util_objects = \
	array.o \
	hash_table.o \

util_dir = include/util
parser_dir = .



#############################################################
# Build the command line interface.

phase2 : $(parser_dir)/p2_itf.c $(parser_objects) $(util_objects)
	gcc -o phase2 $(parser_dir)/p2_itf.c $(parser_objects) $(util_objects)



#############################################################
# Compile helper utilities.

array.o : $(util_dir)/array.c
	gcc -c $(util_dir)/array.c

hash_table.o  : $(util_dir)/hash_table.c
	gcc -c $(util_dir)/hash_table.c



#############################################################
# Compile the (flex/bison) parser.

flex_files = lex.yy.c
bison_files = p2_parse.tab.c p2_parse.tab.h p2_parse.output #p2_parse.vcg

lex.yy.o : lex.yy.c p2_parse.tab.h p2_debug.h
	gcc -c lex.yy.c

p2_parse.tab.o : p2_parse.tab.c
	gcc -c p2_parse.tab.c

lex.yy.c : p2_parse.l
	flex p2_parse.l

p2_parse.tab.c p2_parse.tab.h : p2_parse.y p2_debug.h
	bison -dv p2_parse.y



#############################################################
# "make clean" (Note: doesn't remove the executable)

.PHONY : clean
clean : 
	-rm \
	$(util_objects) \
	$(parser_objects) \
	$(flex_files) \
	$(bison_files) \




