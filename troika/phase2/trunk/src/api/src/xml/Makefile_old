################################################################################
# Makefile for the XML DOM interface module.
#
# Use "make test" to compile the test program.

LIBXML2_FLAGS  = `xml2-config --cflags`
LIBXML2_LIBS   = `xml2-config --libs`
VALGRIND       = valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes

# Using the libxml2 toolkit.
FLAGS          = $(LIBXML2_FLAGS)
LIBS           = $(LIBXML2_LIBS)

CFLAGS  = -g          -O2                 --ansi  -Wall

INCLUDE = -I../../include

CC      = gcc $(CFLAGS) $(INCLUDE)


.PHONY : default
default :  xmldom.o


.PHONY : clean
clean :
	-rm *.o test.xml xmldom-test


.PHONY : test
test :  xmldom-test
	./xmldom-test
	$(VALGRIND) xmldom-test


xmldom-test :  xmldom-test.c xmldom.o
	$(CC) -o xmldom-test xmldom-test.c xmldom.o $(LIBS)


# Using the libxml2 toolkit.
xmldom.o :  xmldom-libxml2.o
	cp xmldom-libxml2.o xmldom.o


xmldom-libxml2.o :  libxml2/xmldom-libxml2.c
	$(CC) -c $(FLAGS) libxml2/xmldom-libxml2.c

