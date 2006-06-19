## Process this file with automake to produce Makefile.in

SUBDIRS = import parser util xml

BUILT_SOURCES = revision.h
noinst_HEADERS = revision.h		\
	settings.h			\
	object/Object-impl.h		\
	Primitive-impl.h		\
	compiler/Compiler-impl.h	\
	compiler/license.h		\
	environment/Environment-impl.h	\
	namespace/Namespace-impl.h	\
	sk/Apply.h			\
	sk/graph.h			\
	sk/graph-types.h

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

compiler_src = \
	compiler/Compiler.c	\
	compiler/command.c	\
	compiler/error.c	\
	compiler/expression.c	\
	compiler/mapping.c	\
	compiler/serial.c

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
	Primitive.c		\
	Primitive-import.c

sk_src = \
	sk/Apply.c		\
	sk/graph.c		\
	sk/sk.c


libphase2_la_SOURCES = \
	$(compiler_src)		\
	$(environment_src)	\
	$(namespace_src)	\
	$(object_src)		\
	$(primitive_src)	\
	$(sk_src)		\
	$(xml_src)		\
	Ast.c \
	Collection.c		\
	Memory_Manager.c	\
	Type.c

## Convenience libraries from each subdirectory with its own Makefile.am.
libphase2_la_LIBADD = \
	import/libp2prims.la	\
	parser/libp2parser.la	\
	util/libp2utils.la	\
	xml/libp2xml.la

phase2_SOURCES = main.c

phase2_LDADD = libphase2.la


## Defines the head revision number in revision.h, prepended with a '!' if
## serial.c differs from the repository copy.
## Note: this assumes that Phase2 is being built from a Subversion working copy.
revision.h :  compiler/serial.c
	svn stat -v compiler/serial.c \
	| sed 's/^[^ ][ ]*/&!/;s/^.[ ]*//;s/[ ].*//;s/.*/\#define REVISION    "&"/' \
	> revision.h
