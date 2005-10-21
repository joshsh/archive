APP	=	troika

INCPATH	=	-I /opt/Qtopia/sharp/include/
LIBS	=	-L /opt/Qtopia/sharp/lib/ -lqpe -lqte -ljpeg -luuid
CXXFLAGS=	-DQWS -fno-rtti

CXX	=	/opt/Embedix/tools/bin/arm-linux-g++
MOC	=	/opt/Qtopia/sharp/bin/moc


################################################################################
## Top-level target.

all: x86 arm


clean: x86-clean arm-clean


distclean: x86-distclean arm-distclean


################################################################################
## x86

HSRC	=	P2MainWindow.h P2CentralWidget.h
CPPSRC	=	$(APP).cpp P2MainWindow.cpp P2CentralWidget.cpp


x86: arm-clean $(APP)

$(APP): compile-target-x86 Makefile_x86
	make -f Makefile_x86

## Use macros specific to the target architecture.
compile-target-x86: compile-target-x86.h
	cp compile-target-x86.h compile-target.h

Makefile_x86: $(APP).pro
	# The Qt tutorial (#1) recommends qmake here, but the resulting Makefile
	# had some problems.
	# Note: requires environment variables defined in top-level make.sh.
	tmake $(APP).pro > Makefile_x86

$(APP).pro: $(HSRC) $(CPPSRC)
	qmake -project -o $(APP).pro


x86-clean:
	- rm -f Makefile_x86 $(APP).pro compile-target.h *.o moc_*


x86-distclean: x86-clean
	- rm -f $(APP)


################################################################################
## ARM cross-compile

arm: x86-clean $(APP)_arm

## Link.
$(APP)_arm: compile-target-arm $(APP).o P2MainWindow.o P2CentralWidget.o P2Binder.o P2MainWindow_moc.o P2CentralWidget_moc.o P2Binder_moc.o
	$(CXX) $(LIBS) -o $(APP)_arm $(APP).o P2MainWindow.o P2MainWindow_moc.o P2CentralWidget.o P2CentralWidget_moc.o P2Binder.o P2Binder_moc.o

## Use macros specific to the target architecture.
compile-target-arm: compile-target-arm.h
	cp compile-target-arm.h compile-target.h

## Compile the main class.
$(APP).o: $(APP).cpp
	$(CXX) $(INCPATH) $(CXXFLAGS) -o $(APP).o -c $(APP).cpp

## Compile the GUI classes.
P2MainWindow.o: P2MainWindow.cpp P2MainWindow.h
	$(CXX) $(INCPATH) $(CXXFLAGS) -o P2MainWindow.o -c P2MainWindow.cpp
P2CentralWidget.o: P2CentralWidget.cpp P2CentralWidget.h
	$(CXX) $(INCPATH) $(CXXFLAGS) -o P2CentralWidget.o -c P2CentralWidget.cpp
P2Binder.o: P2Binder.cpp P2Binder.h
	$(CXX) $(INCPATH) $(CXXFLAGS) -o P2Binder.o -c P2Binder.cpp

## Compile the _mocs.
P2MainWindow_moc.o: P2MainWindow_moc.cpp
	$(CXX) $(INCPATH) $(CXXFLAGS) -o P2MainWindow_moc.o -c P2MainWindow_moc.cpp
P2CentralWidget_moc.o: P2CentralWidget_moc.cpp
	$(CXX) $(INCPATH) $(CXXFLAGS) -o P2CentralWidget_moc.o -c P2CentralWidget_moc.cpp
P2Binder_moc.o: P2Binder_moc.cpp
	$(CXX) $(INCPATH) $(CXXFLAGS) -o P2Binder_moc.o -c P2Binder_moc.cpp

## Run Meta Object Compiler on header files.
P2MainWindow_moc.cpp: P2MainWindow.h
	$(MOC) -o P2MainWindow_moc.cpp P2MainWindow.h
P2CentralWidget_moc.cpp: P2CentralWidget.h
	$(MOC) -o P2CentralWidget_moc.cpp P2CentralWidget.h
P2Binder_moc.cpp: P2Binder.h
	$(MOC) -o P2Binder_moc.cpp P2Binder.h

arm-clean:
	- rm -f compile-target.h *.o *_moc.cpp


arm-distclean: arm-clean
	- rm -f $(APP)_arm
