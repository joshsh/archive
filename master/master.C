//note: from now on, objects will be drawn only on the pixmap pix and not on
//the window as well, to minimize server requests.  After drawing a set of
//objects, redrawScreen() will have to be used to transfer them to to screen. 

#include "graphics.h"
#include <math.h>
//#include <fstream.h>
//#include <strings.h>

void redraw();

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

#include "files.h"
#include "complex.h"
#include "inout.h"
#include "areas.h"

struct pixel{
  int x, y;
};

struct point2{
  double x, y;
};

struct point3{
  double x, y, z;
};

/*struct icon{
  int length, height;
  pixmap pix;
}*/

int const xpix=500;
int const ypix=500;
int const menucols=4;
int const menurows=5;
int const panelcols=4;
int const panelrows=4;
int const pagelines=35;

int letterheight=12;
int textheight=20;
int commentheight=20; //height of comment box
int commentlength;  //length of currently displayed comment string
int pmenuheight=panelrows*textheight;
int pmenu[panelcols][panelrows];
int panelheight=pmenuheight+commentheight;
int menuheight=200;
int menu[menucols][menurows];
int linelist[pagelines];
//int lileli[pagelines]; //( line length list ) 
int pagelength=400, pageheight=ypix+panelheight-menuheight;
int wide=xpix+pagelength;
int tall=ypix+panelheight;
int textwidth=xpix/panelcols;
int textC=40, partC=144, objectC=100, stopC=10, boxC=55, axesC=50;
int highC=180, lowC=1, colspan3D=highC-lowC, cconst3D;
int numcols=256;  //if numcols isn't a perfect square, getNum() must be changed
int xs0=xpix/2, ys0=ypix/2;
int xsmid=xpix/2, ysmid=ypix/2;
int real=1, complex=2;
int mathmode;
int mode; //for general use
int up=1, right=2, down=3, left=4;
int startatx, startaty;  //for 3-D graphing, the direction at the 'back'
int intA, intB, intC;
double dx, dy, dz, dt, realA, realB, realC, realD;

//coloring constants
int coloring;
int byiter=1, bymag=2, byrep=3, colconst=4, bydist=5;

double backC, whiteC;
double const pi=3.1415;

//--------------------------------------------------------------------
//area name constants
int thaP, thaM, thbP, thbM, lconstP, lconstM, xP, xM, yP, yM;

//area group constants
int general, panelmenu, linemenu, rotcontrols, tracearrows;

double lconst;
double r;  //length of reference vector for parallel projection
double x, y, Z, t, xl, yl, zl, tl;
double pc1, pc2, pc3, pc4, pc5, pc6, pc7, pc8;
double angleinc=pi/20; //angle - change increment for axes positioning
double lenginc=1.1;  //length - change increment "     "
double xax, xay, yax, yay, yaz;
double xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax, xmid, ymid, zmid, tmid;
double xspan, yspan, zspan, tspan, xinc, yinc, tinc;
double tdivs; //really an integer, but so that integer division isn't used...
double minz, maxz; //minimum and maximum z-values for elevation graphing
double distCconst1, distCconst2;
double cconst3D1, cconst3D2;

point2 theta;
point3 axisC;
point3 ref; //point for direction of parallel projection
//--------------------------------------------------------------------

int i, j, k, l, m, n;

  //data fields
int setmatrix[xpix][ypix];
int setmatrix2[xpix][ypix];
double field[xpix][ypix];
Window w;
Pixmap pix;

pixel pagecor;
pixel menucor;
pixel pmenucor;
pixel mainMenuPlace;
pixel panelPlace;

//menu positions
pixel contB;
pixel yesB, noB;
pixel fractalB, analysisB, mechanicsB, randomB, systemB, matrixB, mathB, tEB, lifeB;
pixel F2DB, F3DB, fieldsB;
pixel backB, backPB;
pixel quitB, quitPB;
pixel photoB, photoPB;
pixel refreshPB, stopPB;
pixel magB, cconstB, distB, repB, iterB; //for general use.
pixel boringB;  //with coordinates (0, 0)


//preliminary function declarations
int topten( double tt1 );

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

//graphing, transformations, event handling --------------------------------

pixel mpixel( int mp1, int mp2 ){
  pixel mp3;
  mp3.x=mp1;
  mp3.y=mp2;
  return mp3;
}

