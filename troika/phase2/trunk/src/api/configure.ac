dnl Process this file with autoconf to produce a configure script.

AC_INIT([phase2], [0.6.4], [parcour@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/main.c])

AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL

dnl Doxygen stuff.
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(ON)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([phase2], [doxygen.cfg], [doc])

AC_OUTPUT([Makefile src/Makefile src/import/Makefile src/parser/Makefile src/util/Makefile src/xml/Makefile])
