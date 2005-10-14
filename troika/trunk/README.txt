Embedix Plus PDA v. 2.0
Qtopia for Embedix Plus PDA v. 1.5.4
Linux kernel 2.4.18-rmk7-pxa3-embedix
Opera browser v. 6.0

developer.sharpsec.com

[*] A CF card slot allows for an 802.11 or Bluetooth wireless networking card
[*] The XScale CPU lacks a floating-point unit so it's not going to be much of a number cruncher
[R] port of micro-emacs -[*]- runs in the terminal window, uses a fixed space font
[-] media player is lousy
[-] out of the box, the cradle can only synch with Windows
[-] no spell checker or word processor

[!] download terminal, Sharp's expanded File Manager

################################################################################

http://www.kevinboone.com/myfirstqtopiaapp.html

[*] ipk file is a compressed archive containing three files [U]:
--[.] debian-binary -- identifies this ipk to the installer (single line of text)
--[.] data.tar.gz -- contains the executable, configuration files, icons, data, etc.
--[.] control.tar.gz -- contains a description of the application, along with any pre-installation and post-installation shell scripts that are required to configure it.

[*] the RPMs install their software in /opt/Embedix on the PC

Qtopia is a graphical interface and program manager for small Linux-based devices. The user interface part of Qtopia is based on a user interface library called Qt-embedded, which is the small-systems version of the Qt library

Native Qtopia applications must be written in C++, not C (or, at least, there is no non-trivial way to write them in C)

/opt/Qtopia -- the Qtopia system itself is here

    /bin -- executables
    /lib -- dynamically-linked libraries
    /apps -- launcher configuration files
    /etc -- data and configuration files

/mnt/card/opt/Qtopia -- on the Sharp Zaurus, each memory card will have the same basic directory structure, mirroring the main /opt/Qtopia

[*] /System (within host computer) = /media/System (within Zaurus)

################################################################################

[hello1.cpp] #include  int main(int argc, char **argv) { cout << "Hello, world!\n"; }

[UUP]
--[P] Compile this source to object code as follows:
      /opt/Embedix/tools/bin/arm-linux-g++ -o hello1.o -c hello1.cpp
--[P] Then link this with the standard libraries to produce an executable:
      /opt/Embedix/tools/bin/arm-linux-g++ -o hello1 hello1.o
--[P] % file hello1
      hello1: ELF 32-bit LSB executable, ARM, version 1 (ARM), 
      for GNU/Linux 2.0.0, dynamically linked (uses shared libs), not stripped
--[P] /opt/Embedix/tools/bin/arm-linux-strip hello1
--[P] get the hello1 program onto the [device] for testing
----[$P] copy files onto a local web server, use wget utility from the PMA's command line
--[P] # chmod 755 /media/System/hello1
      # /media/System/hello
      Hello, World!

################################################################################

[hello2.cpp]
#include <qmainwindow.h>
#include <qpe/qpeapplication.h>
int main(int argc, char** argv) {
  QPEApplication app(argc, argv);
  QMainWindow* mainWindow = new QMainWindow();
  mainWindow->setCaption("Hello, World!");
  app.showMainWidget(mainWindow);
  return app.exec();
}

[UUP]
--[P] Compile
      /opt/Embedix/tools/bin/arm-linux-g++ \
      -I /opt/Qtopia/sharp/include/ \
      -DQWS -fno-rtti -o hello2.o -c hello2.cpp
--[P] Link
      /opt/Embedix/tools/bin/arm-linux-g++ \
      -L /opt/Qtopia/sharp/lib/ -o hello2  hello2.o \
      -lqte -lqpe -ljpeg -luuid

################################################################################

[FrmMain.h]
#include 
class FrmMain : public QMainWindow {
    Q_OBJECT
public:
    FrmMain(QWidget* parent=0, const char* name=0, WFlags fl=0);
public slots:
    void    cmdFileQuit();
};

[FrmMain.cpp]
#include <qpopupmenu.h>
#include <qmenubar.h>
#include "FrmMain.h"
FrmMain::FrmMain(QWidget* parent, const char* name, WFlags fl)
    : QMainWindow(parent, name, fl) {
    setCaption("Hello, World!");
    QMenuBar *menubar = this->menuBar();
    QPopupMenu *mnuFile = new QPopupMenu(this, "MenuFile");
    menubar->insertItem("&File", mnuFile);
    mnuFile->insertItem("&Quit", this, SLOT(cmdFileQuit()), 0, 1);
}
void FrmMain::cmdFileQuit() {
    this->close();
}

[hello3.cpp]
#include <qmainwindow.h>
#include <qpe/qpeapplication.h>
#include "FrmMain.h"
int main(int argc, char** argv) {
    QPEApplication app(argc, argv);
    QMainWindow* mainWindow = new FrmMain();
    app.showMainWidget(mainWindow);
    return app.exec();
}

