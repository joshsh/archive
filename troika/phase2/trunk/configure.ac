dnl Process this file with autoconf to produce a configure script.

AC_INIT([p2tools], [0.1], [parcour@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([api/src/main.c])

AC_CONFIG_SUBDIRS([api])

dnl The GUI subproject requires a C++ compiler.
AC_PROG_CXX

AM_INIT_AUTOMAKE
AM_CONDITIONAL(HOST__ARM_LINUX, test "$host" = arm-linux)

dnl Note: the api subdirectory is not represented here because it has its own
dnl configure.ac.
AC_OUTPUT([Makefile gui/Makefile ipk/Makefile])