point2 mpoint2( double mp1, double mp2 ){
  point2 mp3;
  mp3.x=mp1;
  mp3.y=mp2;
  return mp3;
}

point3 mpoint3( double mp1, double mp2, double mp3 ){
  point3 mp4;
  mp4.x=mp1;
  mp4.y=mp2;
  mp4.z=mp3;
  return mp4;
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
  pp2.x=((pp1.x-xmin)/xinc)-.5;
  pp2.y=((ymax-pp1.y)/yinc)-.5;
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
  tdivs=inputNumber( "divisions = " );
  tspan=tmax-tmin;
  tinc=tspan/tdivs;
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

void alarm( int alarmcolor ){
  int lastC=getColor();
  setColor(alarmcolor);
  fillRectangle(w, 0, 0, wide, tall);
  wait();
  setColor(lastC);
  redraw();
}

//-----------------------------------------------------------------------

double sq( double sq1 ){
  return sq1*sq1;
}

double middist(point3 md1){
  return sq(md1.x-xmid)+sq(md1.y-ymid)+sq(md1.z-zmid);
}

int menuNumber( pixel mn1 ){
  return menu[mn1.x][mn1.y];
}

int panelNumber( pixel pn1 ){
  return pmenu[pn1.x][pn1.y];
}

void resetPixels(){
 magB=boringB;
 cconstB=boringB;
 distB=boringB;
 repB=boringB;
 iterB=boringB;
}

void clearComment(){
  setColor( backC );
  fillRectangle( w, 2, ypix+1, xpix-3, commentheight-2 );
  fillRectangle( pix, 2, ypix+1, xpix-3, commentheight-2 );
  //fillRectangle( panel, 2, 1, xpix-3, commentheight-2 );
}

void comment( char *com1 ){
  int lastC = getColor();
  clearComment();
  setColor( backC-1 );
  drawString( w, 5, ypix+letterheight, com1 );
  drawString( pix, 5, ypix+letterheight, com1 );
  //drawString( panel, 5, letterheight, com1 );
  commentlength=strlen( com1 );
  setColor( lastC );
}

void addComment( char *com1){
  int lastC = getColor();
  setColor( backC - 1 );
  drawString( w, 5+(commentlength*lwidth), ypix+letterheight, com1 );
  drawString( pix, 5+(commentlength*lwidth), ypix+letterheight, com1 );
  //drawString( panel, 5+(commentlength*lwidth), letterheight, com1 );
  commentlength = commentlength + strlen( com1 );
  setColor( lastC );
}

void putPanel( pixel ppm1, char *ppm2 ){
  int ppmx=pmenucor.x+((ppm1.x*textwidth)+lwidth);
  int ppmy=pmenucor.y+((ppm1.y*textheight)+letterheight);
  drawString( w, ppmx, ppmy, ppm2 );
  //drawString( panel, ppmx, ppmy-ypix, ppm2 ); 
  drawString( pix, ppmx, ppmy, ppm2 );
}

void clearPanel(){
  int lastC=getColor();
  //cout << endl << "lastC = " << lastC << endl;
  setColor( backC );
  fillRectangle( pix, 0, commentheight+ypix, xpix, panelheight-commentheight );
  clearComment();
  setColor( partC );
  drawAreas(pix, panelmenu, 0, 0 );
  setColor( textC );
  //putPanel( photoB, "save" );
  putPanel( backPB, "back" );
  putPanel( quitPB, "quit" );
  copyArea( pix, w, 0, ypix, xpix, panelheight, 0, ypix );
  setColor( lastC );
}

int onPanel(int opx, int opy){
  if (areaName(panelmenu, opx, opy) > -1) return 1;
  return 0;
}

int onMainMenu(int omx, int omy){
  if (areaName(general, omx, omy) > -1) return 1;
  return 0;
}

void drawPanel(){
  //copyArea( panel, w, 0, 0, xpix, panelheight, 0, ypix);
  copyArea( pix, w, 0, ypix, xpix, panelheight, 0, ypix);
}

void emptyPanel(){
  int lastC=getColor();
  setColor( backC );
  //fillRectangle( panel, 0, 0, xpix, panelheight);
  fillRectangle( pix, 0, ypix, xpix, panelheight);
  setColor( lastC );
}

void putMenu( pixel pm1, char *pm2 ){
  int pmx=xpix+(pm1.x*pagelength/menucols)+lwidth;
  int pmy=ypix+panelheight-menuheight+(pm1.y*menuheight/menurows)+letterheight;
  drawString( w, pmx, pmy, pm2 );
  drawString( pix, pmx, pmy, pm2 );
}

void putMenu2( pixel pm1, char *pm2, char *pm3 ){
  int pmx=xpix+(pm1.x*pagelength/menucols)+lwidth;
  int pmy=ypix+panelheight-menuheight+(pm1.y*menuheight/menurows)+letterheight;
  drawString( w, pmx, pmy, pm2 );
  drawString( w, pmx, pmy+letterheight, pm3);
  drawString( pix, pmx, pmy, pm2 );
  drawString( pix, pmx, pmy+letterheight, pm3);
}

void newMainMenu(){
  mainMenuPlace.x=0;
  mainMenuPlace.y=0;
}

pixel addMainMenu(){
  mainMenuPlace.x=mainMenuPlace.x+1;
  if (mainMenuPlace.x >= menucols){
    mainMenuPlace.x=0;
    mainMenuPlace.y=mainMenuPlace.y+1;
  };
  return mainMenuPlace;
}

void clearMenu(){
  int lastC=getColor();
  setColor( backC );
  fillRectangle(pix, xpix, ypix+panelheight-menuheight, pagelength, menuheight);
  setColor( partC );
  drawAreas( pix, general, 0, 0 );
  setColor( textC );
  putMenu( backB, "back" );
  putMenu( quitB, "quit" );
  //putMenu( photoB, "save" );
  copyArea( pix, w, xpix, ypix+panelheight-menuheight, pagelength, menuheight,
    xpix, ypix+panelheight-menuheight );
  setColor( lastC );
}

void writeYesNo(){
  setColor( textC );
  putMenu( yesB, "yes" );
  putMenu( noB, "no" );
}

void redraw(){
  copyArea( pix, w, 0, 0, wide, tall, 0, 0 );
}

void redrawScreen(){
  copyArea( pix, w, 0, 0, xpix, ypix, 0, 0 );
}

int findColor(){
  //starts with color 1, ends with color numcols.
  //findColor assumes that xpix==ypix
  int returncolor;
  int colside=sqrt( numcols );
  //cout << endl << "colside = " << colside << endl;  //!
  double sqside=xpix/(colside*1.0);
  for (i=1; i <= colside; i++){
    for (j=1; j <= colside; j++){
      setColor( ((j-1)*colside)+i );
      fillRectangle( w, (i-1)*sqside, (j-1)*sqside, sqside+1, sqside+1 );
    };
  };
  Event e;
  while ( checkEvent() ) e=nextEvent();
  e=nextEvent();
  while ((e.type != ButtonPress)||(e.value != 1)) e=nextEvent();
  returncolor=((e.y/(sqside*1.0))*colside)+((e.x/(sqside*1.0))+1);
  redraw();
  return returncolor;
}

void drawPixel( pixel dp1 ){
  drawPoint( w, dp1.x, dp1.y );
  drawPoint( pix, dp1.x, dp1.y );
}

void clear(){
  int lastC = getColor();
  setColor(backC);
  fillRectangle(w, 0, 0, xpix, ypix);
  fillRectangle(pix, 0, 0, xpix, ypix);
  setColor( lastC );
}

void clearField(){
  comment("Resetting projection matrix..." );
  for (i=0; i<xpix; i++){
    for (j=0; j<ypix; j++) field[i][j]=0;
  };
  comment("done.");
}

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
}

