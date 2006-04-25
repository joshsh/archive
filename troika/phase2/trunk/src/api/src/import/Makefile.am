## Process this file with automake to produce Makefile.in

BUILT_SOURCES = src/p2-stubs.c

INCLUDES = -I../../include -I../../src

noinst_LTLIBRARIES = libp2prims.la

libp2prims_la_SOURCES = \
	src/p2-stubs.c \
	src/char.c	\
	src/combinator-impl.c	\
	src/cstring.c	\
	src/double.c	\
	src/int.c	\
	src/toys.c

CXX_IN  = ./src
OUT     = ./target
XML_OUT = ./$(OUT)/xml
OBJ_OUT = ./$(OUT)/obj

src/p2-stubs.c :  $(XML_OUT)/p2-import.xml p2-stubs.xslt
	xsltproc p2-stubs.xslt $(XML_OUT)/p2-import.xml > $(CXX_IN)/p2-stubs.c

other_dependencies = $(CXX_IN)/Set-operations.h

$(XML_OUT)/p2-import.xml :  $(other_dependencies) ##$(objects)
	doxygen p2-import.cfg
	xsltproc $(XML_OUT)/combine.xslt $(XML_OUT)/index.xml > $(XML_OUT)/p2-import.xml
