APP	=	troika

INCPATH	=	-I /opt/Qtopia/sharp/include/
LIBS	=	-L /opt/Qtopia/sharp/lib/ -lqpe -lqte -ljpeg -luuid

CXX	=	/opt/Embedix/tools/bin/arm-linux-g++
MOC	=	/opt/Qtopia/sharp/bin/moc


################################################################################
## Top-level target.

all: x86 arm


################################################################################
## x86

CPPFILES	=	P2MainWindow.cpp P2MainWindow.h $(APP).cpp

x86: $(APP)

$(APP): Makefile_x86
	
	make -f Makefile_x86

Makefile_x86: $(APP).pro
	# The Qt tutorial (#1) recommends qmake here, but the resulting Makefile
	# had some problems.
	# Note: requires environment variables defined in top-level make.sh.
	tmake $(APP).pro > Makefile_x86


$(APP).pro: $(CPPFILES)
	qmake -project -o $(APP).pro


################################################################################
## arm

arm: $(APP)_arm

$(APP)_arm:
	# Compile the main window class.
	$(CXX) $(INCPATH) -DQWS -fno-rtti -o P2MainWindow.o -c P2MainWindow.cpp
	
	# Compile the main class.
	$(CXX) $(INCPATH) -DQWS -fno-rtti -o $(APP).o -c $(APP).cpp
	
	# Run moc (Meta Object Compiler) on FrmMain.h --> FrmMain_moc.cpp.
	$(MOC) -o P2MainWindow_moc.cpp P2MainWindow.h
	
	# Compile P2MainWindow_moc.cpp.
	$(CXX) $(INCPATH) -DQWS -fno-rtti -o P2MainWindow_moc.o -c P2MainWindow_moc.cpp
	
	# Link.
	$(CXX) $(LIBS) -o $(APP)_arm $(APP).o P2MainWindow.o P2MainWindow_moc.o
