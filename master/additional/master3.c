//note: from now on, objects will be drawn only on the pixmap pix and not on
//the window as well, to minimize server requests.  After drawing a set of
//objects, redrawScreen() will have to be used to transfer them to to screen. 

#include <math.h>
//#include "math+.h"
#include "complex.h"
//#include "files.h"
#include "interface.h"
#include "vector2.h"
#include "vector3.h"

int i, j, k, l, m, n;
int up=1, right=2, down=3, left=4;
int intA, intB, intC;
int mode; //for general use
double realA, realB, realC, realD;

struct int3{
  int x, y, z;
};

int3 axisC;

//mathematical/geometrical constants and variables------------------------

int tdivs;

double x, y, Z, t, xl, yl, zl, tl;

double xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax, xmid, ymid, zmid, tmid;
double xspan, yspan, zspan, tspan, xinc, yinc, tinc;
double dx, dy, dz, dt;

//interface components----------------------------------------------------

int const menucols=4;
int const menurows=5;
int const panelcols=4;
int const panelrows=4;
int const pagelines=35;

int const xpix=500;
int const ypix=500;
int xs0=xpix/2, ys0=ypix/2;
int xsmid=xpix/2, ysmid=ypix/2;
int textheight=20;

int commentheight=20; //height of comment box
int commentlength;  //length of currently displayed comment string
int pmenuheight=panelrows*textheight;
int panelheight=pmenuheight+commentheight;
int menuheight=200;
int linelist[pagelines];
int pagelength=400, pageheight=ypix+panelheight-menuheight;
int wide=xpix+pagelength;
int tall=ypix+panelheight;
int textwidth=xpix/panelcols;

  //area name constants
int thaP, thaM, thbP, thbM, lconstP, lconstM, xP, xM, yP, yM;

  //area group constants
int general, panelmenu, leftmenu, linemenu, rotcontrols, tracearrows;

int mpage;

pixel pagecor;
pixel generalcor;
pixel panelcor;

pixel yesB, noB, contB;
pixel magB, cconstB, distB, repB, iterB; //for general use.
pixel boringB;  //with coordinates (0, 0)

//data fields-------------------------------------------------------------

int setmatrix[xpix][ypix];
int setmatrix2[xpix][ypix];
double field[xpix][ypix];

//coloring----------------------------------------------------------------

int coloring;
int byiter=1, bymag=2, byrep=3, colconst=4, bydist=5;
int objectC=100, stopC=10, boxC=55, axesC=50;
int highC=180, lowC=1, colspan3D=highC-lowC, cconst3D;
int numcols=256;  //if numcols isn't a perfect square, getNum() must be changed

//------------------------------------------------------------------------

//rand48.h
#ifndef	__random_h_info_I
#ifdef __cplusplus
extern "C" {
#endif
#define	__random_h_info_I
extern void srand48( long seedval );
extern double drand48( void );
#endif /* defined(__cplusplus) */
#ifdef __cplusplus
}
#endif

//graphing, transformations, event handling=================================

pixel mpixel( int mp1, int mp2 ){
  pixel mp3;
  mp3.x=mp1;
  mp3.y=mp2;
  return mp3;
}

point2 pixelPoint( pixel pp1 ){
  point2 pp2;
  pp2.x=xmin+(xinc*(pp1.x+.5));
  pp2.y=ymax-(yinc*(pp1.y+.5));
  return pp2;
}

point2 eventPoint( Event e ){
  point2 ep2;
  ep2.x=xmin+(xinc*(e.x+.5));
  ep2.y=ymax-(yinc*(e.y+.5));
  return ep2;  
}

pixel pointPixel( point2 pp1 ){
  pixel pp2;
  pp2.x=int((pp1.x-xmin)/xinc);
  pp2.y=int((ymax-pp1.y)/yinc);
  return pp2;
}

int inWindow( point2 iw1 ){
  if (iw1.x < xmin) return 0;
  if (iw1.x > xmax) return 0;
  if (iw1.y < ymin) return 0;
  if (iw1.y > ymax) return 0;
  return 1;
}
 
int inWindow3D( point3 iw1 ){
  if (iw1.x < xmin) return 0;
  if (iw1.x > xmax) return 0;
  if (iw1.y < ymin) return 0;
  if (iw1.y > ymax) return 0;
  if (iw1.z < zmin) return 0;
  if (iw1.z > zmax) return 0;
  return 1;
}