[UUP]
--[P] Compile the main window class
      /opt/Embedix/tools/bin/arm-linux-g++ \
      -I /opt/Qtopia/sharp/include/ \
      -DQWS -fno-rtti -o FrmMain.o -c FrmMain.cpp
--[P] Compile the main class
      /opt/Embedix/tools/bin/arm-linux-g++ \
      -I /opt/Qtopia/sharp/include/ \
      -DQWS -fno-rtti -o hello3.o -c hello3.cpp
--[P] Run moc (Meta Object Compiler) on FrmMain.h --> FrmMain_moc.cpp
      /opt/Qtopia/sharp/bin/moc -o FrmMain_moc.cpp FrmMain.h
--[P] Compile FrmMain_moc.cpp
      /opt/Embedix/tools/bin/arm-linux-g++ \
      -I /opt/Qtopia/sharp/include/ -DQWS -fno-rtti \
      -o FrmMain_moc.o -c FrmMain_moc.cpp
--[P] Link
      /opt/Embedix/tools/bin/arm-linux-g++ \
      -L /opt/Qtopia/sharp/lib/ -o hello3  \
      hello3.o FrmMain.o FrmMain_moc.o \
      -lqpe -lqte -ljpeg -luuid

[*] More complex applications are just a matter of more advanced QTopia programming.

################################################################################

[control]
package: pma_test
Installed-Size: 24000
Filename: ./pma_test.ipk
Version: 0.01
Depends:
Priority: optional
Section: qpe/utilities
Maintainer: your name here
Architecture: arm
Description: Test application

[hello3.desktop]
[Desktop Entry]
Comment=PMA430 test app
Exec=hello3
Icon=hello3
Type=Application
Name=Test app

[debian-binary]
2.0

[UUP]
--[P] Compress this file into control.tar.gz
      tar cvfz control.tar.gz ./control
--[P] In an empty directory...
      mkdir -p opt/QtPalmtop/bin
      mkdir -p opt/QtPalmtop/apps/Applications
      mkdir -p opt/QtPalmtop/pics
--[P] Copy your executable into opt/QtPalmtop/bin
--[P] Copy the icon file hello3.png into ./opt/QtPalmtop/pics
--[P] Copy hello3.desktop into ./opt/QtPalmtop/apps/Applications
--[*]
    opt
      QtPalmtop
        pics
          hello3.png
        apps
          Applications
            hello3.desktop
        bin
          hello3
--[P] Compress this directory structure into data.tar.gz (this name is mandatory)
      tar cfvz data.tar.gz ./opt
--[P] Compress the whole thing up to make the installable package.
      tar cvfz hello3.ipk ./data.tar.gz ./control.tar.gz ./debian-binary
--[P] Copy the .ipk file into the Documents directory of the PMA430
--[P] Start the `Package manager' application from the Settings tab


[On the Zaurus] ################################################################

[*] debian-binary is optional

hellozaurus
    home
        QtPalmtop
            apps
                Applications
                Games
                [...]
            bin
            java
            pics

[UUP] Install
# Transfer the .ipk file to the /home/root/Documents/apps/ipkg directory on the Zaurus.
# Select the Add/Remove Software item from the Settings tab.
# Click on Install Packages. You will see HelloZaurus listed. Click on it to install it.
# Exit from the Package Installer and exit from Add/Remove Software.

[U] Optional siblings of "control":
    * preinst - Pre-installation (before the actual program files are extracted).
    * postinst - Post-installation (after the program files have been extracted).
    * prerm - Pre-uninstallation (before your files are removed).
    * postrm - Post-uninstallation (after your files have been removed).
    * conffiles - List of configuration files not to be overwritten during an upgrade.


################################################################################
# Environment variables needed by the build.

export TMAKEPATH=$TMAKEDIR/lib/linux-g++
export QTDIR=$QT2DIR
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH


################################################################################
# Subversion


# Remove the old repository.
rm -rf ~/mystuff/dev/svn

# Create a new FSFS SVN repository.
svnadmin create ~/mystuff/dev/svn --fs-type fsfs

# Import the project into the repository.
svn import ~/mystuff/dev/troika/ \
    file:///home/joshs/mystuff/dev/svn/troika/trunk \
    -m "Initial import of the 'Troika' Phase2 interface."

# Remove the outdated working copy.
cd ~/mystuff/dev/troika
rm -rf trunk

# Get a new working copy.
svn checkout file:///home/joshs/mystuff/dev/svn/troika/trunk/


# Get info.
svn list file:///home/joshs/mystuff/dev/svn/
svnlook tree ~/mystuff/dev/svn/

cd ~/mystuff/dev/troika/trunk/
svn status

# Commit the current working copy.
cd ~/mystuff/dev/troika/trunk/
svn commit
