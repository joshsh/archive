## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = parser-test
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


## Command-line test #######################################################

parser_test_LDADD = libp2parser.la

parser_test_SOURCES = \
	parser-test.c			\
	../Ast.c			\
	../collection/Array.c		\
	../collection/Hash_Table.c	\
	../collection/Hash_Map.c	\
	../collection/Name.c		\
	../collection/String.c		\
	../collection/Term.c		\
	../object/Object.c		\
	../object/association.c		\
	../object/methods.c		\
	../object/traversal.c		\
	../type/Type.c
