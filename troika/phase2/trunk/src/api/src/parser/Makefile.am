## Process this file with automake to produce Makefile.in

bin_PROGRAMS = debugger

BUILT_SOURCES = p2_parser_parser.h
AM_YFLAGS = -d ##-p mfg
AM_LFLAGS = -o $(LEX_OUTPUT_ROOT).c \
	--header=foo.h \
        -p  # Get comments regarding features that lead to performance losses.

debugger_SOURCES = debugger.c \
	p2_parser_lexer.l \
	p2_parser_parser.y \
	../Type.c \
	../Object.c \
	../Ast.c \
	../util/Array.c \
	../util/Hash_Table.c \
	../util/Hash_Map.c \
	../util/Name.c \
	../util/Term.c

INCLUDES = -I../../include


##	p2_parser.c \
##
