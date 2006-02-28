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
	-rm -f *.o


.PHONY : distclean
distclean :
	$(MAKE) distclean -C $(BNF)
	$(MAKE) distclean -C $(IMPORT)
	$(MAKE) distclean -C $(PARSER)
	$(MAKE) distclean -C $(UTILS)
	$(MAKE) distclean -C $(XML)
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
# Compile utilities.

.PHONY : utils
utils :
	$(MAKE) -C $(UTILS)


################################################################################
# Build the command line interface.

.PHONY : phase2
phase2 :  utils $(PARSER_OBJECTS) p2_memory_manager.o p2_namespace.o p2_object.o p2_primitive.o p2_type.o sk.o


p2_memory_manager.o :  p2_memory_manager.c p2_flags.h
	$(CC) -c p2_memory_manager.c

p2_namespace.o :  p2_namespace.c p2_flags.h
	$(CC) -D_GNU_SOURCE -c p2_namespace.c

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

