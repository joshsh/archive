# Makefile for the SParse SNPL interpreter/parser.
# author: Joshua Shinavier
# last edited: 1/19/2005
#
# make, make clean

SParse_objects = \
	lex.yy.o \
	sparse.tab.o

util_objects = \
	array.o \
	associative_array.o \
	expanding_string.o \
	lookup_table.o \
	strutil.o \
	ternary_associative_array.o
	
SNPL_objects = \
	cpp_primitive.o \
	data_type.o \
	executor.o \
	expression.o \
	import.o \
	primitive_shell.o \
	registry.o \
	serial.o \
	snpl.o
	#http.o \
	
misc_objects = \
	bitmap.o \
	color.o \
	frame.o \
	
graphics_objects = \
	gui.o \
	key_map.o \
	SDLprims.o
	
include_dir = include
import_dir = $(include_dir)/import
util_dir = $(include_dir)/util
type_dir = $(include_dir)/type
SParse_dir = SParse0
SDL_dir = /usr/include/SDL

CC = gcc
++ = g++



#############################################################
# Build the SParse interpreter.

sparse : $(SParse_dir)/sparse.cpp $(SParse_objects) $(util_objects) $(SNPL_objects) $(include_dir)/snpl.h $(misc_objects) $(graphics_objects)
	$(++) -o sparse $(SParse_dir)/sparse.cpp $(SParse_objects) $(SNPL_objects) $(util_objects) $(misc_objects) $(graphics_objects) -lpthread -lSDL -lSDL_image



#############################################################
# Compile SNPL (minus utilities).

cpp_primitive.o : $(include_dir)/cpp_primitive.cpp
	$(++) -c $(include_dir)/cpp_primitive.cpp

data_type.o : $(include_dir)/data_type.cpp
	$(++) -c $(include_dir)/data_type.cpp

executor.o : $(include_dir)/executor.cpp
	$(++) -c $(include_dir)/executor.cpp

expression.o : $(include_dir)/expression.cpp
	$(++) -c $(include_dir)/expression.cpp

#http.o : $(include_dir)/http.cpp
#	$(++) -c $(include_dir)/http.cpp

import.o : import.cpp
	$(++) -c $(include_dir)/import.cpp -I$(SDL_dir)

primitive_shell.o : $(include_dir)/primitive_shell.cpp
	$(++) -c $(include_dir)/primitive_shell.cpp

registry.o : $(include_dir)/registry.cpp
	$(++) -c $(include_dir)/registry.cpp

serial.o : $(include_dir)/serial.cpp $(include_dir)/serializer.h $(include_dir)/deserializer.h
	$(++) -c $(include_dir)/serial.cpp

snpl.o : $(include_dir)/snpl.cpp
	$(++) -c $(include_dir)/snpl.cpp



#############################################################
# Compile miscellaneous components (non-SDL imported types)

bitmap.o : $(type_dir)/bitmap.cpp
	$(++) -c $(type_dir)/bitmap.cpp

color.o : $(type_dir)/color.cpp
	$(++) -c $(type_dir)/color.cpp

frame.o : $(type_dir)/frame.cpp
	$(++) -c $(type_dir)/frame.cpp
	
	

#############################################################
# Compile graphics components.

SDLprims.o : $(include_dir)/SDLprims.cpp
	$(++) -c $(include_dir)/SDLprims.cpp

gui.o : $(type_dir)/gui.cpp
	$(++) -c $(type_dir)/gui.cpp

key_map.o : $(type_dir)/key_map.cpp
	$(++) -c $(type_dir)/key_map.cpp



#############################################################
# Generate import.cpp.

import_output = import.yy.c import.tab.c import.tab.h

import.cpp : import $(import_dir)/import.txt
	./import < $(import_dir)/import.txt > $(include_dir)/import.cpp

import : import.yy.c import.tab.c import.tab.h
	$(CC) -o import import.yy.c import.tab.c
	-rm $(import_output)

import.yy.c : $(import_dir)/import.l
	cp $(import_dir)/import.l .
	flex -oimport.yy.c import.l
	rm import.l

import.tab.c import.tab.h : $(import_dir)/import.y
	cp $(import_dir)/import.y .
	bison -d import.y
	rm import.y



#############################################################
# Compile the utility libraries.

#.PHONY : utils
#utils : $(include_dir)/util/*.cpp $(include_dir)/util/*.h
#	$(++) -c $(include_dir)/util/*.cpp

array.o : $(util_dir)/array.cpp
	$(++) -c $(util_dir)/array.cpp
	
associative_array.o  : $(util_dir)/associative_array.cpp
	$(++) -c $(util_dir)/associative_array.cpp

expanding_string.o  : $(util_dir)/expanding_string.cpp
	$(++) -c $(util_dir)/expanding_string.cpp

lookup_table.o  : $(util_dir)/lookup_table.cpp
	$(++) -c $(util_dir)/lookup_table.cpp

strutil.o  : $(util_dir)/strutil.cpp
	$(++) -c $(util_dir)/strutil.cpp

ternary_associative_array.o : $(util_dir)/ternary_associative_array.cpp
	$(++) -c $(util_dir)/ternary_associative_array.cpp



#############################################################
# Compile the Flex and Bison portions of the interpreter.

flex_output = lex.yy.c
bison_output = sparse.tab.c sparse.tab.h sparse.output sparse.vcg
SParse_input = debug.h jstring.h sparse.l sparse.y

.PHONY : flex_bison
flex_bison : lex.yy.o sparse.tab.o

lex.yy.o : lex.yy.c sparse.tab.h debug.h jstring.h
	$(CC) -c lex.yy.c

sparse.tab.o : sparse.tab.c
	$(CC) -c sparse.tab.c

lex.yy.c : sparse.l
	flex sparse.l

sparse.tab.c sparse.tab.h : sparse.y debug.h jstring.h
	bison -d -v sparse.y    # verbose output

sparse.y : $(SParse_dir)/sparse.y
	cp $(SParse_dir)/sparse.y .

sparse.l : $(SParse_dir)/sparse.l
	cp $(SParse_dir)/sparse.l .

jstring.h : $(SParse_dir)/jstring.h
	cp $(SParse_dir)/jstring.h .

debug.h : $(SParse_dir)/debug.h
	cp $(SParse_dir)/debug.h .



#############################################################
# "make clean"

.PHONY : clean
clean : 
	-rm \
	$(SParse_objects) \
	$(util_objects) \
	$(misc_objects) \
	$(graphics_objects) \
	$(SNPL_objects) \
	$(SParse_input) \
	$(flex_output) \
	$(bison_output) \
	$(import_output)




