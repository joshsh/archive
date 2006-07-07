## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = debugger
noinst_LTLIBRARIES = libp2parser.la

LIBS := $(LIBS) -lncurses

BUILT_SOURCES = parser.h

AM_YFLAGS = -d
AM_LFLAGS = --header=header.h \
	-p  # Get comments regarding features that lead to performance losses.

libp2parser_la_SOURCES = \
	lexer.l		\
	parser.y

EXTRA_DIST = README


## Command-line debugger #######################################################

debugger_LDADD = libp2parser.la

debugger_SOURCES = \
	debugger.c		\
	../Type.c		\
	../object/Object.c	\
	../object/association.c	\
	../object/methods.c	\
	../object/traversal.c	\
	../Ast.c		\
	../collection/Array.c		\
	../collection/Hash_Table.c	\
	../collection/Hash_Map.c	\
	../collection/Name.c		\
	../collection/String.c	\
	../collection/Term.c