void rrangeconst2D(){
  xspan=xmax-xmin;
  yspan=ymax-ymin;
  xinc=xspan/xpix;
  yinc=yspan/ypix;
  xmid=(xmax+xmin)/2;
  ymid=(ymax+ymin)/2;
}

void rrangeconst3D(){
  xspan=xmax-xmin;
  yspan=ymax-ymin;
  xmid=(xmin+xmax)/2;
  ymid=(ymin+ymax)/2;
  zspan=(xspan+yspan)/2;
  zmin=zmid-(zspan/2);
  zmax=zmid+(zspan/2);
}

void rrange2D(){
  xmin=inputNumber( "x minimum = " );
  xmax=inputNumber( "x maximum = " );
  ymin=inputNumber( "y minimum = " );
  ymax=inputNumber( "y maximum = " );
  rrangeconst2D();
}

void rrange3D(){
  xmin=inputNumber( "x minimum = " );
  xmax=inputNumber( "x maximum = " );
  ymin=inputNumber( "y minimum = " );
  ymax=inputNumber( "y maximum = " );
  zmid=inputNumber( "middle z value = " );
  rrangeconst3D();
}

void trange(){
  tmin=inputNumber( "t minimum = " );
  tmax=inputNumber( "t maximum = " );
  tdivs=inputInt( "divisions = " );
  tspan=tmax-tmin;
  tinc=tspan/(tdivs*1.0);
  tmid=(tmax+tmin)/2;
}

void unitRange(int dimen){
  if (dimen == 2 ){
    xmin=-1; xmax=1; ymin=-1; ymax=1; 
    rrangeconst2D();
  };
  if (dimen == 3){
    xmin=-1; xmax=1; ymin=-1; ymax=1;
    zmid=0;
    rrangeconst3D();
  };
}

pixel readPoint(){
  Event rp1;
  pixel rp2;
  //while ( checkEvent() ) rp1=nextEvent();
  //rp1=nextEvent();
  updateEvents();
  rp1=nextEvent();
  if (rp1.type == Expose) redraw();
  while ((rp1.type != ButtonPress)||(rp1.value != 1)){
    rp1=nextEvent();
    if (rp1.type == Expose) redraw();
  }; 
  rp2.x=rp1.x;
  rp2.y=rp1.y;
  return rp2;
}

point2 getPoint2(){
  point2 gp1;
  gp1.x=inputNumber( "x = " );
  gp1.y=inputNumber( "y = " );
  return gp1;
}

point3 getPoint3(){
  point3 gp1;
  gp1.x=inputNumber( "x = " );
  gp1.y=inputNumber( "y = " );
  gp1.z=inputNumber( "z = " );
  return gp1;
}

void alarm( int alarmcolor ){
  int lastC=getColor();
  setColor(alarmcolor);
  fillRectangle(win, 0, 0, wide, tall);
  wait();
  setColor(lastC);
  redraw();
}

//graphics interface=====================================================

 //note: only clears the graph in pix
void clear(){
  setColor(backC);
  fillRectangle(pix, 0, 0, xpix, ypix);  
}

 // get rid of this stupid thing!
void resetPixels(){
 magB=boringB;
 cconstB=boringB;
 distB=boringB;
 repB=boringB;
 iterB=boringB;
}

void drawPanel(){
  copyArea( pix, win, 0, ypix, xpix, panelheight, 0, ypix);
}

void emptyPanel(){
  int lastC=getColor();
  setColor( backC );
  fillRectangle( pix, 0, ypix, xpix, panelheight);
  setColor( lastC );
}

void clearComment(){
  setColor( backC );
  fillRectangle(win, 2, ypix+1, xpix-3, commentheight-2 );
  fillRectangle(pix, 2, ypix+1, xpix-3, commentheight-2 );
}

void comment( char *com1 ){
  int lastC = getColor();
  clearComment();
  setColor( backC-1 );
  drawString(win, 5, ypix+letterheight, com1 );
  drawString(pix, 5, ypix+letterheight, com1 );
  commentlength=strlen( com1 );
  setColor( lastC );
}

void addComment( char *com1){
  int lastC = getColor();
  setColor( backC - 1 );
  drawString(win, 5+(commentlength*lwidth), ypix+letterheight, com1 );
  drawString( pix, 5+(commentlength*lwidth), ypix+letterheight, com1 );
  //drawString( panel, 5+(commentlength*lwidth), letterheight, com1 );
  commentlength = commentlength + strlen( com1 );
  setColor( lastC );
}

