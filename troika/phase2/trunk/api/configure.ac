dnl Process this file with autoconf to produce a configure script.

AC_INIT([phase2], [0.7.1], [parcour@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/main.c])

AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL

dnl The maintainer needs xsltproc, but the user doesn't...
dnl AC_PATH_PROG(XSLTPROC, xsltproc, no)
dnl if test "$XSLTPROC" = "no"; then
dnl    AC_MSG_ERROR([You need xsltproc to build Phase2 from scratch])
dnl fi

AC_ARG_ENABLE(debug,
[  --enable-debug    Turn on debugging],
[case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

dnl The maintainer needs doxygen as well.
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(ON)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([phase2], [doxygen.cfg], [doc])

AC_OUTPUT([Makefile include/Makefile src/Makefile src/import/Makefile src/parser/Makefile src/collection/Makefile src/xml/Makefile])
