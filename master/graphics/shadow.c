#include "graphics.h"

int i, j;

struct pixel{
  int x;
  int y;
};

Window w;

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

int const xpix = 500;
int const ypix = 500;
int setmatrix[xpix][ypix];

void resetMatrix(){
  for (i=0; i<xpix; i++){
    for (j=0; j<ypix; j++) setmatrix[i][j]=0;
  };
}  

int lcol=140, scol=lcol-30;
pixel posA, posB, posC;
int const shadows=12;
int size=20;
int blen=xpix/size;
int lowcolor, notfound;
int dir;

void shadowBox(pixel posA){
  setColor(setmatrix[posA.x][posA.y]);
  fillRectangle(w,posA.x*blen,posA.y*blen,blen,blen);
}

pixel newpos(pixel posA){
  posB=posA;
  lowcolor=lcol;
  if (setmatrix[posA.x][posA.y-1]<lowcolor){dir = 1;
    lowcolor=setmatrix[posA.x][posA.y-1];};
  if (setmatrix[posA.x-1][posA.y]<lowcolor){dir = 4;
    lowcolor=setmatrix[posA.x-1][posA.y];};
  if (setmatrix[posA.x][posA.y+1]<lowcolor){dir = 3; 
    lowcolor=setmatrix[posA.x][posA.y+1];};
  if (setmatrix[posA.x+1][posA.y]<lowcolor){dir = 2;
    lowcolor=setmatrix[posA.x+1][posA.y];};
  if (lowcolor == lcol) dir = 0;
  switch (dir){
    case 1: {posB.y=posA.y-1; break;};
    case 3: {posB.y=posA.y+1; break;};
    case 4: {posB.x=posA.x-1; break;};
    case 2: {posB.x=posA.x+1; break;};
    case 0: break;
  };
  setmatrix[posB.x][posB.y]=lcol;
  shadowBox(posB);
  return posB;
}

void newsquare(){
  for (i=0;i<size;i++){
    for (j=0;j<size;j++){
      posB.x=i;
      posB.y=j;
      shadowBox(posB);
    };
  };
}

pixel startpos(){
  notfound=1;
  while (notfound==1){
    i=drand48()*size;
    j=drand48()*size;
    if (setmatrix[i][j] == scol) notfound=0;
  };
  posC.x=i;
  posC.y=j;
  setmatrix[i][j]=lcol;
  shadowBox(posC);  
  return posC;
}

void lighten(){
  for (i = 1; i < size-1; i++){
    for (j = 1; j < size-1; j++){
      if (setmatrix[i][j]>scol) setmatrix[i][j]=setmatrix[i][j]-1;
    };
  };  
  newsquare();
}

int main(){
  int steps, thisbox;
  pixel posList[shadows]; 

  w=createWindow( 500, 500, xpix, ypix, "shadow tracers" );
  resetMatrix();

  for (i=0;i<size;i++){
    for (j=0;j<size;j++) setmatrix[i][j]=lcol;
  };
  for (i=1;i<size-1;i++){
    for (j=1;j<size-1;j++) setmatrix[i][j]=scol;
  };

  //set screen
  setColor(lcol);
  fillRectangle(w, 0, 0, xpix, ypix);
  for (i=0;i<size;i++){
    for (j=0;j<size;j++){
      posB.x=i;
      posB.y=j;
      shadowBox(posB);
    };
  };

  int ii;
  for (ii=0; ii< shadows; ii++) posList[ii]=startpos();

  while (1==1){
    lighten();
    for (ii=0; ii<shadows; ii++) posList[ii]=newpos(posList[ii]);
  };
}
