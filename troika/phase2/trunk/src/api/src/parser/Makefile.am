## Process this file with automake to produce Makefile.in

bin_PROGRAMS = debugger

BUILT_SOURCES = parser.h
AM_YFLAGS = -d
AM_LFLAGS = \
	--header=foo.h \
        -p  # Get comments regarding features that lead to performance losses.

debugger_SOURCES = debugger.c \
	lexer.l \
	parser.y \
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
