## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src

if DEBUG
DEBUG_FLAGS = -g -p
endif

export AM_CFLAGS = -O2 --ansi -W -Wall $(DEBUG_FLAGS) -I$(PWD)/include/phase2
##	-fpic
##	-pedantic
##	 -Werror \
##	 -fshort-enums -fno-common \
##	 -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
##	 -D_GNU_SOURCE


include aminclude.am

$(DX_DOCDIR) :  doxygen-doc

EXTRA_DIST = include bootstrap cleanup m4 aminclude.am versionfilter.sh \
	AUTHORS COPYING INSTALL NEWS README TODO \
	doxygen.cfg $(DX_DOCDIR)

## Exclude any SVN version control directories from the distribution.
dist-hook:
	rm -rf `find $(distdir) -name .svn`
	rm -f `find $(distdir) -name \*~`
