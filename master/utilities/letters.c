#include <fstream.h>
#include "graphics.h"

int i, j, k;
int backC, whiteC;

Window w;
Pixmap pix;

  //user-variables
int dotlen=10;
int dotsperside;
int const maxdots = 30;  //the maximum value for dotsperside
int winlen;
  
int setmatrix[maxdots][maxdots];
  
void redraw(){
  copyArea(pix, w, 0, 0, winlen, winlen, 0, 0);
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
  fillRectangle(dld, dscale*dlx, dscale*dly, dscale, dscale);
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
        k=getColor();
        if (!j) setColor(whiteC); else setColor(backC);
        drawLargePoint(pix, thisx, thisy, dotlen);
        drawLargePoint(w, thisx, thisy, dotlen);
        setColor(k);
      };
      if (e.value == 3) return;
    };
    if (e.type == Expose) redraw();
  };
}

int main(){
  fstream lifefile;
  char lifename[6]="_.map";
  int firsttime, found;
  char ch;
  
  cout << endl << "size of array = "; cin >> dotsperside; cout << endl;
  if (dotsperside > maxdots){
    cout << "Array is too large.. using" << maxdots << "X" << maxdots;
    cout << "array." << endl;
    dotsperside=maxdots;
  };
  winlen=dotlen*dotsperside;
  
  w = createWindow(0, 0, winlen, winlen, "Bitmap editor");
  pix = createPixmap(w, winlen, winlen);
  
  found = 0;
  backC=200;
  while (!found){
    backC++;
    setColor(backC);
    fillRectangle(w, 0, 0, winlen, winlen);
    j=nextButtonPress();
    if (j==1) found=1;
  };
  
  //backC=204;
  whiteC=backC-1;
  
  while (1==1){
    setColor(whiteC);
    fillRectangle(pix, 0, 0, winlen, winlen);
    redraw();
  
    resetMatrix();
    bitmapInput();

    cout << "What character will this represent? "; cin >> lifename[0]; 
    cout << endl;

    int imin=dotsperside-1, imax=0, jmin=dotsperside-1, jmax=0;
    for(i=0; i<dotsperside; i++){
      for(j=0; j<dotsperside; j++){
        if( setmatrix[i][j] == 1){
          if (i<imin) imin=i;
          if (i>imax) imax=i;
          //if (j<jmin) jmin=j;
          //if (j>jmax) jmax=j;
        };
      };
    };
  
    lifefile.open( lifename, ios::out);
    firsttime=1;
    //for (j=jmin; j<=jmax; j++){
    for (j=0; j<dotsperside; j++){
      if (!firsttime) lifefile.put(10);
      firsttime=0;
      for (i=imin; i<=imax; i++){
        ch = 48 + setmatrix[i][j];
        lifefile.put(ch);
      };
    };
    lifefile.close();
  };
}