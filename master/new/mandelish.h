//required libraries: mgraphics.h  Change this name!

//note:  before using any of the functions here, the pixmap and/or window must
//specified to which the fractals are to be graphed, and the offset and 
//boundary lengths (xpix and ypix) for those Drawables as well.
//pmin, pmax, qmin, and qmax must also be defined, using compRange()
//q is always graphed in the verical direction, p in the horizontal.

double pmin, pmax, qmin, qmax;
double pspan, qspan;
double pmid, qmid;
double pinc, qinc;

  //set- and graphing constants
int maxit;
int power;
int redfac;
double bound;
comp ref;      //complex reference number used in different ways by the sets

//SYSTEM FUNCTIONS============================================================

  //xpix and ypix should already be defined if you are going to do any
  //graphing.
void compRange(double ppmin, double ppmax, double pqmin, double pqmax){
  pmin=ppmin;
  pmax=ppmax;
  qmin=pqmin;
  qmax=pqmax;
  pspan=pmax-pmin;
  qspan=qmax-qmin;
  pmid=pmin+(pspan/2);
  qmid=qmin+(qspan/2);
  pinc=pspan/xpix;
  qinc=qspan/ypix;
}

  //must be used before graphing a 'mandelic' set
void mandelConstants(comp pref, double pbound, int ppower, int pmaxit){
  ref=pref;
  power=ppower;
  bound=pbound;
  maxit=pmaxit;
}

void dotP( pixel d, int iters ){
  setmatrix[d.x/s.redfac][d.y/s.redfac]=iters;
  if (coloring == blend){
    color=highcol-int((iters/(s.maxit*1.0))*s.cconst*colspan);
  }else{
    cplace=int((iters/s.maxit)*s.cconst*6);
    while (cplace > colors-1) cplace=cplace-colors;
    color=colorList[colorlist][cplace];
  };
  if (color < lowcol) color=1;
  setColor(color);
  if (s.redfac==1) drawPoint(pix, d.x, d.y);
  else fillRectangle(pix, d.x-s.redfac, d.y-s.redfac, s.redfac, s.redfac);
}

//MANDELBROT GRAPHING========================================================

  //redfac is the reduction factor
void mandel(int redfac){
  comp C;
  int i, j;
  int posx, posy;
  comp z, zl;
  for (posx=0; posx <= xpix; posx=posx+redfac){
    C.p=pmin+(pinc*(posx+.5));
    if (checkStop()) return;
    for (posy=0; posy <= ypix; posy=posy+redfac){
      C.q=qmax-(qinc*(posy+.5));
      z = init;
      for (i=1; i<=maxit; i++){
        zt=cAdd( cExp(z, power), C);
        z=zt;
        if(dist(z) > bound){
          dotP(pos, i);
          i=maxit+1;
        };
      };
    };
    //copyArea(manpix, manwin, posx, 0, s.redfac, ypix, pos.x, 0);
  };
}

//JULIA GRAPHING===========================================================
