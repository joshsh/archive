## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = debugger
noinst_LTLIBRARIES = libp2utils.la

libp2utils_la_SOURCES = \
	Array.c		\
	Bunch.c		\
	Dictionary.c	\
	Hash_Map.c	\
	Hash_Table.c	\
	Name.c		\
	Set.c		\
	String.c	\
	Term.c

## Test ########################################################################

debugger_LDADD = libp2utils.la

debugger_SOURCES = \
	debugger.c