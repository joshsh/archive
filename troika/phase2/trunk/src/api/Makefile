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

export CFLAGS = \
	-O2 \
	-g \
	--ansi -pedantic \
	-W -Wall # -Werror \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -fshort-enums -fno-common \
	# -D_GNU_SOURCE

LIBS    = `xml2-config --libs`

CC      = gcc $(CFLAGS) $(INCLUDE) $(LIBS)



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

.PHONY : utils
utils :
	$(MAKE) -e -C $(UTILS)

UTIL_OBJECTS =	$(UTILS)/p2_array.o \
		$(UTILS)/p2_bunch.o \
		$(UTILS)/p2_dictionary.o \
		$(UTILS)/p2_hash_table.o \
		$(UTILS)/p2_lookup_table.o \
		$(UTILS)/p2_name.o \
		$(UTILS)/p2_set.o \
		$(UTILS)/p2_term.o


################################################################################
# Compile XML util.

$(XML)/xmldom.o :
	$(MAKE) -C $(XML)


################################################################################
# Build the command line interface.

OBJECTS = p2_compiler.o p2_environment.o p2_memory_manager.o p2_primitive-import.o p2_namespace.o p2_collection.o p2_object.o p2_primitive.o p2_type.o sk.o serial.o

phase2 :  main.c utils $(XML)/xmldom.o $(OBJECTS) import parser
	$(CC) -o phase2 main.c $(OBJECTS) $(UTIL_OBJECTS) $(IMPORT_OBJECTS) $(PARSER_OBJECTS) $(XML)/xmldom.o


serial.o :  serial.c serial.h flags.h
	$(CC) -c serial.c

p2_%.o :  p2_%.c p2_%.h flags.h
	$(CC) -c p2_$*.c


################################################################################
# Compile SK semantics library.

sk.o :  sk/sk.c flags.h
	$(CC) -c sk/sk.c

