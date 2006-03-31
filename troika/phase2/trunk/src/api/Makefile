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
SK      = ./sk
UTILS   = ./util
XML     = ./xml

SUBDIRS = $(BNFTOOL) $(IMPORT) $(LOGTOOL) $(PARSER) $(UTILS) $(XML)


.PHONY : default
default :  phase2


.PHONY : clean
clean :
	for dir in $(SUBDIRS); do cd $$dir; $(MAKE) clean; cd ..; done
	-rm -f *.o $(SK)/sk.o phase2 revision.h


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

objects = main.o Closure.o Collection.o Compiler.o Environment.o Memory_Manager.o Namespace.o Object.o Primitive.o Primitive-import.o Type.o sk/sk.o serial.o


phase2 :  recursive $(utils) $(XML)/xmldom.o $(objects) import/target/imports.o parser
	$(CC) $(CFLAGS) -o phase2 $(objects) $(utils) import/target/imports.o parser/parser.o $(XML)/xmldom.o `xml2-config --libs`

$(objects) $(utils) : defs.h settings.h

serial.o :  revision.h


# Note: assumes that Phase2 is being built from a working copy.
# Defines the head revision number in revision.h, prepended with a '!' if
# serial.c differs from the repository copy.
revision.h :  serial.c
	svn stat -v serial.c \
	| sed 's/^[^ ][ ]*/&!/;s/^.[ ]*//;s/[ ].*//;s/.*/\#define REVISION    "&"/' \
	> revision.h


.PHONY: strip
strip :  phase2
	strip phase2


# DO NOT DELETE