void writeYesNo(){
  setColor( textC );
  putMenu( yesB, "yes" );
  putMenu( noB, "no" );
}

void redrawScreen(){
  copyArea( pix, win, 0, 0, xpix, ypix, 0, 0 );
}

int findColor(){
  //starts with color 1, ends with color numcols.
  //findColor assumes that xpix==ypix
  int returncolor;
  int colside=int(sqrt( numcols ));
  //cout << endl << "colside = " << colside << endl;  //!
  double sqside=xpix/(colside*1.0);
  for (i=1; i <= colside; i++){
    for (j=1; j <= colside; j++){
      setColor( ((j-1)*colside)+i );
      fillRectangle(win, int((i-1)*sqside), int((j-1)*sqside), int(sqside+1), int(sqside+1));
    };
  };
  Event e;
  while ( checkEvent() ) e=nextEvent();
  e=nextEvent();
  while ((e.type != ButtonPress)||(e.value != 1)) e=nextEvent();
  returncolor=int(((e.y/(sqside*1.0))*colside)+((e.x/(sqside*1.0))+1));
  redraw();
  return returncolor;
}

void clearGraph(){
  int lastC = getColor();
  setColor(backC);
  fillRectangle(win, 0, 0, xpix, ypix);
  fillRectangle(pix, 0, 0, xpix, ypix);
  setColor( lastC );
}

/*
void clearField(){
  comment("Resetting projection matrix..." );
  for (i=0; i<xpix; i++){
    for (j=0; j<ypix; j++) field[i][j]=0;
  };
  comment("done.");
  }*/

/*
void triangle(Drawable d, int tp1x, int tp1y, int tp2x, int tp2y, int tp3x, int tp3y){
  drawLine(d, tp1x, tp1y, tp2x, tp2y);
  drawLine(d, tp2x, tp2y, tp3x, tp3y);
  drawLine(d, tp1x, tp1y, tp3x, tp3y);
}

void arrow(Drawable d, int xtip, int ytip, int length, int direct){
  int length2=length/2;
  if ((direct < 1)||(direct > 4)) return;
  if (direct == up){
    drawLine(d, xtip, ytip, xtip-length2, ytip+length);
    drawLine(d, xtip, ytip, xtip+length2, ytip+length);
    drawLine(d, xtip-length2, ytip+length, xtip+length2, ytip+length);
  };
  if (direct == right){
    drawLine(d, xtip, ytip, xtip-length, ytip-length2);
    drawLine(d, xtip, ytip, xtip-length, ytip+length2);
    drawLine(d, xtip-length, ytip-length2, xtip-length, ytip+length2);
  };
  if (direct == down){
    drawLine(d, xtip, ytip, xtip-length2, ytip+length);
    drawLine(d, xtip, ytip, xtip+length2, ytip+length);
    drawLine(d, xtip-length2, ytip+length, xtip+length2, ytip+length);
  };
  if (direct == left){
    drawLine(d, xtip, ytip, xtip+length, ytip-length2);
    drawLine(d, xtip, ytip, xtip+length, ytip+length2);
    drawLine(d, xtip+length, ytip-length2, xtip+length, ytip+length2);
  };
  }*/

int getYesNo( char *gyn1 ){
  printLine( gyn1 );
  clearMenu();
  writeYesNo();
  int box = getChoice( general );
  if (menuNumber( yesB ) == box) return 1;
  else return 0;
}

void resetAll(){
  int thismenu=getMenu();
  int thispage=getPage();
  setMenu(general); clearMenu();
  setMenu(panelmenu); clearMenu();
  setPage(mpage); resetPage();
  redraw();
  setMenu(thismenu);
  setPage(thispage);
}

  //setMenu() must be used BEFORE using this function
int checkStop(){
  Event e;
  while ( checkEvent() ){
    e=nextEvent();
    if (e.type == Expose) redraw();
    if ((e.type == ButtonPress)&&(e.value == 1)){
      if (stopM(areaName( panelmenu, e.x, e.y ))) return 1;
    };
  };
  if (e.type == Expose) redraw();
  if ((e.type == ButtonPress)&&(e.value == 1)){
    if (stopM(areaName( panelmenu, e.x, e.y ))) return 1;
  };
  return 0;
}

 //still has a bug-- it freezes when it comes to the end page of the file.
