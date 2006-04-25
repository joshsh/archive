dnl Process this file with autoconf to produce a configure script.
AC_INIT(phase2, 0.6, parcour@gmail.com)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/main.c)

AM_INIT_AUTOMAKE

dnl Enables C compiler support.
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC

dnl Doxygen stuff.
dnl DX_HTML_FEATURE(ON)
dnl DX_CHM_FEATURE(OFF)
dnl DX_CHI_FEATURE(OFF)
dnl DX_MAN_FEATURE(OFF)
dnl DX_RTF_FEATURE(OFF)
dnl DX_XML_FEATURE(OFF)
dnl DX_PDF_FEATURE(OFF)
dnl DX_PS_FEATURE(OFF)
dnl DX_INIT_DOXYGEN(foobar, doxygen.cfg, doc)

AC_OUTPUT(Makefile src/Makefile src/parser/Makefile)
