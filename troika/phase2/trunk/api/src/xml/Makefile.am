## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = xmldom-test
noinst_LTLIBRARIES = libp2xml.la

INCLUDES := $(INCLUDES) `xml2-config --cflags`
##LIBS := $(LIBS) `xml2-config --libs`
LIBS := $(LIBS) -lxml2

libp2xml_la_SOURCES = \
	libxml2/xmldom-libxml2.c


## Test program ################################################################

xmldom_test_SOURCES = \
	xmldom-test.c

xmldom_test_LDADD = libp2xml.la


################################################################################

## Note: gsoap subdirectory is not included in the distribution.
EXTRA_DIST = libxml2/README