int chooseLine( char* glFile ){ //returns a chosen file line. The top line is 0.
  char ch;
  int box, linefeeds=0, pageflips;
  fstream fileA;
  fileA.open( glFile, ios::in );
  int numlines=countLines( glFile );
  
  setPage(mpage);
  resetPage();
  
  Event e;
  resetPage();
  setColor(textC);
  if (numlines <= pagelines){
    ch=fileA.get();
    while (!fileA.eof()){
      if (ch > 31) printString(charStr(ch));
      if (ch == 10) newLine();
      ch = fileA.get();
    };
    redraw();
    box = getChoice(linemenu);
    for (i=0; i<pagelines; i++){
      if (linelist[i]==box) return i;
    };
  }
  else{
    ch=fileA.get();
    pageflips=0;
    while (!fileA.eof()){
      linefeeds=0;
      while(( linefeeds < pagelines )&&(!fileA.eof())){
        if (ch > 31) printString(charStr(ch));
        if (ch == 10){ newLine(); linefeeds++; };
        ch = fileA.get();
      };
      redraw();
      updateEvents();
      e=nextEvent();
      while (e.type !=ButtonPress){
        if (e.type == Expose) redraw();
        e=nextEvent();
      };
      if (e.value==1){
        box=areaName(linemenu, e.x, e.y);
        for (i=0; i<pagelines; i++){
          if (linelist[i]==box) return i+(pagelines*pageflips);
        };  
      };
      pageflips++;
      resetPage();
      //setColor( partC );
      //pix=drawAreas( pix, linemenu, 0, 0 );
    };
    box = getChoice(linemenu);
    for (i=0; i<pagelines; i++){
      if (linelist[i]==box) return i+(pagelines*(pageflips-1));
    };  
  };  
}  

void waitGeneral(){
  setMenu(general);
  Event e=getNextEvent();
  while (getChoice(general) != menuNumber(contB) ){
    clearMenu();
    setColor(textC);
    putMenu( contB, "continue" );
  };
}

void roam(double txmin, double txmax, double tymin, double tymax, char* c1, char* c2 ){
  double lxmin=xmin, lxmax=xmax, lymin=ymin, lymax=ymax;
  xmin=txmin; xmax=txmax; ymin=tymin; ymax=tymax;
  rrangeconst2D();
  point2 p;
  Event e=getNextEvent();
  writeStopM();
  setPage(mpage);
  while (!checkStop()){
    e=nextEvent();
    if (e.type == Expose) redraw();
    if (e.type == MotionNotify){
      p=eventPoint(e);
      resetPage();
      printString( c1 ); printString( " = " ); printNumber(p.x); newLine();
      printString( c2 ); printString( " = " ); printNumber(p.y);
    };
  };
  resetPage();
  xmin=lxmin; xmax=lxmax; ymin=lymin; ymax=lymax;
  rrangeconst2D();
}

void inputSystem(){
  setPage(mpage);
  resetPage();
  setColor( textC );
  printString("Decimal digits displayed = "); printNumber(disdigits); newLine();
  disdigits=-1;
  while (disdigits < 0){
    disdigits=inputInt( "new value = " );
    newLine();
    if (disdigits < 0) printLine( "That is ridiculous. Try again. " );
  };
  resetPage();
}

void randomize(){
  Event e;
  int thissum=0;
  for (i=1; i < 5; i++){
    e=nextEvent();
    if (e.type == Expose) redraw();
    thissum=thissum+e.x+(e.y*3);
  };
  srand48(thissum);
}

void resetMatrix(){
  for (i=0; i<xpix; i++){
    for (j=0; j<ypix; j++) setmatrix[i][j]=0;
  };
}

void resetMatrix2(){
  for (i=0; i<xpix; i++){
    for (j=0; j<ypix; j++) setmatrix2[i][j]=0;
  };
}

//2d functions===========================================================

void dot2D( point2 d1 ){
  pixel d2=pointPixel( d1 );
  drawPoint(win, d2.x, d2.y );
  drawPoint( pix, d2.x, d2.y );
}

void line2D( point2 lp1, point2 lp2 ){
  pixel lp3=pointPixel( lp1 ), lp4=pointPixel( lp2 );
  drawLine(win, lp3.x, lp3.y, lp4.x, lp4.y );
  //drawLine( pix, lp3.x, lp3.y, lp4.x, lp4.y );
}

void drawAxes(){
  setColor( axesC );
  line2D( p2(xmin, 0), p2(xmax, 0) );
  line2D( p2(0, ymin), p2(0, ymax) );
  //draw grid
}

