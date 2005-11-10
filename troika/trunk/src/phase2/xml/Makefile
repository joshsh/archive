################################################################################
## 

LIBXML2_FLAGS    = `xml2-config --cflags`
LIBXML2_LIBS     = `xml2-config --libs`

## Using the libxml2 toolkit.
FLAGS = $(LIBXML2_FLAGS)
LIBS  = $(LIBXML2_LIBS)

all :  xmldom.o

clean :
	-rm xmldom-*.o test.xml

distclean :  clean
	-rm xmldom.o xmldom-test

test :  xmldom-test

xmldom-test :  xmldom-test.c xmldom.h xmldom.o
	gcc -o xmldom-test xmldom-test.c xmldom.o $(LIBS)

## Using the libxml2 toolkit.
xmldom.o :  xmldom-libxml2.o
	cp xmldom-libxml2.o xmldom.o

xmldom-libxml2.o :  xmldom.h libxml2/xmldom-libxml2.c
	gcc -c -o xmldom-libxml2.o $(FLAGS) libxml2/xmldom-libxml2.c # $(LIBXML2_LIBS)

