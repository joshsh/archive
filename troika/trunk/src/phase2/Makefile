############################################################
#
#  Makefile for the "Phase2" programming API.
#  author: Joshua Shinavier
#
#  Use "make", "make clean", "make distclean"


UTIL_PATH = ./util
SK_PATH = ./sk
PARSER_PATH = .
SOAP_PATH = ./xml
#LIBXML2_PATH = /usr/include/libxml2
GSOAP_PATH = ../../gsoap-linux-2.7
IMPORT_PATH = ./import

CC      = gcc
CPP     = g++
LEX     = flex
YACC    = bison
GSOAP   = $(GSOAP_PATH)/soapcpp2
SOAPH   = $(GSOAP_PATH)/stdsoap2.h
SOAPC   = $(GSOAP_PATH)/stdsoap2.c
SOAPCPP = $(GSOAP_PATH)/stdsoap2.cpp
DOMCPP  = $(GSOAP_PATH)/dom++.cpp

LIBS =
COFLAGS = -O2
CWFLAGS = -Wall
CIFLAGS =
CMFLAGS =
CFLAGS = $(CWFLAGS) $(COFLAGS) $(CIFLAGS) $(CMFLAGS)

UTIL_OBJECTS = \
	array.o \
	bunch.o \
	hash_table.o \
	#set.o \
	term.o \

PARSER_OBJECTS = \
	lex.yy.o \
	p2_parse.tab.o

#P2_OBJECTS = \
#	p2_error.o \
#	p2_command.o \
#	p2_type.o \
#	#p2_primitive.o

GSOAP_OUTPUT = \
	p2.nsmap
	# ....

CORE_LIBS = \
	p2.c \
	p2_error.c \
	p2_type.c \
	p2_primitive.c

APP_LIBS = \
	sk/sk.c \
	p2_syntax.c \
	p2_command.c \
	p2_dataset.cpp \
	p2_client.cpp \
	$(SOAP_PATH)/xmldom.h $(SOAP_PATH)/xmldom.cpp \
	$(IMPORT_PATH)/primitives.cpp \
	$(IMPORT_PATH)/commands.cpp


############################################################
# Build the command line interface.

phase2 :  $(CORE_LIBS) $(APP_LIBS) $(UTIL_OBJECTS) $(PARSER_OBJECTS) p2.nsmap # $(UTIL_OBJECTS) $(SOAP_PATH)/xmldom.h $(SOAP_PATH)/xmldom.cpp commands.cpp $(GSOAP_OUTPUT) p2_flags.h 
	$(CPP) -o phase2 -I$(GSOAP_PATH) $(CFLAGS) \
		$(UTIL_OBJECTS) \
		$(PARSER_OBJECTS) \
		p2_client.cpp \
		$(SOAP_PATH)/xmldom.cpp \
		$(SOAPCPP) $(LIBS) $(DOMCPP) soapC.cpp soapClient.cpp \
		#p2_dataset.o \
		#$(APP_LIBS) \
		#$(CORE_LIBS) \
		#$(SK_PATH)/sk.c \

############################################################
# Invoke gSOAP's "stdsoap2" to generate proxy code.

p2.nsmap : $(SOAP_PATH)/gsoap_header.h
	$(GSOAP) $(SOAP_PATH)/gsoap_header.h



############################################################
# Compile p2_error and p2_command libraries.

#p2_error.o : p2_error.c p2_flags.h
#	$(CC) -c p2_error.c

#p2_syntax.o : p2_syntax.c p2_flags.h
#	$(CC) -c p2_syntax.c

#p2_command.o : p2_command.c p2_error.h $(UTIL_PATH)/p2_array.h $(UTIL_PATH)/p2_hash_table.h
#	$(CC) -c p2_command.c
#	
#p2_type.o : p2_type.c p2_error.h $(UTIL_PATH)/p2_array.h $(UTIL_PATH)/p2_hash_table.h
#	$(CC) -c p2_type.c
#	
#p2_primitive.o : p2_primitive.c p2_error.h $(UTIL_PATH)/p2_array.h $(UTIL_PATH)/p2_hash_table.h
#	$(CC) -c p2_primitive.c


############################################################
# Compile the p2_data_set class.

p2_dataset.o : p2_dataset.cpp p2_dataset.h
	$(CPP) -c p2_dataset.cpp


############################################################
# Compile SK semantics library.

#sk.o : $(SK_PATH)/sk.c p2.h p2_flags.h
#	$(CC) -c sk.c


############################################################
# Compile helper utilities.

array.o : $(UTIL_PATH)/p2_array.c
	$(CC) -c $(UTIL_PATH)/p2_array.c

bunch.o : $(UTIL_PATH)/p2_bunch.c
	$(CC) -c $(UTIL_PATH)/p2_bunch.c

hash_table.o : $(UTIL_PATH)/p2_hash_table.c
	$(CC) -c $(UTIL_PATH)/p2_hash_table.c

array.o : $(UTIL_PATH)/p2_array.c
	$(CC) -c $(UTIL_PATH)/p2_array.c

#set.o : $(UTIL_PATH)/p2_set.c
#	$(CC) -c $(UTIL_PATH)/p2_set.c

term.o : $(UTIL_PATH)/p2_term.c
	$(CC) -c $(UTIL_PATH)/p2_term.c


############################################################
# Compile the (flex/bison) parser.

FLEX_OUTPUT = lex.yy.c

BISON_OUTPUT = \
	p2_parse.tab.c \
	p2_parse.tab.h \
	p2_parse.output \
	#p2_parse.vcg

lex.yy.o : lex.yy.c p2_parse.tab.h p2_flags.h
	$(CC) -c lex.yy.c

p2_parse.tab.o : p2_parse.tab.c
	$(CC) -c p2_parse.tab.c

lex.yy.c : p2_parse.l
	$(LEX) p2_parse.l

p2_parse.tab.c p2_parse.tab.h : p2_parse.y p2_flags.h
	$(YACC) -dv p2_parse.y



############################################################
# "make clean" (Note: doesn't remove the executable)

.PHONY : clean
clean : 
	-rm \
	$(UTIL_OBJECTS) \
	$(PARSER_OBJECTS) \
	$(FLEX_OUTPUT) \
	$(BISON_OUTPUT) \

.PHONY : distclean
distclean:
	-rm \
	$(UTIL_OBJECTS) \
	$(PARSER_OBJECTS) \
	$(FLEX_OUTPUT) \
	$(BISON_OUTPUT) \
	rm -f *.o *.wsdl *.xsd *.xml *.nsmap *.log soap*.* phase2