zoom2D(){
  int xmins, xmaxs, ymins, ymaxs;
  double pxmin, pxmax, pymin, pymax;
  pixel cor1, cor2;
  Event e;
  comment( "Use the mouse to choose a new viewing window. " ); 
  //setColor(backC-1);  //(doesn't show up on black)
  setColor( partC );   //but this is useless if screen color=partC!
  setDrawMode(GXxor);
  e = getNextEvent();
  while ( (e.type != ButtonPress) || (e.value != 1) ){
    e = nextEvent(); 
    if (e.type == Expose) redraw();
  };
  cor1.x = e.x; 
  cor1.y = e.y;
  e = nextEvent();
  if (e.type == Expose) redraw();
  cor2=cor1;
  xmaxs=cor2.x; xmins=cor1.x; 
  if (cor1.x > cor2.x){ xmaxs=cor1.x; xmins=cor2.x;};
  ymaxs=cor2.y; ymins=cor1.y; 
  if (cor1.y < cor2.y){ ymaxs=cor1.y; ymins=cor2.y;};
  drawRectangle(win, cor1.x, cor1.y, 0, 0 );
  while ( (e.type != ButtonPress) || (e.value != 1) ){
    e = nextEvent();
    if (e.type == Expose) redraw(); 
    drawRectangle(win, xmins, ymaxs, xmaxs-xmins, ymins-ymaxs );
    cor2.x = e.x; 
    cor2.y = e.y;
    xmaxs=cor2.x; xmins=cor1.x; 
    if (cor1.x > cor2.x){ xmaxs=cor1.x; xmins=cor2.x;};
    ymaxs=cor2.y; ymins=cor1.y; 
    if (cor1.y < cor2.y){ ymaxs=cor1.y; ymins=cor2.y;};
    drawRectangle(win, xmins, ymaxs, xmaxs-xmins, ymins-ymaxs );
  };
  setDrawMode( GXcopy );
  drawRectangle(win, xmins, ymaxs, xmaxs-xmins, ymins-ymaxs );
  xmaxs=cor2.x; xmins=cor1.x; 
  if (cor1.x > cor2.x){ xmaxs=cor1.x; xmins=cor2.x;};
  ymaxs=cor2.y; ymins=cor1.y; 
  if (cor1.y < cor2.y){ ymaxs=cor1.y; ymins=cor2.y;};
  pxmin=xmin+((xmins+.5)*xspan/xpix);
  pxmax=xmin+((xmaxs+.5)*xspan/xpix);
  pymin=ymax-((ymins+.5)*yspan/ypix);
  pymax=ymax-((ymaxs+.5)*yspan/ypix);
  xmin=pxmin; xmax=pxmax; ymin=pymin; ymax=pymax;
  newLine();
  setColor( textC );
  printString( "x min = " ); printNumber( xmin ); newLine();
  printString( "x max = " ); printNumber( xmax ); newLine();
  printString( "y min = " ); printNumber( ymin ); newLine();
  printString( "y max = " ); printNumber( ymax ); newLine();
  rrangeconst2D();
}

void zoomOut2D(){
  xmin=xmin-(xspan/3);
  xmax=xmax+(xspan/3);
  ymin=ymin-(yspan/3);
  ymax=ymax+(yspan/3);
  rrangeconst2D();
}
//=======================================================================

    //get rid of these stupid things!
  pixel backPB, quitPB, refreshPB;
  
//#include "3D.h"

//#include "2D.h"
//#include "F3D.h"
//#include "random.h"
//#include "fractal.h"  //take distance.h out of fractal.h!
//#include "distance.h"
//#include "field.h"
//#include "matrix.h"
//#include "mathe.h"
//#include "textEdit.h"
//#include "conway.h"
//#include "mathF.h"
#include "shadow.h"

