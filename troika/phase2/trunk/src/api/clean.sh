#!/bin/sh
# Erases generated files.  Don't use this script unless all the necessary tools
# (Lex, Yacc, doxygen, xsltproc, etc.) are available to rebuild them.

make maintainer-clean

rm -rf	configure		\
	autom4te.cache		\
	config aclocal.m4	\
	phase2-*.tar.gz

rm -rf	Makefile.in		\
	include/Makefile.in	\
	src/Makefile.in		\
	src/import/Makefile.in	\
	src/parser/Makefile.in	\
	src/util/Makefile.in	\
	src/xml/Makefile.in

rm -rf	src/import/src/p2-stubs.c	\
	src/import/target

rm -rf	src/parser/header.h

rm -rf doc doxygen.log
