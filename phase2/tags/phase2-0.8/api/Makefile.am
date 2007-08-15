## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src

if DEBUG
DEBUG_FLAGS = -g -p
endif

export AM_CFLAGS = -O2 --ansi -W -Wall $(DEBUG_FLAGS) -I$(PWD)/include
##	-fpic
##	-pedantic
##	 -Werror \
##	 -fshort-enums -fno-common \
##	 -Wcast-align -Wcast-qual -Wmissing-prototypes -Wpointer-arith -Wshadow -Wunused -Wwrite-strings \
##	 -D_GNU_SOURCE

include aminclude.am

$(DX_DOCDIR) :  doxygen-doc

## Not included: NEWS test examples
EXTRA_DIST =								\
	include m4 aminclude.am doxygen.cfg $(DX_DOCDIR)		\
	bootstrap.sh changelog.sh cleanup.sh versionfilter.sh		\
	AUTHORS ChangeLog COPYING INSTALL NOTES README TODO

## Exclude any SVN version control directories from the distribution.
rm-backup:
	rm -rf `find $(distdir) -name .svn`
	rm -f `find $(distdir) -name \*~`

## If possible, generate a new ChangeLog.
changes:
	./changelog.sh

dist-hook: rm-backup changes

