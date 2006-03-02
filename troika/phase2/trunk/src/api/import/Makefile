################################################################################
## Makefile for the Phase2 importing utility.
##
## This program builds an object file which is linked with the Phase2 API source
## when an application is compiled.  To import a function [or data type], place a
## skeleton header file in the src directory.  If this function [or data type]
## depends on special libraries or compiler options, these must be specified in
## the project Makefile.
##
## Dependencies: doxygen, xsltproc

NAME         = p2-import

CXX_IN  = ./src
CXX_OUT = ./target/cxx
XML_OUT = ./target/xml
OBJ_OUT = ./target

CFLAGS       = -g -Wall --pedantic-errors  # These are temporary.
CC           = gcc $(CFLAGS)


.PHONY : default
default :  objects


.PHONY : clean
clean :
	-rm -rf $(CXX_OUT) $(XML_OUT) $(OBJ_OUT)


.PHONY : test
test :  $(OBJ_OUT)/p2-stubs.o
	nm $(OBJ_OUT)/p2-stubs.o

.PHONY : objects
objects : $(CXX_OUT)/p2-stubs.c
	$(CC) -c $(CXX_OUT)/*.c
	mv *.o $(CXX_OUT)


$(OBJ_OUT)/p2-stubs.o :  $(CXX_OUT)/p2-stubs.c
	$(CC) -c $(CXX_OUT)/p2-stubs.c -o $(OBJ_OUT)/p2-stubs.o


$(CXX_OUT)/p2-stubs.c :  $(XML_OUT)/p2-import.xml p2-stubs.xslt
	-rm -rf $(CXX_OUT)
	cp -r $(CXX_IN) $(CXX_OUT)
	xsltproc p2-stubs.xslt $(XML_OUT)/p2-import.xml > $(CXX_OUT)/p2-stubs.c


$(XML_OUT)/p2-import.xml :  doxygen
	xsltproc $(XML_OUT)/combine.xslt $(XML_OUT)/index.xml > $(XML_OUT)/p2-import.xml


.PHONY : doxygen
doxygen :
	doxygen p2-import.cfg

