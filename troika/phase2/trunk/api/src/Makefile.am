## Process this file with automake to produce Makefile.in

SUBDIRS = import parser collection xml

BUILT_SOURCES = revision.h
EXTRA_DIST = $(BUILT_SOURCES)

noinst_HEADERS = revision.h						\
	settings.h							\
	compiler/Apply.h						\
	compiler/Indirection.h						\
	getopt/getopt.h							\
	object/Object-impl.h						\
	primitive/Primitive-impl.h					\
	interpreter/Interpreter-impl.h					\
	interpreter/license.h						\
	environment/Environment-impl.h					\
	namespace/Namespace-impl.h					\
	type/Type-impl.h

bin_PROGRAMS = phase2

## Note: libtool installs all of the following on "make install":
##   libphase2.a
##   libphase2.la
##   libphase2.so
##   libphase2.so.0
##   libphase2.so.0.0.0
lib_LTLIBRARIES = libphase2.la


compiler_src =								\
	compiler/Apply.c						\
	compiler/Indirection.c						\
	compiler/graph-reduce.c						\
	compiler/term-reduce.c

environment_src =							\
	environment/Environment.c					\
	environment/combinators.c					\
	environment/primitives.c					\
	environment/types.c

interpreter_src =							\
	interpreter/Interpreter.c					\
	interpreter/command.c						\
	interpreter/error.c						\
	interpreter/expression.c					\
	interpreter/history.c						\
	interpreter/mapping.c						\
	interpreter/serial.c

manager_src =								\
	manager/Manager.c

model_src = 								\
	model/ModelVector.c

namespace_src =								\
	namespace/Namespace.c						\
	namespace/io-simple.c						\
	namespace/io-search.c

object_src =								\
	object/Object.c							\
	object/association.c						\
	object/methods.c						\
	object/traversal.c

primitive_src =								\
	primitive/Primitive.c						\
	primitive/Primitive-import.c


type_src = \
	type/Type.c


libphase2_la_SOURCES =							\
	$(compiler_src)							\
	$(environment_src)						\
	$(interpreter_src)						\
	$(manager_src)							\
	$(model_src)							\
	$(namespace_src)						\
	$(object_src)							\
	$(primitive_src)						\
	$(type_src)							\
	parser/Parser.c							\
	collection/Collection.c						\
	debug.c								\
	getopt/getopt.c							\
	getopt/getopt1.c

## Convenience libraries from each subdirectory with its own Makefile.am.
libphase2_la_LIBADD =							\
	collection/libp2collection.la					\
	import/libp2prims.la						\
	parser/libp2parser.la						\
	xml/libp2xml.la

##libphase2_la_LDFLAGS = -version-info 0:0:8
##libphase2_la_LDFLAGS = -release 0.8.1


phase2_SOURCES = main.c

phase2_LDADD = libphase2.la


## Defines the head revision number in revision.h, prepended with a '!' if
## serial.c differs from the repository copy.
## Note: this assumes that Phase2 is being built from a Subversion working copy.
revision.h :  interpreter/serial.c
	svn stat -v interpreter/serial.c \
	| sed 's/^[^ ][ ]*/&!/;s/^.[ ]*//;s/[ ].*//;s/.*/\#define REVISION    "&"/' \
	> revision.h
