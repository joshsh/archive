## Process this file with automake to produce Makefile.in

CXX_IN  = ./src
OUT     = ./target
XML_OUT = ./$(OUT)/xml
OBJ_OUT = ./$(OUT)/obj


BUILT_SOURCES = p2-stubs.c src/c89/operators.h

## These are included in the distribution package for completeness, but are not
## required in order to build phase2 from it.
EXTRA_DIST = p2-stubs.xslt p2-import.cfg src/c89/operators.h.templ

## ~
INCLUDES := $(INCLUDES) -I../../src

noinst_LTLIBRARIES = libp2prims.la

libp2prims_la_SOURCES = \
	p2-stubs.c		\
	src/char.c		\
	src/combinator-impl.c	\
	src/cstring.c		\
	src/double.c		\
	src/int.c		\
	src/xpm.c		\
	src/c89/operators.c

noinst_HEADERS = \
	src/char.h		\
	src/combinator-impl.h	\
	src/int.h		\
	src/double.h		\
	src/cstring.h		\
	src/xpm.h		\
	src/Set-operations.h	\
	src/c_std_lib/setjmp.h	\
	src/c_std_lib/ctype.h	\
	src/c_std_lib/float.h	\
	src/c_std_lib/stdio.h	\
	src/c_std_lib/assert.h	\
	src/c_std_lib/limits.h	\
	src/c_std_lib/time.h	\
	src/c_std_lib/locale.h	\
	src/c_std_lib/errno.h	\
	src/c_std_lib/stdarg.h	\
	src/c_std_lib/stdlib.h	\
	src/c_std_lib/string.h	\
	src/c_std_lib/signal.h	\
	src/c_std_lib/math.h	\
	src/c_std_lib/stddef.h

## Note: avoids intermediate targets which would cause the generated Makefile to
## try to rebuild p2-stubs.c (this requires doxygen and xsltproc).
p2-stubs.c :  $(noinst_HEADERS) $(CXX_IN)/c89/operators.h
	doxygen p2-import.cfg
	xsltproc $(XML_OUT)/combine.xslt $(XML_OUT)/index.xml > $(XML_OUT)/p2-import.xml
	xsltproc p2-stubs.xslt $(XML_OUT)/p2-import.xml > p2-stubs.c

$(CXX_IN)/c89/operators.h :
	cp $(CXX_IN)/c89/operators.h.templ temp.h
	$(CC) temp.h -E -o $(CXX_IN)/c89/operators.h
	rm -f temp.h
