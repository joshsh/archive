//simple 2D plotting utility

#include "graphics.h"

struct point2{
  double x, y;
};

int backC, whiteC, axesC, plotC;
int xpix, ypix, xorg, yorg;
double xmin, ymin, xmax, ymax, xmid, ymid, xspan, yspan, xinc, yinc;
Window win;
Pixmap pix;

void redraw(){
  copyArea( pix, win, 0, 0, xpix, ypix, 0, 0 );
}

void updateEvents(){
  Event ue1;
  while (checkEvent()){
    ue1=nextEvent();
    if (ue1.type == Expose) redraw();
  };
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

void wait(){
  Event w1;
  updateEvents();
  do{
    w1=nextEvent();
    if (w1.type == Expose) redraw();
  } while (w1.type != ButtonPress );
}

void initWindow( int xcor, int ycor, int xx, int yx){
  xpix=xx; ypix=yx;
 
  win=createWindow(xcor,ycor,xpix,ypix,"2D plotting utility");
  pix=createPixmap(win, xpix, ypix);

  int found=0;
  backC=195;
  
  int jj;
  while (!found){
    backC++;
    setColor(backC);
    fillRectangle(win, 0, 0, 100, 100);
    jj=nextButtonPress();
    if (jj==1) found=1;
  };
  
  whiteC=backC-1;
  axesC=whiteC;
  plotC=40;
  setColor(backC);
  fillRectangle(win,0,0,xpix,ypix);
  fillRectangle(pix, 0, 0, xpix, ypix);
  setColor(plotC);
}

void rrangeconst2D(){
  xspan=xmax-xmin;
  yspan=ymax-ymin;
  xinc=xspan/xpix;
  yinc=yspan/ypix;
  xmid=(xmax+xmin)/2;
  ymid=(ymax+ymin)/2;
  xorg=(-xmin*xpix)/xspan;
  yorg=(-ymin*ypix)/yspan;
}

void setLimits( int xn, int xx, int yn, int yx ){
  xmin=xn; xmax=xx; ymin=yn; ymax=yx;
  rrangeconst2D();
}

void clearGraph(){
  setColor(backC);
  fillRectangle(pix, 0, 0, xpix, ypix);
  setColor(axesC);
  if ((xmin < 0)&&(xmax > 0)) drawLine(pix, xorg, 0, xorg, ypix);
  if ((ymin < 0)&&(ymax > 0)) drawLine(pix, 0, yorg, xpix, yorg);
  redraw();
  setColor(plotC); 
}

  //the color is already set to plotC
  //you have to redraw() to see the points, or else use plotDirect()
void plot( double px, double py ){
  drawPoint( pix, ((px-xmin)*xpix)/xspan, ((ymax-py)*ypix)/yspan ); 
}

void plotDirect( double px, double py ){
  drawPoint( pix, ((px-xmin)*xpix)/xspan, ((ymax-py)*ypix)/yspan ); 
  drawPoint( win, ((px-xmin)*xpix)/xspan, ((ymax-py)*ypix)/yspan ); 
}

point2 conl;

void startcurve(point2 sc1){
  conl=sc1;
}

void connect(point2 conp){
  drawLine( pix,  (conl.x-xmin)*xpix/xspan, (ymax-conl.y)*ypix/yspan, (conp.x-xmin)*xpix/xspan, (ymax-conp.y)*ypix/yspan );    
  conl=conp;
}

void connectDirect(point2 conp){
  drawLine( pix,  (conl.x-xmin)*xpix/xspan, (ymax-conl.y)*ypix/yspan, (conp.x-xmin)*xpix/xspan, (ymax-conp.y)*ypix/yspan );    
  drawLine( win,  (conl.x-xmin)*xpix/xspan, (ymax-conl.y)*ypix/yspan, (conp.x-xmin)*xpix/xspan, (ymax-conp.y)*ypix/yspan );    
  conl=conp;
}