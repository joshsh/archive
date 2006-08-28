## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libp2parser.la

##LIBS := $(LIBS) -lncurses

BUILT_SOURCES = parser.h

EXTRA_DIST = $(BUILT_SOURCES) README

AM_YFLAGS = -d
AM_LFLAGS = --header=header.h \
	-p  # Get comments regarding features that lead to performance losses.

libp2parser_la_SOURCES = \
	lexer.l		\
	parser.y
