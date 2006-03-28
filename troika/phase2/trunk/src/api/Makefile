################################################################################
#
#  Makefile for the Phase2 programming API.
#  Author: Joshua Shinavier

BNFTOOL = ./bnftool
IMPORT  = ./import
LOGTOOL = ./logtool
PARSER  = ./parser
SK      = ./sk
UTILS   = ./util
XML     = ./xml


INCLUDE  = -I./

export CFLAGS = -O2 -g --ansi -pedantic -W -Wall
	# -Werror \
	# -fshort-enums -fno-common \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

LIBS    =

CC      = gcc $(INCLUDE) $(LIBS)



.PHONY : default
default :  phase2


.PHONY : clean
clean :
	$(MAKE) clean -C $(BNFTOOL)
	$(MAKE) clean -C $(IMPORT)
	$(MAKE) clean -C $(LOGTOOL)
	$(MAKE) clean -C $(PARSER)
	$(MAKE) clean -C $(UTILS)
	$(MAKE) clean -C $(XML)
	-rm -f *.o $(SK)/sk.o phase2


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

import/target/imports.o :
	$(MAKE) -e -C $(IMPORT)


################################################################################
# Compile utilities.

utils	=	$(UTILS)/Array.o \
		$(UTILS)/Bunch.o \
		$(UTILS)/Dictionary.o \
		$(UTILS)/Hash_Table.o \
		$(UTILS)/Hash_Map.o \
		$(UTILS)/Name.o \
		$(UTILS)/Set.o \
		$(UTILS)/Term.o


################################################################################
# XML utility

$(XML)/xmldom.o  :
	$(MAKE) -e -C $(XML)


################################################################################
# Build the command line interface.

objects = main.o Closure.o Collection.o Compiler.o Environment.o Memory_Manager.o Namespace.o Object.o Primitive.o Primitive-import.o Type.o sk/sk.o serial.o


phase2 :  $(utils) $(XML)/xmldom.o $(objects) import/target/imports.o parser
	$(CC) $(CFLAGS) -o phase2 $(objects) $(utils) import/target/imports.o $(PARSER_OBJECTS) $(XML)/xmldom.o `xml2-config --libs`

$(objects) $(utils) : defs.h settings.h


# DO NOT DELETE
