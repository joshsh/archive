## Process this file with automake to produce Makefile.in

SUBDIRS = api gui ipk

EXTRA_DIST = bootstrap cleanup

## Exclude any SVN version control directories from the distribution.
dist-hook:
	rm -rf `find $(distdir) -name .svn`
	-rm -f `find $(distdir) -name *~`
