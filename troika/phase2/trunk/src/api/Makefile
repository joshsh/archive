################################################################################
#
#  Makefile for the Phase2 programming API.
#  Author: Joshua Shinavier

INCLUDE  = -I./

export CFLAGS = -O2 -g --ansi -W -Wall \
	-fpic  # Create position-independent code for the shared library.
	#-pedantic
	# -Werror \
	# -fshort-enums -fno-common \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

LIBS    =

CC      = gcc $(INCLUDE) $(LIBS)


SUBDIRS = ./import ./parser ./util ./xml


.PHONY : default
default :  phase2


.PHONY : clean
clean :
	for dir in $(SUBDIRS); do cd $$dir; $(MAKE) clean; cd ..; done
	-rm -f phase2 libphase2.* revision.h *.o compiler/*.o environment/*.o sk/*.o


.PHONY : recursive
recursive :
	for dir in $(SUBDIRS); do cd $$dir; $(MAKE) -e; cd ..; done


.PHONY : test
test :
	for dir in $(SUBDIRS); do cd $$dir; -$(MAKE) test; cd ..; done


utils	=	util/Array.o \
		util/Bunch.o \
		util/Dictionary.o \
		util/Hash_Map.o \
		util/Hash_Table.o \
		util/Name.o \
		util/Set.o \
		util/Term.o

core_objects = main.o Collection.o Memory_Manager.o Namespace.o Object.o Primitive.o Primitive-import.o Type.o sk/sk.o

compiler_objects = compiler/Compiler.o compiler/command.o compiler/error.o compiler/expression.o compiler/mapping.o compiler/serial.o

environment_objects = environment/Environment.o environment/combinators.o environment/primitives.o environment/types.o

objects = $(utils) $(core_objects) $(compiler_objects) $(environment_objects) import/target/imports.o parser/parser.o xml/xmldom.o


$(core_objects) $(compiler_objects) $(environment_objects) $(utils) : defs.h settings.h

compiler/serial.o :  revision.h


# Note: assumes that Phase2 is being built from a working copy.
# Defines the head revision number in revision.h, prepended with a '!' if
# serial.c differs from the repository copy.
revision.h :  compiler/serial.c
	svn stat -v compiler/serial.c \
	| sed 's/^[^ ][ ]*/&!/;s/^.[ ]*//;s/[ ].*//;s/.*/\#define REVISION    "&"/' \
	> revision.h


################################################################################
# Executable.

phase2 :  recursive $(objects)
	$(CC) $(CFLAGS) -o phase2  $(objects) `xml2-config --libs`

.PHONY: strip
strip :  phase2
	strip phase2


################################################################################
# Libraries.

.PHONY: libraries
libraries : libphase2.a libphase2.so

libphase2.a :  recursive $(objects)
	ar rc libphase2.a $(objects)
	ranlib libphase2.a

libphase2.so :  recursive $(objects)
	gcc -shared -o libphase2.so $(objects)


# DO NOT DELETE
