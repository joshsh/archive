## Process this file with automake to produce Makefile.in

##SUBDIRS = util
SUBDIRS = import parser xml

BUILT_SOURCES = revision.h

bin_PROGRAMS = phase2

phase2_LDADD = import/libp2prims.la

INCLUDES = -I../include `xml2-config --cflags`
LIBS = `xml2-config --libs`

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

parser_src = \
	parser/lexer.c	\
	parser/parser.c

primitive_src = \
	Primitive.c		\
	Primitive-import.c

xml_src = \
	xml/libxml2/xmldom-libxml2.c

phase2_SOURCES = main.c \
	$(compiler_src)		\
	$(environment_src)	\
	$(parser_src)		\
	$(primitive_src)	\
	$(xml_src)		\
	Ast.c \
	Collection.c		\
	Memory_Manager.c	\
	Namespace.c		\
	Object.c \
	Type.c \
	sk/sk.c \
	util/Array.c		\
	util/Bunch.c		\
	util/Dictionary.c	\
	util/Hash_Map.c		\
	util/Hash_Table.c	\
	util/Name.c		\
	util/Set.c		\
	util/Term.c


## Defines the head revision number in revision.h, prepended with a '!' if
## serial.c differs from the repository copy.
## Note: this assumes that Phase2 is being built from a Subversion working copy.
revision.h :  compiler/serial.c
	svn stat -v compiler/serial.c \
	| sed 's/^[^ ][ ]*/&!/;s/^.[ ]*//;s/[ ].*//;s/.*/\#define REVISION    "&"/' \
	> revision.h


