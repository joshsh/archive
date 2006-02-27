#include <fstream.h>
#include "graphics.h"

int i, j, k;
int backC, whiteC;

Window win;
Pixmap pix;

  //user-variables
int dotlen=10;
int dotsperside=10;
int const maxdots = 10;  //the maximum value for dotsperside
int winlen;
  
int setmatrix[maxdots][maxdots];
  
void redraw(){
  copyArea(pix, win, 0, 0, winlen, winlen, 0, 0);
}

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

int nextButtonPress(){
  Event nbp;
  updateEvents();
  do{ 
    nbp=nextEvent();
    if (nbp.type == Expose) redraw();
  } while (nbp.type != ButtonPress );
  return nbp.value;
}

void resetMatrix(){
  for (i=0; i<dotsperside; i++){
    for (j=0; j<dotsperside; j++) setmatrix[i][j]=0;
  };
}

void drawLargePoint(Drawable dld, int dlx, int dly, int dscale ){

}

void bitmapInput(){
  int thisx, thisy;
  int box, backCt;
  Event e;
  updateEvents();
  setColor(backC);
  int stop=0;
  while(!stop){
    e=getNextEvent();
    if (e.type == ButtonPress){
      if (e.value ==1 ){
        thisx=e.x/dotlen;
        thisy=e.y/dotlen;
        i=setmatrix[thisx][thisy];
        if (!i) j=1; else j=0;
        setmatrix[thisx][thisy]=j;
        setmatrix[thisy][thisx]=j;
        k=getColor();
        if (!j) setColor(70); else setColor(40);
        fillRectangle(pix, dotlen*thisx, dotlen*thisy, dotlen, dotlen);
        fillRectangle(win, dotlen*thisx, dotlen*thisy, dotlen, dotlen);
        fillRectangle(pix, dotlen*thisy, dotlen*thisx, dotlen, dotlen);
        fillRectangle(win, dotlen*thisy, dotlen*thisx, dotlen, dotlen);
        setColor(k);
      };
      if (e.value ==2 ){
        thisx=e.x/dotlen;
        thisy=e.y/dotlen;
        i=setmatrix[thisx][thisy];
        if (!i) j=2; else j=0;
        setmatrix[thisx][thisy]=j;
        k=getColor();
        if (!j) setColor(whiteC); else setColor(40);
        fillRectangle(pix, dotlen*thisx, dotlen*thisy, dotlen, dotlen);
        fillRectangle(win, dotlen*thisx, dotlen*thisy, dotlen, dotlen);
        fillRectangle(pix, dotlen*thisy, dotlen*thisx, dotlen, dotlen);
        fillRectangle(win, dotlen*thisy, dotlen*thisx, dotlen, dotlen);
        setColor(k);
      };
      if (e.value == 3) return;
    };
    if (e.type == Expose) redraw();
  };
}

int main(){
  winlen=dotlen*dotsperside;
  
  win = createWindow(0, 0, winlen, winlen, "Bitmap editor");
  pix = createPixmap(win, winlen, winlen);

  int found = 0;
  backC=200;
  while (!found){
    backC++;
    setColor(backC);
    fillRectangle(win, 0, 0, winlen, winlen);
    j=nextButtonPress();
    if (j==1) found=1;
  };
  
  whiteC=backC-1;
  setColor(whiteC);
  fillRectangle(pix, 0, 0, winlen, winlen);
  redraw(); 
  resetMatrix();
  bitmapInput();
}