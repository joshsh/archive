################################################################################
#
#  Makefile for the Phase2 programming API.
#  Author: Joshua Shinavier

UTIL_PATH    = ./util
SK_PATH      = ./sk
PARSER_PATH  = ./parser
XML_PATH     = ./xml
IMPORT_PATH  = ./import


#         |temporary  |bigger but faster  |
CFLAGS  = -g          -O2                 --ansi
CC      = gcc $(CFLAGS)
LIBS    =


.PHONY : default
default :  phase2


.PHONY : clean
clean :
	$(MAKE) clean -C $(IMPORT_PATH)
	$(MAKE) clean -C $(PARSER_PATH)
	$(MAKE) clean -C $(UTIL_PATH)
	$(MAKE) clean -C $(XML_PATH)
	-rm -f *.o


.PHONY : distclean
distclean :
	$(MAKE) distclean -C $(IMPORT_PATH)
	$(MAKE) distclean -C $(PARSER_PATH)
	$(MAKE) distclean -C $(UTIL_PATH)
	$(MAKE) distclean -C $(XML_PATH)
	-rm -f *.o phase2


.PHONY : test
test :
	$(MAKE) test -C $(IMPORT_PATH)
	$(MAKE) test -C $(PARSER_PATH)
	$(MAKE) test -C $(UTIL_PATH)
	$(MAKE) test -C $(XML_PATH)


################################################################################
# Build the parser.

PARSER_OBJECTS = $(PARSER_PATH)/lex.yy.o \
                 $(PARSER_PATH)/p2_parser.tab.o \
                 $(PARSER_PATH)/p2_ast.o

.PHONY: $(PARSER_OBJECTS)
$(PARSER_OBJECTS) :
	$(MAKE) -C $(PARSER_PATH)


################################################################################
# Compile utilities.

UTIL_OBJECTS = $(UTIL_PATH)/p2_array.o \
               $(UTIL_PATH)/p2_bunch.o \
               $(UTIL_PATH)/p2_hash_table.o \
               $(UTIL_PATH)/p2_set.o \
               $(UTIL_PATH)/p2_term.o

.PHONY : $(UTIL_OBJECTS)
$(UTIL_OBJECTS) :
	$(MAKE) -C $(UTIL_PATH)


################################################################################
# Build the command line interface.

.PHONY : phase2
phase2 :  $(UTIL_OBJECTS) $(PARSER_OBJECTS) p2_memory_manager.o p2_namespace.o p2_object.o p2_primitive.o p2_type.o


p2_memory_manager.o :  p2_memory_manager.c
	$(CC) -c p2_memory_manager.c

p2_namespace.o :  p2_namespace.c
	$(CC) -D_GNU_SOURCE -c p2_namespace.c

p2_object.o :  p2_object.c
	$(CC) -c p2_object.c

p2_primitive.o :  p2_primitive.c
	$(CC) -c p2_primitive.c

p2_type.o :  p2_type.c
	$(CC) -c p2_type.c


################################################################################
# Compile SK semantics library.

#sk.o :  $(SK_PATH)/sk.c p2.h p2_flags.h
#	$(CC) -c sk.c

