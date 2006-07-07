## Process this file with automake to produce Makefile.in

SUBDIRS = import parser collection xml

BUILT_SOURCES = revision.h
noinst_HEADERS = revision.h		\
	settings.h			\
	object/Object-impl.h		\
	primitive/Primitive-impl.h	\
	interpreter/Interpreter-impl.h	\
	interpreter/license.h		\
	environment/Environment-impl.h	\
	namespace/Namespace-impl.h	\
	compiler/Apply.h			\
	compiler/graph.h			\
	compiler/graph-types.h

bin_PROGRAMS = phase2

## Note: libtool installs all of the following on "make install":
##   libphase2.a
##   libphase2.la
##   libphase2.so
##   libphase2.so.0
##   libphase2.so.0.0.0
lib_LTLIBRARIES = libphase2.la

LIBS := $(LIBS) -lxml2 -lncurses ##`xml2-config --libs`


environment_src = \
	environment/Environment.c	\
	environment/combinators.c	\
	environment/primitives.c	\
	environment/types.c

interpreter_src = \
	interpreter/Interpreter.c	\
	interpreter/command.c		\
	interpreter/error.c		\
	interpreter/expression.c	\
	interpreter/mapping.c		\
	interpreter/serial.c

namespace_src = \
	namespace/Namespace.c	\
	namespace/io-simple.c	\
	namespace/io-search.c

object_src = \
	object/Object.c		\
	object/association.c	\
	object/methods.c	\
	object/traversal.c

primitive_src = \
	primitive/Primitive.c		\
	primitive/Primitive-import.c

compiler_src = \
	compiler/Apply.c	\
	compiler/graph.c	\
	compiler/sk.c


libphase2_la_SOURCES = \
	$(interpreter_src)	\
	$(environment_src)	\
	$(namespace_src)	\
	$(object_src)		\
	$(primitive_src)	\
	$(compiler_src)		\
	$(xml_src)		\
	Ast.c			\
	collection/Collection.c	\
	Memory_Manager.c	\
	Type.c

## Convenience libraries from each subdirectory with its own Makefile.am.
libphase2_la_LIBADD = \
	import/libp2prims.la		\
	parser/libp2parser.la		\
	collection/libp2collection.la	\
	xml/libp2xml.la

phase2_SOURCES = main.c

phase2_LDADD = libphase2.la


## Defines the head revision number in revision.h, prepended with a '!' if
## serial.c differs from the repository copy.
## Note: this assumes that Phase2 is being built from a Subversion working copy.
revision.h :  interpreter/serial.c
	svn stat -v interpreter/serial.c \
	| sed 's/^[^ ][ ]*/&!/;s/^.[ ]*//;s/[ ].*//;s/.*/\#define REVISION    "&"/' \
	> revision.h
