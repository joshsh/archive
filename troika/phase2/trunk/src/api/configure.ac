dnl Process this file with autoconf to produce a configure script.

AC_INIT([phase2], [0.6.4], [parcour@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/main.c])

AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL

dnl Is this necessary?
AC_PROG_RANLIB

dnl Doxygen stuff.
dnl DX_HTML_FEATURE(ON)
dnl DX_CHM_FEATURE(OFF)
dnl DX_CHI_FEATURE(OFF)
dnl DX_MAN_FEATURE(OFF)
dnl DX_RTF_FEATURE(OFF)
dnl DX_XML_FEATURE(OFF)
dnl DX_PDF_FEATURE(OFF)
dnl DX_PS_FEATURE(OFF)
dnl DX_INIT_DOXYGEN([phase2], [doxygen.cfg], [doc])

AC_OUTPUT([Makefile src/Makefile src/import/Makefile src/parser/Makefile src/util/Makefile src/xml/Makefile])
