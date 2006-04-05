################################################################################
#
#  Makefile for the Phase2 programming API.
#  Author: Joshua Shinavier

INCLUDE  = -I./

export CFLAGS = -O2 -g --ansi -W -Wall #-pedantic
	# -Werror \
	# -fshort-enums -fno-common \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

LIBS    =

CC      = gcc $(INCLUDE) $(LIBS)


BNFTOOL = ./bnftool
IMPORT  = ./import
LOGTOOL = ./logtool
PARSER  = ./parser
UTILS   = ./util
XML     = ./xml

SUBDIRS = $(BNFTOOL) $(IMPORT) $(LOGTOOL) $(PARSER) $(UTILS) $(XML)


.PHONY : default
default :  phase2


.PHONY : clean
clean :
	for dir in $(SUBDIRS); do cd $$dir; $(MAKE) clean; cd ..; done
	-rm -f phase2 revision.h *.o compiler/*.o environment/*.o sk/*.o


.PHONY : recursive
recursive :
	for dir in $(SUBDIRS); do cd $$dir; $(MAKE) -e; cd ..; done


.PHONY : test
test :
	for dir in $(SUBDIRS); do cd $$dir; -$(MAKE) test; cd ..; done


################################################################################
# Compile utilities.

utils	=	$(UTILS)/Array.o \
		$(UTILS)/Bunch.o \
		$(UTILS)/Dictionary.o \
		$(UTILS)/Hash_Map.o \
		$(UTILS)/Hash_Table.o \
		$(UTILS)/Name.o \
		$(UTILS)/Set.o \
		$(UTILS)/Term.o


################################################################################
# XML utility

$(XML)/xmldom.o  :
	$(MAKE) -e -C $(XML)


################################################################################
# Build the command line interface.

objects = main.o Collection.o Memory_Manager.o Namespace.o Object.o Primitive.o Primitive-import.o Type.o sk/sk.o

compiler_objects = compiler/Compiler.o compiler/command.o compiler/error.o compiler/expression.o compiler/mapping.o compiler/serial.o

environment_objects = environment/Environment.o environment/combinators.o environment/primitives.o environment/types.o


phase2 :  recursive $(utils) $(XML)/xmldom.o $(objects) $(compiler_objects) $(environment_objects) import/target/imports.o parser
	$(CC) $(CFLAGS) -o phase2 $(objects) $(compiler_objects) $(environment_objects) $(utils) import/target/imports.o parser/parser.o $(XML)/xmldom.o `xml2-config --libs`

$(objects) $(compiler_objects) $(environment_objects) $(utils) : defs.h settings.h

compiler/serial.o :  revision.h


# Note: assumes that Phase2 is being built from a working copy.
# Defines the head revision number in revision.h, prepended with a '!' if
# serial.c differs from the repository copy.
revision.h :  compiler/serial.c
	svn stat -v compiler/serial.c \
	| sed 's/^[^ ][ ]*/&!/;s/^.[ ]*//;s/[ ].*//;s/.*/\#define REVISION    "&"/' \
	> revision.h


.PHONY: strip
strip :  phase2
	strip phase2


# DO NOT DELETE
