## Process this file with automake to produce Makefile.in

EXTRA_DIST = project README TODO

## Note: the moc_ files are also built sources.
BUILT_SOURCES = project/p2gui.pro \
		project/Makefile

bin_PROGRAMS = p2gui

## Prevent automake from expecting an implicit p2gui.c.
p2gui_SOURCES =

project/p2gui.pro :
	qmake -project -o project/p2gui.pro
	echo "QMAKE_CXXFLAGS += -Wno-deprecated" >> project/p2gui.pro
	echo "INCPATH += /usr/local/include/phase2" >> project/p2gui.pro
	echo "LIBS += -lphase2 -lxml2" >> project/p2gui.pro
##	echo "QMAKE_INSTALL_FILE = p2gui" >> project/p2gui.pro
##	echo "INSTALLS += p2gui" >> project/p2gui.pro
	echo $(target) > target.h

project/Makefile : project/p2gui.pro
	qmake project/p2gui.pro -o project/Makefile

p2gui : project/Makefile
	$(MAKE) -C project
	mv project/p2gui .

## Exclude object files from the distribution.
dist-hook:
	-rm -f `find $(distdir) -name *.o`

## Note: apparently the qmake-generated Makefile does not have a target to
## clean up object files but NOT moc_ files.
clean-local:
	-rm -f project/*.o