int getYesNo( char *gyn1 ){
  printLine( gyn1 );
  clearMenu();
  writeYesNo();
  int box = getChoice( general );
  if (menuNumber( yesB ) == box) return 1;
  else return 0;
}

void resetAll(){
  redraw();
  clearMenu();
  clearPanel();
  resetWindow();
}

double controlBar(char *title, double lowval, double highval, int x, int y, int length){
  int height=commentheight*4;
  int lastC = getColor(); 
  int controlheight=commentheight*2;
  int numlen=lwidth*(disdigits+topten(highval));
  int numplace=x+((length+numlen)/2)+5;
  int lineY=(commentheight*3)+y;
  int barplacelast;
  setColor( backC );
  fillRectangle(w, x, y, length, height);
  //setColor( boxC );
  //fillRectangle(w, x+length-commentheight, y, commentheight, commentheight);
  setColor( partC );
  drawRectangle(w, x, y, length, height);
  drawLine(w, x, y+commentheight, x+length, commentheight);
  drawRectangle(w, x+length-commentheight, y, commentheight, commentheight);
  setColor( textC );
  drawString(w, x+5, y+letterheight, title);
  drawString(w, x+length-commentheight+5, y+letterheight, "Done" );
  drawNumber(w, x+5, y+commentheight+letterheight, lowval);  //!
  drawNumber(w, x+length-(2*numlen)-15, y+commentheight+letterheight, highval);

  double presentval;
  int barplace=10+x;

  setColor( backC );
  fillRectangle(w, x+numplace-5, y+commentheight+2, numlen+10, commentheight-3);
  fillRectangle(w, x+1, y+commentheight+2, length-3, controlheight-3);
  setColor( backC-1 );
  presentval=lowval+((barplace-x-10)*(highval-lowval)/(length-20));
  drawNumber(w, numplace, y+commentheight+letterheight, presentval);
  setLineWidth(2);
  setColor( partC );
  drawLine(w, x+5, lineY, x+length-5, lineY);
  setLineWidth(3);
  setColor( backC-1 );
  drawLine(w, barplace-1, lineY-10, barplace-1, lineY+10);

  Event e;
  //while (checkEvent()) e=nextEvent();
  //e=nextEvent();
  updateEvents();
  e=nextEvent();
  if (e.type == Expose) redraw();
  int done=0, stop;
  while (!done){
    if ((e.type == ButtonPress)&&(e.value == 1)){
       stop=0;
      if((e.y > y)&&(e.y < y+commentheight)&&(e.x > x+length-commentheight)&&(e.x < x+length)){ stop=1; done=1;};
       while (!stop){
         if (fabs(e.y-lineY) <= 10){
           barplacelast=barplace;
           barplace=e.x;
           setColor( backC );
           fillRectangle(w, numplace-5, y+commentheight+2, numlen+10,            commentheight-3);
           fillRectangle(w, 1, commentheight+2, length-3, controlheight-3);
           setColor( backC-1 );
           presentval=lowval+((barplace-x-10)*(highval-lowval)/(length-20));
           drawNumber(w, numplace, y+commentheight+letterheight, presentval);
           setLineWidth(3);
           setColor(backC);
           drawLine( w, barplacelast-1, lineY-10, barplacelast-1, lineY+10);
           setColor( backC-1 );
           drawLine(w, barplace-1, lineY-10, barplace-1, lineY+10);
           setLineWidth(2);
           setColor( partC );
           drawLine(w, x+5, lineY, x+length-5, lineY);
         };
         if (checkEvent()) e=nextEvent();
         if (e.type == Expose) redraw(); 
         if (e.type == ButtonRelease) stop=1;
       };
    };
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
    
  setLineWidth(1);
  setColor( backC );
  fillRectangle(w, x, y, length+1, height+1);
  setColor( lastC );
  return presentval;
}

void writeStop(){
  int lastC = getColor();
  clearPanel();
  setColor( backC );
  putPanel( backPB, "back" );
  putPanel( quitPB, "quit" );
  setColor( stopC );
  putPanel( refreshPB, "refresh screen" );
  putPanel( stopPB, "stop" );
  setColor( lastC );
}

int checkStop(){
  Event e;
  while ( checkEvent() ){
    e=nextEvent();
    if (e.type == Expose) redraw();
    if ((e.type == ButtonPress)&&(e.value == 1)){
      if (panelNumber( refreshPB )==areaName(panelmenu, e.x, e.y )){
        redraw();
        clearMenu();
        resetWindow();
      };
      if (panelNumber( stopPB ) == areaName( panelmenu, e.x, e.y ) ) return 1;
    };
  };
  if (e.type == Expose) redraw();
  if ((e.type == ButtonPress)&&(e.value == 1)){
    if (panelNumber( refreshPB )==areaName(panelmenu, e.x, e.y )){
      redraw();
      clearMenu();
      resetWindow();
    };
    if (panelNumber( stopPB ) == areaName( panelmenu, e.x, e.y ) ) return 1;
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
  
  resetWindow();
  //setColor( partC );
  //pix=drawAreas( pix, linemenu, 0, 0 );
  
  Event e;
  resetWindow();
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
      resetWindow();
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
  writeStop();
  while (!checkStop()){
    e=nextEvent();
    if (e.type == Expose) redraw();
    if (e.type == MotionNotify){
      p=eventPoint(e);
      resetWindow();
      printString( c1 ); printString( " = " ); printNumber(p.x); newLine();
      printString( c2 ); printString( " = " ); printNumber(p.y);
    };
  };
  resetWindow();
  xmin=lxmin; xmax=lxmax; ymin=lymin; ymax=lymax;
  rrangeconst2D();
}

void inputSystem(){
  resetWindow();
  setColor( textC );
  printString("Decimal digits displayed = "); printNumber(disdigits); newLine();
  disdigits=-1;
  while (disdigits < 0){
    disdigits=inputNumber( "new value = " );
    newLine();
    if (disdigits < 0) printLine( "That is ridiculous. Try again. " );
  };
  resetWindow();
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

//mathematical procedures-----------------------------------------------

int randInt( int ri1 ){
  return drand48()*(ri1+1);
}

double trunc( double t1, int digits){
  int t2=t1*tenpow(digits);
  return t2/(tenpow(digits)*1.0);
}

double cut( double c1, int digits){
  return trunc (c1, digits-tenpow(c1) );
}

int brack( double b1 ){
  int b2=b1;
  return b2; 
}

double min( double m1, double m2 ){
  if (m1 < m2) return m1;
  else return m2;
}

double max( double m1, double m2){
  if (m1 > m2) return m1;
  else return m2;
}

double norm2D( point2 d1 ){
  return sqrt( sq(d1.x) + sq(d1.y) );
}

double norm3D( point3 d1 ){
  return sqrt( sq(d1.x) + sq(d1.y) + sq(d1.z) );
}

double dist2D( point2 d1, point2 d2){
  return sqrt( sq(d1.x-d2.x) + sq(d1.y-d2.y) );
}

double dist3D( point3 d1, point3 d2 ){
  return sqrt( sq(d1.x-d2.x) + sq(d1.y-d2.y) + sq(d1.z-d2.z) );
}

int topten( double tt1 ){
  if (tt1 < 0) return -1;
  if (tt1 < 10) return 0;
  j=0;
  i=10;
  while (i <= tt1) i=i*10;
  return j;
}

double fixAngle(double preAngle){
  if ((preAngle >=0 ) && (preAngle <= (2*pi))) return preAngle;
  while (preAngle > (2*pi)) preAngle=preAngle-(2*pi);
  while (preAngle < 0) preAngle=preAngle+(2*pi);
  return preAngle;
}

int sign( double si1){
  if (!si1) return 0;
  if (si1 < 0) return -1;
  return 1;
}

//color and graphics---------------------------------------------------

void drawLargePoint(Drawable dld, int dlx, int dly, int dscale ){
  fillRectangle(dld, dscale*dlx, dscale*dly, dscale, dscale);
}

void drawLargePointOffset(Drawable dld, int dlx, int dly, int dscale, pixel offset ){
  fillRectangle(dld, offset.x+(dscale*dlx), offset.y+(dscale*dly), dscale, dscale);
}

/*icon makeIcon(pixmap mi1, int milength, int miheight){
  icon miic;
  miic.pix=createPixmap(w, milength, miheight);
  miic.length=milength;
  miic.height=miheight;
  return miic; 
}

void putIcon(icon ic1, int corx, int cory){
  copyArea(ic1.pix, pix, 0, 0, ic1.length, ic1.height, corx, cory);
  copyArea(ic1.pix, w, 0, 0, ic1.length, ic1.height, corx, cory);
}*/

//void putMenuIcon( icon pmi1, string pmi2 ){
//
//}

//void putPanelIcon( icon ppi1, string ppi2 ){
//
//}

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

//2d functions---------------------------------------------------------
void dot2D( point2 d1 ){
  pixel d2=pointPixel( d1 );
  drawPoint( w, d2.x, d2.y );
  drawPoint( pix, d2.x, d2.y );
}

void line2D( point2 lp1, point2 lp2 ){
  pixel lp3=pointPixel( lp1 ), lp4=pointPixel( lp2 );
  drawLine( w, lp3.x, lp3.y, lp4.x, lp4.y );
  //drawLine( pix, lp3.x, lp3.y, lp4.x, lp4.y );
}

point2 makePoint( double mp1, double mp2 ){
  point2 mp3;
  mp3.x=mp1;
  mp3.y=mp2;
  return mp3;
}

void drawAxes(){
  setColor( axesC );
  line2D( makePoint(xmin, 0), makePoint(xmax, 0) );
  line2D( makePoint(0, ymin), makePoint(0, ymax) );
  //draw grid
  //
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
  drawRectangle( w, cor1.x, cor1.y, 0, 0 );
  while ( (e.type != ButtonPress) || (e.value != 1) ){
    e = nextEvent();
    if (e.type == Expose) redraw(); 
    drawRectangle( w, xmins, ymaxs, xmaxs-xmins, ymins-ymaxs );
    cor2.x = e.x; 
    cor2.y = e.y;
    xmaxs=cor2.x; xmins=cor1.x; 
    if (cor1.x > cor2.x){ xmaxs=cor1.x; xmins=cor2.x;};
    ymaxs=cor2.y; ymins=cor1.y; 
    if (cor1.y < cor2.y){ ymaxs=cor1.y; ymins=cor2.y;};
    drawRectangle( w, xmins, ymaxs, xmaxs-xmins, ymins-ymaxs );
  };
  setDrawMode( GXcopy );
  drawRectangle( w, xmins, ymaxs, xmaxs-xmins, ymins-ymaxs );
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
//-------------------------------------------------------------------

#include "3D.h"

//#include "2D.h"
//#include "F3D.h"
//#include "random.h"
#include "fractal.h"
//#include "field.h"
//#include "matrix.h"
//#include "mathe.h"
//#include "textEdit.h"
//#include "conway.h"
//#include "shadow.h"

int main(){
  //mathmode = real;

  // find the color black
  /*
  Window cW=createWindow(20,20,1000,25, "Choose a background color.");
  for (i=1; i<=40; i++){
    setColor(205+i);
    fillRectangle(cW, (i-1)*25, 0, 25, 25);
  };
  Event e = nextEvent();
  if (e.type == Expose) redraw();
  int colorplace; 
  while ( (e.type != ButtonPress) || (e.value != 3) ){
    if ( ( e.type == ButtonPress ) && ( e.value == 1 ) ){
      colorplace = e.x; 
    };
    e = nextEvent(); 
    if (e.type == Expose) redraw();
  };     
  backC=206+(colorplace/25);
  destroyWindow( cW );
  */
  
  pagecor.x=xpix;
  pagecor.y=0;
  menucor.x=xpix;
  menucor.y=ypix+panelheight-menuheight;
  pmenucor.x=0;
  pmenucor.y=ypix+commentheight;

  //assign places for the menu areas  
  boringB.x=0; boringB.y=0;
  stopPB.x=panelcols-1; stopPB.y=panelrows-1;
  photoB.x=menucols-3; photoB.y=menurows-1;
  backB.x=menucols-2; backB.y=menurows-1;
  quitB.x=menucols-1; quitB.y=menurows-1;
  photoPB.x=panelcols-3; photoPB.y=panelrows-1; 
  backPB.x=panelcols-2; backPB.y=panelrows-1;
  quitPB.x=panelcols-1; quitPB.y=panelrows-1;
  refreshPB.x=panelcols-2; refreshPB.y=panelrows-1;
  newMainMenu();
  yesB=addMainMenu();
  noB=addMainMenu();
  newMainMenu();
  fractalB=addMainMenu();
  analysisB=addMainMenu();
  mechanicsB=addMainMenu();
  randomB=addMainMenu();
  systemB=addMainMenu();
  matrixB=addMainMenu();
  lifeB=addMainMenu();
  mathB=addMainMenu();
  tEB=addMainMenu();
  newMainMenu();
  F2DB=addMainMenu();
  F3DB=addMainMenu();
  fieldsB=addMainMenu();
  
  /*boringB.x=0; boringB.y=0;
  noB.x=1;
  analysisB.x=1; mechanicsB.x=2; randomB.x=3;
  matrixB.y=1; mathB.y=1; tEB.y=1; systemB.y=1;
  mathB.x=1; tEB.x=2; systemB.x=3;
  backB.x=menucols-2; backB.y=menurows-1;
  quitB.x=menucols-1; quitB.y=menurows-1;
  backPB.x=panelcols-2; backPB.y=panelrows-1;
  quitPB.x=panelcols-1; quitPB.y=panelrows-1;
  refreshPB.x=panelcols-2; refreshPB.y=panelrows-1;
  stopPB.x=panelcols-1; stopPB.y=panelrows-1;
  F3DB.x=1; fieldsB.x=2;*/

//-------------------------------------------------------------------------
  axisC.x=1; axisC.y=40; axisC.z=160;
  rotControls();
  //traceArrows();
//-------------------------------------------------------------------------

  setTextColor( textC );
  initComplex();
  
    //make window
  w=createWindow(20,20,wide,tall,"Mathematics master program, by Josh Shinavier");
  pix=createPixmap(w, wide, tall);
  comment( "Choose a background color. " );
  //backC=findColor();
  
  //!
  int found=0;
  backC=195;
  while (!found){
    backC++;
    setColor(backC);
    fillRectangle(w, 0, 0, 100, 100);
    j=nextButtonPress();
    if (j==1) found=1;
  };
  //!
  
  whiteC=backC-1;
  setColor(backC);
  fillRectangle(w,0,0,wide,tall);
  fillRectangle(pix, 0, 0, wide, tall);
  setClearColor( backC );
  setWindow( w, pix );
  setLimits( xpix, xpix+pagelength, 0, tall-menuheight );
  setCorner( pagecor.x + lwidth, lheight );
  resetWindow();

  randomize();
  int aa, bb;
  
    //make panel menu
  aa=textwidth;
  bb=textheight;
  panelmenu=startGroup();
  for( i=0; i < panelcols; i++){
    for( j=0; j < panelrows; j++)
      pmenu[i][j]=addList( i*aa, pmenucor.y+(j*bb), aa, bb );
  };

    //make general menu
  int mxcor=xpix, mycor=ypix+panelheight-menuheight;
  aa=pagelength/menucols;
  bb=menuheight/menurows;
  general=startGroup();
  for (i=0; i < menucols; i++){
    for (j=0; j < menurows; j++){
      menu[i][j]=addList( mxcor+(aa*i), mycor+(bb*j), aa, bb );
    };
  };
    
    //make line menu
  for (i=0; i<pagelines; i++) linelist[i] = -2;
  linemenu=startGroup();
  for (i=0; (i+1)*lheight<=pageheight; i++)
    linelist[i]=addList( pagecor.x, pagecor.y+(i*lheight)+ycor, pagelength, lheight);
    
  clear();
  int box;
  while (1==1){
    resetAll();
    comment( "Main menu: choose a program type. " );
    setColor( textC );
    putMenu( fractalB, "Fractals" );
    putMenu( analysisB, "Analysis" );
    putMenu( mechanicsB, "Mechanics" );
    putMenu( randomB, "Randomness" );
    putMenu( matrixB, "Matrices" );
    putMenu( lifeB, "Life" );
    putMenu2( mathB, "Mathematical", "functions" );
    putMenu2( tEB, "Text", "editor" );
    putMenu( systemB, "System" );
    //box = shadowMain();
    box=getChoice(general);
    //if (box == menuNumber(photoB)) photograph();
    if (box == menuNumber(quitB)) exit(1);
    if (box == menu[fractalB.x][fractalB.y]) fractalMain();
    //if (menuNumber( matrixB ) == box) matrixMain();
    //if (box == menuNumber(randomB)) randomMain();
    //if (box == menuNumber(mathB)) mathMain();
    //if (box == menuNumber(tEB)) tEMain();
    //if (box == menuNumber(lifeB)) conwayMain();
    if (box == menu[analysisB.x][analysisB.y]){
      clearMenu();
      setColor( textC );
      putMenu( F2DB, "2D Functions" );
      putMenu( F3DB, "3D Functions" );
      putMenu( fieldsB, "fields" );
      box = getChoice( general );
      //if (box == menu[F3DB.x][F3DB.y]) F3DMain();
      //if (box == menuNumber( F2DB ) ) 2DMain();
      //if (box == menuNumber( fieldsB )) fieldMain();
    };
    if (menuNumber( systemB ) == box) inputSystem();
  };   
}
