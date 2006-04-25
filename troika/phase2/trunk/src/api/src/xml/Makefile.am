## Process this file with automake to produce Makefile.in

bin_PROGRAMS = xmldom-test

INCLUDES = -I../../include `xml2-config --cflags`
LIBS = `xml2-config --libs`

xmldom_test_SOURCES = \
	xmldom-test.c	\
	libxml2/xmldom-libxml2.c
