################################################################################
#
#  Makefile for the Phase2 programming API.
#  Author: Joshua Shinavier

BNF     = ./bnftool
IMPORT  = ./import
PARSER  = ./parser
SK      = ./sk
UTILS   = ./util
XML     = ./xml


INCLUDE  = -I./

export CFLAGS = -O2 -g --ansi -pedantic -W -Wall -fshort-enums -fno-common
	# -Werror \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

LIBS    =

CC      = gcc $(INCLUDE) $(LIBS)



.PHONY : default
default :  phase2


.PHONY : clean
clean :
	$(MAKE) clean -C $(BNF)
	$(MAKE) clean -C $(IMPORT)
	$(MAKE) clean -C $(PARSER)
	$(MAKE) clean -C $(UTILS)
	$(MAKE) clean -C $(XML)
	-rm -f *.o phase2


.PHONY : test
test :
	$(MAKE) test -C $(IMPORT)
	$(MAKE) test -C $(PARSER)
	$(MAKE) test -C $(UTILS)
	$(MAKE) test -C $(XML)


################################################################################
# Build the parser.

PARSER_OBJECTS = $(PARSER)/lex.yy.o \
                 $(PARSER)/p2_parser.tab.o \
                 $(PARSER)/p2_ast.o

.PHONY: parser
parser :
	$(MAKE) -e -C $(PARSER)


################################################################################
# Import primitives.

IMPORT_OBJECTS = \
	$(IMPORT)/target/cxx/p2-stubs.o \
	$(IMPORT)/target/cxx/char.o \
	$(IMPORT)/target/cxx/cstring.o \
	$(IMPORT)/target/cxx/combinator.o \
	$(IMPORT)/target/cxx/double.o \
	$(IMPORT)/target/cxx/int.o \
	$(IMPORT)/target/cxx/toys.o

.PHONY : import
import :
	$(MAKE) -e -C $(IMPORT)


################################################################################
# Compile utilities.

utils	=	$(UTILS)/Array.o \
		$(UTILS)/Bunch.o \
		$(UTILS)/Dictionary.o \
		$(UTILS)/Hash_Table.o \
		$(UTILS)/Lookup_Table.o \
		$(UTILS)/Name.o \
		$(UTILS)/Set.o \
		$(UTILS)/Term.o


################################################################################
# XML utility

$(XML)/xmldom.o  :
	$(MAKE) -e -C $(XML)


################################################################################
# Build the command line interface.

objects = main.o Collection.o Compiler.o Environment.o Memory_Manager.o Namespace.o Object.o Primitive.o Primitive-import.o Type.o sk/sk.o serial.o


phase2 :  $(utils) $(XML)/xmldom.o $(objects) import parser
	$(CC) $(CFLAGS) -o phase2 $(objects) $(utils) $(IMPORT_OBJECTS) $(PARSER_OBJECTS) $(XML)/xmldom.o `xml2-config --libs`

$(objects) $(utils) : defs.h settings.h


# DO NOT DELETE
