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


#         |temporary  |bigger but faster  |       |
CFLAGS  = -g          -O2                 --ansi  -Wall
CC      = gcc $(CFLAGS)
LIBS    =


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

.PHONY: $(PARSER_OBJECTS)
$(PARSER_OBJECTS) :
	$(MAKE) -C $(PARSER)


################################################################################
# Import primitives.

IMPORT_OBJECTS = \
	$(IMPORT)/target/cxx/p2-stubs.o \
	$(IMPORT)/target/cxx/cstring.o \
	$(IMPORT)/target/cxx/combinator.o \
	$(IMPORT)/target/cxx/double.o \
	$(IMPORT)/target/cxx/int.o

.PHONY : $(IMPORT_OBJECTS)
$(IMPORT_OBJECTS) :
	$(MAKE) -C $(IMPORT)


################################################################################
# Compile utilities.

.PHONY : utils
utils :
	$(MAKE) -C $(UTILS)

UTIL_OBJECTS =	$(UTILS)/p2_array.o \
		$(UTILS)/p2_bunch.o \
		$(UTILS)/p2_dictionary.o \
		$(UTILS)/p2_hash_table.o \
		$(UTILS)/p2_name.o \
		$(UTILS)/p2_term.o


################################################################################
# Build the command line interface.

OBJECTS = p2_compiler.o p2_environment.o p2_memory_manager.o p2_primitive-import.o p2_namespace.o p2_collection.o p2_object.o p2_primitive.o p2_type.o sk.o $(IMPORT_OBJECTS) $(PARSER_OBJECTS)

phase2 :  main.c utils $(OBJECTS)
	$(CC) -o phase2 main.c $(OBJECTS) $(UTIL_OBJECTS)


p2_compiler.o : p2_compiler.c p2_flags.h
	$(CC) -c p2_compiler.c

p2_environment.o :  p2_environment.c p2_flags.h
	$(CC) -c p2_environment.c

p2_primitive-import.o :  p2_primitive-import.c p2_flags.h
	$(CC) -c p2_primitive-import.c

p2_memory_manager.o :  p2_memory_manager.c p2_flags.h
	$(CC) -c p2_memory_manager.c

p2_namespace.o :  p2_namespace.c p2_flags.h
	$(CC) -D_GNU_SOURCE -c p2_namespace.c

p2_collection.o :  p2_collection.c p2_flags.h
	$(CC) -c p2_collection.c

p2_object.o :  p2_object.c p2_flags.h
	$(CC) -c p2_object.c

p2_primitive.o :  p2_primitive.c p2_flags.h
	$(CC) -c p2_primitive.c

p2_type.o :  p2_type.c p2_flags.h
	$(CC) -c p2_type.c


################################################################################
# Compile SK semantics library.

sk.o :  sk/sk.c p2_flags.h
	$(CC) -c sk/sk.c

