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

CXX_IN  = ./src
OUT     = ./target
XML_OUT = ./$(OUT)/xml
OBJ_OUT = ./$(OUT)/obj

INCLUDE = -I../../include

CFLAGS = -O2 -g --ansi -W -Wall
	# -pedantic
	# -Werror \
	# -fshort-enums -fno-common \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

CC      = gcc $(INCLUDE)


.PHONY : default
default :  $(OUT)/imports.o


.PHONY : clean
clean :
	-rm -rf $(OUT) $(CXX_IN)/p2-stubs.c


.PHONY : test
test :  $(OUT)/imports.o
	nm $(OUT)/imports.o


objects =			\
	$(OBJ_OUT)/char.o	\
	$(OBJ_OUT)/combinator.o	\
	$(OBJ_OUT)/cstring.o	\
	$(OBJ_OUT)/double.o	\
	$(OBJ_OUT)/int.o	\
	$(OBJ_OUT)/toys.o

other_dependencies = $(CXX_IN)/Set-operations.h


$(OUT)/imports.o : $(OUT)/p2-stubs.o $(objects)
	ld -r -o $(OUT)/imports.o $(OBJ_OUT)/*.o $(OUT)/p2-stubs.o


$(OBJ_OUT)/%.o : $(CXX_IN)/%.c ../settings.h #$(OBJ_OUT)
	@-mkdir -p $(OBJ_OUT)
	$(CC) -c $(CFLAGS) -o $@ $<


$(OUT)/p2-stubs.o : $(CXX_IN)/p2-stubs.c $(OBJ_OUT) ../settings.h
	$(CC) -c $(CFLAGS) -o $(OUT)/p2-stubs.o $(CXX_IN)/p2-stubs.c


$(CXX_IN)/p2-stubs.c :  $(XML_OUT)/p2-import.xml p2-stubs.xslt
	xsltproc p2-stubs.xslt $(XML_OUT)/p2-import.xml > $(CXX_IN)/p2-stubs.c


$(XML_OUT)/p2-import.xml :  $(objects) $(other_dependencies)
	doxygen p2-import.cfg
	xsltproc $(XML_OUT)/combine.xslt $(XML_OUT)/index.xml > $(XML_OUT)/p2-import.xml

