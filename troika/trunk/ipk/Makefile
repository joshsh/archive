################################################################################
## Makefile to create an ipk installation package for the Sharp Zaurus.
##
## Note: depends on macro EXEC (the executable to be placed in the data directory).

APP	=	troika

TAR	=	tar -cvz -f


all: $(APP).ipk

## Compress the whole thing up to make the installable package.
$(APP).ipk: data.tar.gz control.tar.gz debian-binary
	$(TAR) $(APP).ipk ./data.tar.gz ./control.tar.gz ./debian-binary

## Compress control into control.tar.gz.
control.tar.gz: control
	$(TAR) control.tar.gz ./control

## Compress the opt directory to data.tar.gz (this name is mandatory).
data.tar.gz: opt
	$(TAR) data.tar.gz ./opt

opt: $(EXEC) $(APP).png $(APP).desktop
	# Create opt directory structure.
	rm -rf opt/
	mkdir opt/
	mkdir opt/QtPalmTop/
	mkdir opt/QtPalmTop/apps/
	mkdir opt/QtPalmTop/apps/Games/
	mkdir opt/QtPalmTop/bin/
	mkdir opt/QtPalmTop/pics/
	
	# Fill opt directory with the appropriate data.
	cp $(APP).png opt/QtPalmTop/pics
	cp $(APP).desktop opt/QtPalmTop/apps/Games
	cp $(EXEC) opt/QtPalmTop/bin/$(APP)
