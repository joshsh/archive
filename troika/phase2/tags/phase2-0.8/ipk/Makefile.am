## Process this file with automake to produce Makefile.in

## Creates an ipk installation package for the Sharp Zaurus.
## Note: depends on macro EXEC (the executable to be placed in the data directory).

TAR	= tar -cvz -f
EXEC	= ../api/src/.libs/phase2


noinst_PROGRAMS = phase2.ipk

phase2_ipk_SOURCES = \
	control \
	debian-binary \
	phase2.desktop \
	phase2.png

BUILT_SOURCES = \
	control.tar.gz	\
	data.tar.gz	\
	opt


## Compress the whole thing to make the installable package.
phase2.ipk: data.tar.gz control.tar.gz debian-binary
	$(TAR) phase2.ipk ./data.tar.gz ./control.tar.gz ./debian-binary

## Compress control into control.tar.gz.
control.tar.gz: control
	$(TAR) control.tar.gz control

## Compress the opt directory to data.tar.gz (this name is mandatory).
data.tar.gz: opt
	$(TAR) data.tar.gz ./opt

opt: $(EXEC) phase2.png phase2.desktop
	## Create opt directory structure.
	- rm -rf opt/
	mkdir -p opt/QtPalmTop/apps/Games/
	mkdir -p opt/QtPalmTop/bin/
	mkdir -p opt/QtPalmTop/pics/
	## Fill opt directory with the appropriate data.
	cp phase2.png opt/QtPalmTop/pics
	cp phase2.desktop opt/QtPalmTop/apps/Games
	cp $(EXEC) opt/QtPalmTop/bin/phase2

clean-local:
	-rm -f control.tar.gz data.tar.gz opt
