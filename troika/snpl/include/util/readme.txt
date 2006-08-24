NOTE:
     Apart from SNPL.h/.cpp, the contents of include/util are the only libraries
for which I've split header files from body files.  This is because I don't
expect any of the others to serve a purpose outside of the SNPL kernel, whereas
the utility libraries are more generally useful.  For instance, SParse uses the
SNPL_lookup_table class for its runtime dictionary.

ANOTHER NOTE:
     I'm not so sure strutil.h belongs here.
