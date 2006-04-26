## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src

export INCLUDES = -I$(PWD)/include
export AM_CFLAGS = -O2 -g --ansi -W -Wall \
	-fpic  ## Create position-independent code for the shared library.
##	-pedantic
##	 -Werror \
##	 -fshort-enums -fno-common \
##	 -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
##	 -D_GNU_SOURCE

## include aminclude.am

EXTRA_DIST = include clean.sh autogen.sh m4 doxygen.cfg
##EXTRA_DIST = autogen.sh $(DX_CONFIG) doc/html

## Exclude any SVN version control directories from the distribution.
dist-hook:
	rm -rf `find $(distdir) -name .svn`
