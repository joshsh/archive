## Process this file with automake to produce Makefile.in

CXX_IN  = ./src
OUT     = ./target
XML_OUT = $(OUT)/xml
OBJ_OUT = $(OUT)/obj


BUILT_SOURCES = $(OUT)/p2-stubs.c $(CXX_IN)/c89/operators.h

## These are included in the distribution package for completeness, but are not
## required in order to build phase2 from it.
EXTRA_DIST = p2-stubs.xslt p2-import.cfg src/c89/operators.h.templ

## ~
INCLUDES := $(INCLUDES) -I../../src

noinst_LTLIBRARIES = libp2prims.la

libp2prims_la_SOURCES = \
	$(OUT)/p2-stubs.c		\
	$(CXX_IN)/combinator-impl.c	\
	$(CXX_IN)/xpm.c			\
	$(CXX_IN)/c89/operators.c	\
	$(CXX_IN)/c89/types/char.c	\
	$(CXX_IN)/c89/types/cstring.c	\
	$(CXX_IN)/c89/types/double.c	\
	$(CXX_IN)/c89/types/int.c

noinst_HEADERS = \
	$(CXX_IN)/combinator-impl.h	\
	$(CXX_IN)/Set-operations.h	\
	$(CXX_IN)/xpm.h			\
	$(CXX_IN)/c89/std_lib/setjmp.h	\
	$(CXX_IN)/c89/std_lib/ctype.h	\
	$(CXX_IN)/c89/std_lib/float.h	\
	$(CXX_IN)/c89/std_lib/stdio.h	\
	$(CXX_IN)/c89/std_lib/assert.h	\
	$(CXX_IN)/c89/std_lib/limits.h	\
	$(CXX_IN)/c89/std_lib/time.h	\
	$(CXX_IN)/c89/std_lib/locale.h	\
	$(CXX_IN)/c89/std_lib/errno.h	\
	$(CXX_IN)/c89/std_lib/stdarg.h	\
	$(CXX_IN)/c89/std_lib/stdlib.h	\
	$(CXX_IN)/c89/std_lib/string.h	\
	$(CXX_IN)/c89/std_lib/signal.h	\
	$(CXX_IN)/c89/std_lib/math.h	\
	$(CXX_IN)/c89/std_lib/stddef.h	\
	$(CXX_IN)/c89/types/int.h	\
	$(CXX_IN)/c89/types/char.h	\
	$(CXX_IN)/c89/types/double.h	\
	$(CXX_IN)/c89/types/cstring.h

## Note: avoids intermediate targets which would cause the generated Makefile to
## try to rebuild p2-stubs.c (this requires doxygen and xsltproc).
$(OUT)/p2-stubs.c :  $(noinst_HEADERS) $(CXX_IN)/c89/operators.h
	doxygen p2-import.cfg
	xsltproc $(XML_OUT)/combine.xslt $(XML_OUT)/index.xml > $(XML_OUT)/p2-import.xml
	xsltproc p2-stubs.xslt $(XML_OUT)/p2-import.xml > $(OUT)/p2-stubs.c

$(CXX_IN)/c89/operators.h :  $(CXX_IN)/c89/operators.h.templ
	cp $(CXX_IN)/c89/operators.h.templ temp.h
	$(CC) temp.h -E -o $(CXX_IN)/c89/operators.h
	rm -f temp.h