int main(){
  charSetup();
  initComplex();
  interfaceSetup();

    //initialize menus 
  panelcor.x=0; panelcor.y=xpix+commentheight;
  generalcor.x=xpix; generalcor.y=tall-menuheight; 
  panelmenu=newMenu(0, panelcor.y, xpix, tall-panelcor.y, panelrows, panelcols);
  general=newMenu(generalcor.x, generalcor.y, pagelength, menuheight, menurows, menucols);
  leftmenu=newMenu(0, 0, xpix, tall, 10, 3);
    
    //make line menu
  for (i=0; i<pagelines; i++) linelist[i] = -2;
  linemenu=startGroup();
  for (i=0; (i+1)*lheight<=pageheight; i++)
  linelist[i]=addList( pagecor.x, pagecor.y+(i*lheight)+ycor, pagelength, lheight);
     
    //menu positions
  pixel fractalB, analysisB, mechanicsB, randomB, systemB, matrixB, mathB, tEB, lifeB;
  pixel F2DB, F3DB, fieldsB, geometryB, distanceB;

    //assign places for the menu areas    
  boringB.x=0; boringB.y=0;
  yesB=boringB; noB.x=1; noB.y=0;
  contB=boringB;
  
  /*
  setMenu(panelmenu);
  resetMenu();
  backB.x=menucols-2; backB.y=menurows-1;
  quitB.x=menucols-1; quitB.y=menurows-1;*/
 
  backPB.x=panelcols-2; backPB.y=panelrows-1;
  quitPB.x=panelcols-1; quitPB.y=panelrows-1;
  refreshPB.x=panelcols-2; refreshPB.y=panelrows-1;
  setMenu(general);
  resetMenu();
  yesB=addMenu();
  noB=addMenu();
  resetMenu();
  fractalB=addMenu();
  analysisB=addMenu();
  mechanicsB=addMenu();
  geometryB=addMenu();
  randomB=addMenu();
  systemB=addMenu();
  matrixB=addMenu();
  lifeB=addMenu();
  mathB=addMenu();
  tEB=addMenu();

  resetMenu();
  F2DB=addMenu();
  F3DB=addMenu();
  fieldsB=addMenu();
  
  resetMenu();
  distanceB=addMenu();
  //exit(1);  
//-------------------------------------------------------------------------
  axisC.x=1; axisC.y=40; axisC.z=160;
  //rotControls();
  //traceArrows();
//-------------------------------------------------------------------------
  initWindow(20, 20, wide, tall, "Mathematics master program, by Josh Shinavier");   
  findBlack();
  setColors(blackC, whiteC, 40, 60, 80, 144);
  setPageColors( backC, textC );
  mpage=initPage(win, pix, xpix, xpix+pagelength, 0, tall-menuheight);
  setPage(mpage);
  resetPage();

  randomize();

  int stopgeom, stopanalysis;  
  clear();
  while (1==1){
    resetAll();
    comment( "Main menu: choose a program type. " );
    setMenu(general);
    setColor( textC );
    putMenu(fractalB, "Fractals" );
    putMenu(analysisB, "Analysis" );
    putMenu(mechanicsB, "Mechanics" );
    putMenu(randomB, "Randomness" );
    putMenu(matrixB, "Matrices" );
    putMenu(lifeB, "Life" );
    putMenu(geometryB, "Geometry");
    putMenu2(mathB, "Mathematical", "functions" );
    //putMenu2(tEB, "Text", "editor" );
    //putMenu(systemB, "System" );
    writeQuitM();
    boxM=shadowMain();
    //if (chosen(photoB)) photograph();
    //if (chosen(fractalB)) fractalMain();
    //if (chosen(matrixB)) matrixMain();
    //if (chosen(randomB)) randomMain();
    //if (chosen(mathB)) mathMain();
    //if (chosen(tEB)) tEMain();
    //if (chosen(lifeB)) conwayMain();
    //if (chosen(systemB)) inputSystem();
    //if (chosen(mathB)) mathFMain();   
    if (chosen(geometryB)){
      stopgeom=0;
      while (!stopgeom){
        setMenu(general);
        clearMenu();
        setColor( textC );
        putMenu(distanceB, "distance"); 
        writeBackM(); writeQuitM();
        getChoiceM( general );
        //if (chosen(distanceB)) distanceMain();
        if (backM()) stopgeom=1;
        if (quitM()) exit(1);
      };
    };
    if (chosen(analysisB)){
      stopanalysis=0;
      while (!stopanalysis){
        setMenu(general);
        clearMenu();
        setColor( textC );
        putMenu(F2DB, "2D Functions" );
        putMenu(F3DB, "3D Functions" );
        putMenu(fieldsB, "fields" );
        getChoiceM( general );
        //if (chosen(F3DB)) F3DMain();
        //if (chosen(F3DB)) F3DMain();
        //if (chosen(F2DB)) 2DMain();
        //if (chosen(fieldsB)) fieldMain();
        if (backM()) stopanalysis=1;
        if (quitM()) exit(1);
      };
    };
    if (quitM()) exit(1);
  };   
}
