################################################################################
#
#  Makefile for the Phase2 programming API.
#  Author: Joshua Shinavier

INCLUDE  = -I./include

export CFLAGS = -O2 -g --ansi -W -Wall \
	-fpic  # Create position-independent code for the shared library.
	#-pedantic
	# -Werror \
	# -fshort-enums -fno-common \
	# -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
	# -D_GNU_SOURCE

LIBS    =

CC      = gcc $(INCLUDE) $(LIBS)


SUBDIRS = src/import src/parser src/util src/xml


.PHONY : default
default :  phase2


.PHONY : clean
clean :
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
	-rm -f phase2 libphase2.* revision.h src/*.o src/compiler/*.o src/environment/*.o src/sk/*.o


.PHONY : recursive
recursive :
	for dir in $(SUBDIRS); do $(MAKE) -e -C $$dir; done


.PHONY : test
test :
	for dir in $(SUBDIRS); do -$(MAKE) -C $$dir test; done
##	for dir in $(SUBDIRS); do cd $$dir; -$(MAKE) test; cd ..; done


utils	=				\
	src/util/Array.o		\
	src/util/Bunch.o		\
	src/util/Dictionary.o		\
	src/util/Hash_Map.o		\
	src/util/Hash_Table.o		\
	src/util/Name.o			\
	src/util/Set.o			\
	src/util/Term.o

core_objects =				\
	src/Collection.o		\
	src/Memory_Manager.o		\
	src/Namespace.o			\
	src/Object.o			\
	src/Primitive.o			\
	src/Primitive-import.o		\
	src/Type.o			\
	src/main.o			\
	src/sk/sk.o

compiler_objects =			\
	src/compiler/Compiler.o		\
	src/compiler/command.o		\
	src/compiler/error.o		\
	src/compiler/expression.o	\
	src/compiler/mapping.o		\
	src/compiler/serial.o

environment_objects = \
	src/environment/Environment.o	\
	src/environment/combinators.o	\
	src/environment/primitives.o	\
	src/environment/types.o

objects =				\
	$(utils)			\
	$(core_objects)			\
	$(compiler_objects)		\
	$(environment_objects)		\
	src/import/target/imports.o	\
	src/parser/parser.o		\
	src/xml/xmldom.o


$(core_objects) $(compiler_objects) $(environment_objects) $(utils) : src/settings.h

src/compiler/serial.o :  revision.h


# Note: assumes that Phase2 is being built from a working copy.
# Defines the head revision number in revision.h, prepended with a '!' if
# serial.c differs from the repository copy.
revision.h :  src/compiler/serial.c
	svn stat -v src/compiler/serial.c \
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

.PHONY: libs
libs :  libphase2.a libphase2.so

libphase2.a :  recursive $(objects)
	ar rc libphase2.a $(objects)
	ranlib libphase2.a

libphase2.so :  recursive $(objects)
	gcc -shared -o libphase2.so $(objects)


################################################################################
## Installation.

.PHONY: install
install :  phase2 libs
	cp phase2 /usr/bin
	cp libphase2.a libphase2.so /usr/lib


# DO NOT DELETE
