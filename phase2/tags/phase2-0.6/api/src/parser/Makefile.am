## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = debugger
noinst_LTLIBRARIES = libp2parser.la

BUILT_SOURCES = parser.h

AM_YFLAGS = -d
AM_LFLAGS = --header=header.h \
	-p  # Get comments regarding features that lead to performance losses.

libp2parser_la_SOURCES = \
	lexer.l \
	parser.y


## Command-line debugger #######################################################

debugger_LDADD = libp2parser.la

debugger_SOURCES = \
	debugger.c		\
	../Type.c		\
	../Object.c		\
	../Ast.c		\
	../util/Array.c		\
	../util/Hash_Table.c	\
	../util/Hash_Map.c	\
	../util/Name.c		\
	../util/String.c	\
	../util/Term.c
