## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src

export INCLUDES = -I$(PWD)/include/phase2
export AM_CFLAGS = -O2 -g --ansi -W -Wall
##	-fpic
##	-pedantic
##	 -Werror \
##	 -fshort-enums -fno-common \
##	 -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
##	 -D_GNU_SOURCE

include aminclude.am

$(DX_DOCDIR) :  doxygen-doc

EXTRA_DIST = include clean.sh autogen.sh m4 \
	doxygen.cfg $(DX_DOCDIR)

## Exclude any SVN version control directories from the distribution.
dist-hook:
	rm -rf `find $(distdir) -name .svn`
