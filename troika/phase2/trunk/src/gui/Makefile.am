## Process this file with automake to produce Makefile.in

SUBDIRS = project

EXTRA_DIST = README TODO

BUILT_SOURCES = src/p2gui.pro \
		src/Makefile

noinst_HEADERS = revision.h		\
	settings.h			\
	Object-impl.h			\
	Primitive-impl.h		\
	compiler/Compiler-impl.h	\
	compiler/license.h		\
	environment/Environment-impl.h

noinst_PROGRAMS = hello

hello_SOURCES = test.cpp

# Using x86 qmake.
qmake -project -o p2gui.pro
	echo "QMAKE_CXXFLAGS += -Wno-deprecated" >> p2gui.pro
	echo "INCPATH += /usr/local/include/phase2" >> p2gui.pro
	echo "LIBS += -lphase2 -lxml2" >> p2gui.pro
	echo "QMAKE_INSTALL_FILE = p2gui" >> p2gui.pro
#	echo "INSTALLS += p2gui" >> p2gui.pro

qmake

make