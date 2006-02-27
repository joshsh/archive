//graphics interface utilities
//so far, only one window is allowed, upon which all graphics functions act
//initWindow() must be run once at the beginning of main().
//setColors() sets the colors of each area type together,
//  otherwise they must be set separately in main()
//interfaceSetup() must be run once before use

#include "graphics.h"

struct pixel{
  int x, y;
};

Window win;
Pixmap pix;

int backC, foreC, textC, text2C, text3C, partC; //to be set with setColors
int blackC, whiteC;  //to be initialized with findBlack()

int winwide, wintall, xcor, ycor;
int letterheight=12;

//preliminary declarations, internal includes-------------------------------

Event getNextEvent();
int nextButtonPress();
void updateEvents();
void wait();

void clearW();
void drawLargePoint(Drawable, int, int, int);
void drawLargePointOffset(Drawable, int, int, int);
void drawPixel(pixel);
void findBlack();
void framedRectangle(Drawable, int, int, int, int);
void initWindow(int, int, int, int, int, char *);
void interfaceSetup();
void setInterfaceColors(int, int, int, int, int, int);
//void photograph(int);
void redraw();

#include "areas.h"
#include "page.h"
#include "menu.h"
//#include "cbar.h"

//event handling============================================================

void updateEvents(){
  Event ue1;
  while (checkEvent()){
    ue1=nextEvent();
    if (ue1.type == Expose) redraw();
  };
}

Event getNextEvent(){
  Event gne1;
  while (checkEvent()){
    gne1=nextEvent();
    if (gne1.type == Expose) redraw();
  };
  gne1=nextEvent();
  if (gne1.type == Expose) redraw();
  return gne1;
}

void wait(){
  Event w1;
  updateEvents();
  do{
    w1=nextEvent();
    if (w1.type == Expose) redraw();
  } while (w1.type != ButtonPress );
}

int nextButtonPress(){
  Event nbp;
  updateEvents();
  do{ 
    nbp=nextEvent();
    if (nbp.type == Expose) redraw();
  } while (nbp.type != ButtonPress );
  return nbp.value;
}

//general graphics==========================================================

void clearW(){
  setColor(backC);
  fillRectangle(pix, 0, 0, winwide, wintall);
  redraw();
}

void drawLargePoint(Drawable dld, int dlx, int dly, int dscale ){
  fillRectangle(dld, dscale*dlx, dscale*dly, dscale, dscale);
}

void drawLargePointOffset(Drawable dld, int dlx, int dly, int dscale, pixel offset ){
  fillRectangle(dld, offset.x+(dscale*dlx), offset.y+(dscale*dly), dscale, dscale);
}

void drawPixel( pixel dp1 ){
  drawPoint( win, dp1.x, dp1.y );
  drawPoint( pix, dp1.x, dp1.y );
}

void findBlack(){
  int found = 0, jj;
  blackC=200;
  while (!found){
    blackC++;
    setColor(blackC);
    fillRectangle(win, 0, 0, winwide, wintall);
    jj=nextButtonPress();
    if (jj==1) found=1;
  };
  whiteC=blackC-1;
}

  //! does not conserve color
void framedRectangle(Drawable frd, int frx, int fry, int frlen, int frhgt, int flC, int frC){
  setColor(flC);
  fillRectangle(frd, frx, fry, frlen, frhgt);
  setColor(frC);
  drawRectangle(frd, frx, fry, frlen, frhgt );
}

void initWindow(int xcori, int ycori, int xpixi, int ypixi, char *title){
  winwide=xpixi; wintall=ypixi; xcor=xcori; ycor=ycori;
  win=createWindow(xcor, ycor, winwide, wintall, title);
  pix=createPixmap(win, winwide, wintall);
}

void interfaceSetup(){
  menuSetup();
}

void redraw(){
  copyArea( pix, win, 0, 0, winwide, wintall, 0, 0 );
}

void setColors(int sbackC, int sforeC, int stextC, int stext2C, int stext3C, int spartC){
  backC=sbackC; foreC=sforeC; textC=stextC;
  text2C=stext2C; text3C=stext3C; partC=spartC;
  setPageColors(backC, textC);
  setMenuColors(backC, partC, textC);
  //setCBarColors(...)
}

/*void photograph( int predfac ){
  comment( "See command tool." );
  char *newname;
  system( "clear" );
  cout << "Save as... "; cin >> newname; cout << endl;
  cout << "Transforming data..." ) << endl;
  comment( "Transforming data... " );
  
  fstream out;
  out.open( newname, ios::out );
  if ( !out.good ){
    resetWindow();
    printLine( "Error: bad file name." );
    comment( "Error: bad file name. " );
    return;
  };

  comment( "Save complete. " );
  system( "clear" );
}*/

