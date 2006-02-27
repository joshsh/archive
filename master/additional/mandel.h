struct set{ 
  int mode, method, maxit, Power, redfac;
  double pmin, pmax, qmin, qmax, pspan, qspan, pinc, qinc, bound, cconst;
  comp init, c;
};

struct range{
  set s;
  Pixmap pix;
};

int color;
int ranges=5;
int windows=20;
range rangeList[5];  //ranges!
int windowList[20];  //windows!
int windowplace=-1;
int windowlabel=-1;
int Power;
int listplace=-1;
int rangeplace=0;
int maxiters=20000; //iterations which openClosed() considers bounded
int maxtries=5000; //the number of tries after which openClosed() gives up.
int maxtries2;    //for openClosedSet().
//int lowcol=1, highcol=130, colspan=highcol-lowcol+1;
int lowcol, highcol, colspan;
int blends = 4;
int blendList[4][2];  //[blends][2]!
int colorlists=4, colors=6;
int colorList[4][6]; //[colorlists][colors]!
int colmenu[8]; //[blends+colorlists]!
int plotC=30;
int colorlist;
int blend=1, list=2;
double radius;
int maxit, redfac;
double cconst;

//area constants
int cons, windo, zoomo, zoomi, point, newse, openc, quit, refre, infor;

//area group constants
int IFSmenu, colormenu;

//set constants
int mandel=1, julia=2, newton=3, halley=4, unity=5;
  int exper1=6, exper2=7, exper3=8, exper4=9;

//method constants
int bound=1, conv=2, openbound=3, openconv=4, plot=5, cloud=6, norm=7;

//set group constants
int IFS=1, orbit=2, growth=3;

int steps, cplace;
comp z,zt;
comp cor[2];
comp C, zp, zpt;
//int roots=50;
//comp rootList[50];  //roots!
set c, s;
pixel pos;

//menu positions
pixel IFSB, orbitB, growthB, IFSrB, coastB, distanceB, landB, experB;
pixel mandelB, juliaB, newtonB, halleyB, exper1B, exper2B, exper3B, unityB;
  pixel othersB;
pixel boundB, convB, openboundB, openconvB, plotB, cloudB, normB;
pixel constB, inforB, newseB, opencB, pointB, refreB, windoB, zoomiB, zoomoB;
pixel colorB;
pixel rotatB;

void draw( set drawset );
void getColors();
void information();
void openClosed();
int openClosedSet();
void openGraph();
void openInputs();

void dot( pixel d, int iters ){
  setmatrix[d.x/s.redfac][d.y/s.redfac]=iters;
  if (coloring == blend) color=highcol-int((iters/(s.maxit*1.0))*s.cconst*colspan);
  else{
    cplace=int((iters/s.maxit)*s.cconst*6);
    while (cplace > colors-1) cplace=cplace-colors;
    color=colorList[colorlist][cplace];
  };
  if (color < lowcol) color=1;
  setColor(color);
  if (s.redfac==1){drawPoint(win,d.x, d.y); drawPoint(pix, d.x, d.y);}
  else{ 
    fillRectangle(pix, d.x-s.redfac, d.y-s.redfac, s.redfac, s.redfac);
    fillRectangle(win, d.x-s.redfac, d.y-s.redfac, s.redfac, s.redfac);};
}

void dotP( pixel d, int iters ){
  setmatrix[d.x/s.redfac][d.y/s.redfac]=iters;
  if (coloring == blend) color=highcol-int((iters/(s.maxit*1.0))*s.cconst*colspan);
  else{
    cplace=int((iters/s.maxit)*s.cconst*6);
    while (cplace > colors-1) cplace=cplace-colors;
    color=colorList[colorlist][cplace];
  };
  if (color < lowcol) color=1;
  setColor(color);
  if (s.redfac==1) drawPoint(pix, d.x, d.y);
  else fillRectangle(pix, d.x-s.redfac, d.y-s.redfac, s.redfac, s.redfac);
}

void dotPS( pixel d, double dnorm ){
  int iters=int((s.maxit*dnorm)/(s.bound));
  setmatrix[d.x/s.redfac][d.y/s.redfac]=iters;
  if (coloring == blend) color=highcol-int((dnorm/(s.bound))*s.cconst*colspan);
  else{
    cplace=int((dnorm/s.bound)*s.cconst*6);
    while (cplace > colors-1) cplace=cplace-colors;
    color=colorList[colorlist][cplace];
  };
  if (color < lowcol) color=1;
  setColor(color);
  if (s.redfac==1) drawPoint(pix, d.x, d.y);
  else fillRectangle(pix, d.x-s.redfac, d.y-s.redfac, s.redfac, s.redfac);
}

void recolor(){
  updateEvents();
  writeStopM();
  comment( "Re - coloring screen. " );
  if (coloring==blend){
    for (i=0; i<= xpix; i=i+s.redfac){
      if ( checkStop() ) return;
      for (j=0; j <= ypix; j=j+s.redfac){ 
        color=highcol-int((setmatrix[i/s.redfac][j/s.redfac]/(s.maxit*1.0))*s.cconst*colspan);
        if (color < lowcol) color=lowcol;
        if ( setmatrix[i/s.redfac][j/s.redfac]==0 ) color=backC;
        setColor(color);
        if (s.redfac==1) drawPoint(pix, i, j);
        else  fillRectangle(pix, i-s.redfac, j-s.redfac, s.redfac, s.redfac);
      };
    };
  };
  if (coloring==list){
    for (i=0; i<= xpix; i=i+s.redfac){
      if ( checkStop() ) return;
      for (j=0; j <= ypix; j=j+s.redfac){
        cplace=int((setmatrix[i/s.redfac][j/s.redfac]/s.maxit)*s.cconst*6);
        while (cplace > colors-1) cplace=cplace-colors;
        color=colorList[colorlist][cplace];
        if ( setmatrix[i/s.redfac][j/s.redfac]==0 ) color=backC;
        setColor(color);
        if (s.redfac==1) drawPoint(pix, i, j);
        else fillRectangle(pix, i-s.redfac, j-s.redfac, s.redfac, s.redfac);
      };
    };
  };
  redraw();
}

void dotR( pixel d, int iters ){  //this was "double iters".  Why?
  setmatrix[d.x/redfac][d.y/redfac]=iters;
  if (coloring == blend) color=highcol-int((iters/(maxit*1.0))*cconst*colspan);
  else{
    cplace=int((iters/maxit)*cconst*6);
    while (cplace > colors-1) cplace=cplace-colors;
    color=colorList[colorlist][cplace];
  };
  if (color < lowcol) color=1;
  setColor(color);
  if (redfac==1){drawPoint(win,d.x, d.y); drawPoint(pix, d.x, d.y);}
  else{ 
    fillRectangle(pix, d.x-redfac, d.y-redfac, redfac, redfac);
    fillRectangle(win, d.x-redfac, d.y-redfac, redfac, redfac);};
}

void recolorR(){
  updateEvents();
  writeStopM();
  comment( "Re - coloring screen. " );
  if (coloring==blend){
    for (i=0; i<= xpix; i=i+redfac){
      if ( checkStop() ) return;
      for (j=0; j <= ypix; j=j+redfac){
        color=highcol-int((setmatrix[i/redfac][j/redfac]/(maxit*1.0))*cconst*colspan);
        if (color < lowcol) color=lowcol;
        if ( setmatrix[i/redfac][j/redfac]==0 ) color=backC;
        setColor(color);
        if (redfac==1) drawPoint(pix, i, j);
        else fillRectangle(pix, i-redfac, j-redfac, redfac, redfac);
      };
    };
  };
  if (coloring==list){
    for (i=0; i<= xpix; i=i+redfac){
      if (checkStop()) return;
      for (j=0; j <= ypix; j=j+redfac){
        cplace=int((setmatrix[i/redfac][j/redfac]/maxit)*cconst*6);
        while (cplace > colors-1) cplace=cplace-colors;
        color=colorList[colorlist][cplace];
        if ( setmatrix[i/redfac][j/redfac]==0 ) color=backC;
        setColor(color);
        if (redfac==1) drawPoint(pix, i, j);
        else fillRectangle(pix, i-redfac, j-redfac, redfac, redfac);
      };
    };
  };
  redraw();
}

double dist(comp pl){
  return sqrt((pl.p*pl.p)+(pl.q*pl.q));
}

void addRangeList(){
  listplace++;
  //windowplace++;
  //windowlabel=windowplace;
  if (listplace >= ranges){
    listplace=listplace-1;
    for (i=1; i<ranges; i++) rangeList[i-1]=rangeList[i];
  //  for (i=0; i<windows; i++) windowList[i]=windowList[i]-1;
  };
  rangeList[listplace].s=c;
  copyArea( pix, rangeList[listplace].pix, 0, 0, xpix, ypix, 0, 0);
  windowplace=listplace;
  //windowList[windowplace]=listplace;
}

forePage(){
  windowplace++;
  c=rangeList[windowplace].s;
  copyArea(rangeList[windowplace].pix, win, 0, 0, xpix, ypix, 0, 0);
  copyArea(rangeList[windowplace].pix, pix, 0, 0, xpix, ypix, 0, 0);
}

backPage(int back){
  if (back) windowplace=windowplace-1;
  c=rangeList[windowplace].s;
  copyArea(rangeList[windowplace].pix, win, 0, 0, xpix, ypix, 0, 0);
  copyArea(rangeList[windowplace].pix, pix, 0, 0, xpix, ypix, 0, 0);
}

int lastPage(){
  int windowchanged=1;
  int lwcont=0;
  while (lwcont == 0){
    lwcont=1;
    comment( "Range list is full. " );
    if (windowList[windowlabel] < 0){
      windowplace=windowplace-1;
      lwcont=0;
      for (i=windowlabel; i<windows-1; i++)
        windowList[windowlabel]=windowList[windowlabel+1];
      windowlabel=windowlabel-1;
      if (windowlabel < 0) windowlabel = 0;
    };
  };
  windowlabel=windowlabel-1;
  if (windowlabel < 0){ 
    windowlabel=0;
    comment( "Going back is not possible. " );
    windowchanged=0;
  };
   if (windowchanged){
    c=rangeList[windowList[windowlabel]].s;
  };
  return windowchanged;
}

set rangeConstF( set rc1 ){
  rc1.pspan=rc1.pmax-rc1.pmin;
  rc1.qspan=rc1.qmax-rc1.qmin;
  rc1.pinc=rc1.pspan/xpix;
  rc1.qinc=rc1.qspan/ypix;
  return rc1;
}

set rangeF( set r1 ){
  setColor( textC );
  r1.pmin=inputNumber( "p min = " );
  r1.pmax=inputNumber( "p max = " );
  r1.qmin=inputNumber( "q min = " );
  r1.qmax=inputNumber( "q max = " );
  newLine();
  r1 = rangeConstF( r1 );
  return r1;
}

set getRangeF( set source, set target){
  target.pmin=source.pmin;
  target.pmax=source.pmax;
  target.qmin=source.qmin;
  target.qmax=source.qmax;
  target = rangeConstF( target );
  return target;
}

void IFSconstants(){
  setColor( textC );
  comment( "Enter the set constants on the page. " );
  newLine();
  c.maxit=inputInt( "number of iterations = " );
  if (c.mode >= 10) maxtries2=inputInt( "maximum point probes = " );
  int ask=1;
  if ((c.mode==mandel)||(c.mode==julia)){
    c.bound=3;
    ask=0;
  };
  if (c.mode==norm) ask=1;
  if (ask) c.bound=inputNumber( "critical magnitude = " );
  c.cconst=inputNumber( "color constant = " );
  ask=1;
  if ((c.method == plot)||(c.method == cloud)) ask=0;
  c.redfac = 1;
  if (ask) c.redfac=inputInt( "reduce detail by a factor of... " );
  newLine();
  if (c.redfac < 1){
    printLine( "! The reduction factor must be >= 1. Assuming factor 1. " );
    newLine();
    c.redfac = 1;
  };
  if (c.redfac > xpix){
    printLine( "! Aside from being pointless, your reduction factor is also" );
    printLine( "impossible. Assuming a factor of 50. " ); newLine();
    c.redfac=50;
    wait();
  };
  newLine();
  while ( xpix/(s.redfac) != xpix/s.redfac ) s.redfac=s.redfac-1;
}

  //there are (colors) cli's.
void newColorList(int place,int cl0,int cl1,int cl2,int cl3,int cl4,int cl5){
  colorList[place][0]=cl0;
  colorList[place][1]=cl1;
  colorList[place][2]=cl2;
  colorList[place][3]=cl3;
  colorList[place][4]=cl4;
  colorList[place][5]=cl5;
}

void newBlend(int place, int nblow, int nbhigh){
  blendList[place][0]=nblow;
  blendList[place][1]=nbhigh;
}

void writeIFSMenu(){
  clearMenu();
  setColor( textC );
  putMenu( colorB, "colors" );
  putMenu( constB, "constants" );
  putMenu( inforB, "information" );
  putMenu( newseB, "new set" );
  putMenu( opencB, "open/closed set" );
  putMenu( pointB, "point analysis" );
  putMenu( refreB, "refresh screen" );
  putMenu( windoB, "window" );
  putMenu( zoomiB, "zoom in" );
  putMenu( zoomoB, "zoom out" );
}

void colorMenu(){
  colormenu=startGroup();
  for (i=0; i < colorlists+blends; i++){
    colmenu[i]=addList( xpix, ypix+panelheight-menuheight+int(i*menuheight/((colorlists+blends)*1.0)), pagelength, int(menuheight/((colorlists+blends)*1.0)));
  };
}

void drawColors(){
  double aa=menuheight/(colorlists+blends), bb=pagelength/6, cc;
  int dd=ypix+panelheight-menuheight;
  for (i=0; i<blends; i++){
    cc=pagelength/((blendList[i][1]-blendList[i][0])*1.0);
    for( j=blendList[i][0]; j <= blendList[i][1]; j++){
      setColor(j);
      fillRectangle(win, xpix+int((j*cc)), dd+int((i*aa)), int(cc)+1, int(aa));
    };
  };
  for (i=0; i<colorlists; i++){
    for (j=0; j<colors; j++){
      setColor( colorList[i][j] );
      fillRectangle(win, xpix+int((j*bb)), dd+int((i+blends)*aa), int(bb), int(aa));
    };
  };
}

comp pixelComp( pixel pp1 ){
  comp pp2;
  pp2.p=s.pmin+(s.pinc*(pp1.x+.5));
  pp2.q=s.qmax-(s.qinc*(pp1.y+.5));
  return pp2;
}

pixel compPixel( comp pp1 ){
  pixel pp2;
  pp2.x=int((pp1.p-s.pmin)/s.pinc);
  pp2.y=int((s.qmax-pp1.q)/s.pinc);
  return pp2;
}

void zoom(){
  int pmins, pmaxs, qmins, qmaxs;
  double ppmin, ppmax, pqmin, pqmax;
  pixel cor1, cor2;
  Event e;
  comment( "Use the mouse to choose a new viewing window. " ); 
  setColor(backC-1);
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
  pmaxs=cor2.x; pmins=cor1.x; 
  if (cor1.x > cor2.x){ pmaxs=cor1.x; pmins=cor2.x;};
  qmaxs=cor2.y; qmins=cor1.y; 
  if (cor1.y < cor2.y){ qmaxs=cor1.y; qmins=cor2.y;};
  drawRectangle( win, cor1.x, cor1.y, 0, 0 );
  while ( (e.type != ButtonPress) || (e.value != 1) ){
    e = nextEvent(); 
    if (e.type == Expose) redraw();
    drawRectangle( win, pmins, qmaxs, pmaxs-pmins, qmins-qmaxs );
    cor2.x = e.x; 
    cor2.y = e.y;
    pmaxs=cor2.x; pmins=cor1.x; 
    if (cor1.x > cor2.x){ pmaxs=cor1.x; pmins=cor2.x;};
    qmaxs=cor2.y; qmins=cor1.y; 
    if (cor1.y < cor2.y){ qmaxs=cor1.y; qmins=cor2.y;};
    drawRectangle( win, pmins, qmaxs, pmaxs-pmins, qmins-qmaxs );
  };
  setDrawMode( GXcopy );
  drawRectangle( win, pmins, qmaxs, pmaxs-pmins, qmins-qmaxs );
  pmaxs=cor2.x; pmins=cor1.x; 
  if (cor1.x > cor2.x){ pmaxs=cor1.x; pmins=cor2.x;};
  qmaxs=cor2.y; qmins=cor1.y; 
  if (cor1.y < cor2.y){ qmaxs=cor1.y; qmins=cor2.y;};
  ppmin=c.pmin+((pmins+.5)*c.pspan/xpix);
  ppmax=c.pmin+((pmaxs+.5)*c.pspan/xpix);
  pqmin=c.qmax-((qmins+.5)*c.qspan/ypix);
  pqmax=c.qmax-((qmaxs+.5)*c.qspan/ypix);
  c.pmin=ppmin; c.pmax=ppmax; c.qmin=pqmin; c.qmax=pqmax;
  newLine();
  resetPage();
  setColor( textC );
  printString( "p min = " ); printNumber( c.pmin ); newLine();
  printString( "p max = " ); printNumber( c.pmax ); newLine();
  printString( "q min = " ); printNumber( c.qmin ); newLine();
  printString( "q max = " ); printNumber( c.qmax ); newLine();
  c = rangeConstF( c );
}

void zoomOut(){
  int pmins, pmaxs, qmins, qmaxs;
  pixel cor1, cor2;
  Event e;
  set n = c;
  n.pmin=c.pmin-(c.pspan*2);
  n.pmax=c.pmax+(c.pspan*2);
  n.qmin=c.qmin-(c.qspan*2);
  n.qmax=c.qmax+(c.qspan*2);
  n = rangeConstF( n );
  n.redfac=xpix/25;
  draw( n );
  comment( "Use the mouse to choose a new viewing window. " );
  setColor(backC-1);
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
  pmaxs=cor2.x; pmins=cor1.x; 
  if (cor1.x > cor2.x){ pmaxs=cor1.x; pmins=cor2.x;};
  qmaxs=cor2.y; qmins=cor1.y; 
  if (cor1.y < cor2.y){ qmaxs=cor1.y; qmins=cor2.y;};
  drawRectangle( win, cor1.x, cor1.y, 0, 0 );
  while ( (e.type != ButtonPress) || (e.value != 1) ){
    e = nextEvent();
    if (e.type == Expose) redraw(); 
    drawRectangle( win, pmins, qmaxs, pmaxs-pmins, qmins-qmaxs );
    cor2.x = e.x; 
    cor2.y = e.y;
    pmaxs=cor2.x; pmins=cor1.x; 
    if (cor1.x > cor2.x){ pmaxs=cor1.x; pmins=cor2.x;};
    qmaxs=cor2.y; qmins=cor1.y; 
    if (cor1.y < cor2.y){ qmaxs=cor1.y; qmins=cor2.y;};
    drawRectangle( win, pmins, qmaxs, pmaxs-pmins, qmins-qmaxs );
  };
  setDrawMode( GXcopy );
  drawRectangle( win, pmins, qmaxs, pmaxs-pmins, qmins-qmaxs );
  pmaxs=cor2.x; pmins=cor1.x; 
  if (cor1.x > cor2.x){ pmaxs=cor1.x; pmins=cor2.x;};
  qmaxs=cor2.y; qmins=cor1.y; 
  if (cor1.y < cor2.y){ qmaxs=cor1.y; qmins=cor2.y;};
  c.pmin=n.pmin+((pmins+.5)*n.pspan/xpix);
  c.pmax=n.pmin+((pmaxs+.5)*n.pspan/xpix);
  c.qmin=n.qmax-((qmins+.5)*n.qspan/ypix);
  c.qmax=n.qmax-((qmaxs+.5)*n.qspan/ypix);
  newLine();
  resetPage();
  setColor( textC );
  printString( "p min = " ); printNumber( c.pmin ); newLine();
  printString( "p max = " ); printNumber( c.pmax ); newLine();
  printString( "q min = " ); printNumber( c.qmin ); newLine();
  printString( "q max = " ); printNumber( c.qmax ); newLine();
  c = rangeConstF( c );
}

/*void clearRootList(){
  for (i=0; i<roots; i++){
    rootList[i].p=1.111;
    rootList[i].q=1.111;
  };
}*/

char *setName( int snmode ){
  if (snmode == mandel) return "the Mandelbrot set";
  if (snmode == julia) return "the Julia set";
  if (snmode == halley) return "the Halley set";
  if (snmode == newton) return "the Newton set";
  if (snmode == exper1) return "the Experimental set #1";
  if (snmode == exper2) return "the Experimental set #2";
  if (snmode == exper3) return "the Experimental set #3";
  if (snmode == exper4) return "the Experimental set #4";
  if (snmode == unity) return "the Unity set";
  return "n";
}

void dmandel(){
  int stop, stop2;
  comp zl;
  Event e;
  if (s.method == bound){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      C.p=s.pmin+(s.pinc*(pos.x+.5));
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        C.q=s.qmax-(s.qinc*(pos.y+.5));
        z = s.init;
        for (i=1; i<=s.maxit; i++){
          zt=cAdd( cExp(z, s.Power), C);
          z=zt;
          if(dist(z) > s.bound){
            dotP(pos, i);
            i=s.maxit+1;
          };  
        };
      };
      copyArea(pix, win, pos.x, 0, s.redfac, ypix, pos.x, 0);
    };
  };
  if (s.method == norm){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      C.p=s.pmin+(s.pinc*(pos.x+.5));
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        C.q=s.qmax-(s.qinc*(pos.y+.5));
        z = s.init;
        for (i=1; i<=s.maxit; i++){
          zt=cAdd( cExp(z, s.Power), C);
          z=zt;
        };
        dotPS(pos, dist(z));
      };
      copyArea(pix, win, pos.x, 0, s.redfac, ypix, pos.x, 0);
    };
  };
  /*if (s.method == bound){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      C.p=s.pmin+(s.pinc*(pos.x+.5));
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        C.q=s.qmax-(s.qinc*(pos.y+.5));
        z = s.init;
        if (checkStop()) return;
        for (i=1; i<=s.maxit; i++){
          zt=cAdd( cExp(z, s.Power), C);
          z=zt;
          if(dist(z) > s.bound){
            dot(pos, i);
            i=s.maxit+1;
          };  
        };
      };
    };
  };*/
  if (s.method == conv){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      C.p=s.pmin+(s.pinc*(pos.x+.5));
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        C.q=s.qmax-(s.qinc*(pos.y+.5));
        z = s.init;
        for (i=1; i<=s.maxit; i++){
          zl=z;
          z=cAdd( cExp(z, s.Power), C);
          if( cDiff(z, zl) < s.bound){
            dot(pos, i);
            i=s.maxit+1;
          };  
        };
      };
    };
  };
  if (s.method == plot){
    stop=0;
    while (!stop){     
      C=pixelComp( readPoint() );
      z=s.init;
      stop2=0;
      i=0;
      while (!stop2){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.y=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        i++;
        z=cAdd( cExp(z, s.Power), C);
        if (dist(z) > s.bound) i = s.maxit;
        while ( checkEvent() ){
          e=nextEvent();
          if (e.type == Expose) redraw();
          if ((e.type == ButtonPress)||( e.value == 3 )) stop2=1;
        };
      };
      while ( checkEvent() ){
        e=nextEvent();
        if (e.type == Expose) redraw();
        if ((e.type == ButtonPress)||( e.value == 3 )) stop=1;
      };
    };
  };
  if (s.method == cloud){
    while (!checkStop()){
      C.p=s.pmin+(drand48()*s.pspan);
      C.q=s.qmin+(drand48()*s.qspan);
      z=s.init;
      for ( i=0; i < s.maxit; i++ ){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.y=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        z=cAdd( cExp(z, s.Power), C);
        if (dist(z) > s.bound) i = s.maxit;
      };
    };
  };
}

int mandelP(comp zpp, int iters){
  C=zpp;
  zp=s.init;
  steps=0;
  for (i=1; i<=iters; i++){
    zp=cAdd( cExp(zp, 2), C);
    if (dist(zp) > s.bound){ steps=i; i=iters+1;};
  };
  return steps;
}

void djulia(){
  int stop, stop2;
  comp zl;
  Event e;
  if (s.method == bound){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        z.p=s.pmin+(s.pinc*(pos.x+.5));
        z.q=s.qmax-(s.qinc*(pos.y+.5));
        for (i=1; i<=s.maxit; i++){
          z=cAdd( cExp(z, s.Power), s.c);
          if (dist(z) > s.bound){ dot(pos,i); i=s.maxit+1;};  
        };
      };
    };
  };
  if (s.method == conv){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        z.p=s.pmin+(s.pinc*(pos.x+.5));
        z.q=s.qmax-(s.qinc*(pos.y+.5));
        for (i=1; i<=s.maxit; i++){
          zl=z;
          z=cAdd( cExp(z, s.Power), s.c);
          if (cDiff(z, zl) > s.bound){ dot(pos,i); i=s.maxit+1;};  
        };
      };
    };
  };
  if (s.method == plot){
    stop=0;
    while (!stop){     
      z=pixelComp( readPoint() );
      stop2=0;
      i=0;
      while (!stop2){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.y=int(ypix-((z.q-s.qmin)*ypix/s.qspan));
        dot(pos, i);
        i++;
        z=cAdd( cExp(z, s.Power), s.c);
        if (dist(z) > s.bound) i = s.maxit;
        while ( checkEvent() ){
          e=nextEvent();
          if (e.type == Expose) redraw();
          if ((e.type == ButtonPress)||( e.value == 3 )) stop2=1;
        };
      };
      while ( checkEvent() ){
        e=nextEvent();
        if (e.type == Expose) redraw();
        if ((e.type == ButtonPress)||( e.value == 3 )) stop=1;
      };
    };
  };
 if (s.method == cloud){
    while (!checkStop()){
      z.p=s.pmin+(drand48()*s.pspan);
      z.q=s.qmin+(drand48()*s.qspan);
      for ( i=0; i<s.maxit; i++ ){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.x=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        z=cAdd( cExp(z, s.Power), s.c);
        if (dist(z) > s.bound) i = s.maxit;
      };
    };
  };
}

int juliaP(comp zpp, int iters){
  zp=zpp;
  steps=0;
  for (i=1; i<=iters; i++){
    zp=cAdd( cExp(zp, s.Power), s.c);
    if (dist(zp) > s.bound){ steps=i; i=iters+1;};
  };
  return steps;
} 

void dhalley(){
  int stop, stop2;
  comp zl;
  Event e;
  if (s.method == bound){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        z.p=s.pmin+(s.pinc*(pos.x+.5));
        z.q=s.qmax-(s.qinc*(pos.y+.5));
        for (i=1; i<=s.maxit; i++){
          zl=z;
          z= cMult( z, cSub( cExp( z, s.Power ), s.c ));
          if(dist(z) > s.bound){
            dot(pos, i);
            i=s.maxit+1;
          };  
        };
      };
    };
  };
  if (s.method == conv){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        z.p=s.pmin+(s.pinc*(pos.x+.5));
        z.q=s.qmax-(s.qinc*(pos.y+.5));
        for (i=1; i<=s.maxit; i++){
          zl=z;
          z= cMult( z, cSub( cExp( z, s.Power ), s.c ));
          //if(cDiff(z, zl) < s.bound){
          if ( fabs( sq(cAbs(z))-sq(cAbs(zl))) ){
            dot(pos, i);
            i=s.maxit+1;
          };  
        };
      };
    };
  };
  if (s.method == plot){
    stop=0;
    while (!stop){     
      z=pixelComp( readPoint() );
      stop2=0;
      i=0;
      while (!stop2){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.x=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        i++;
        z=cMult( z, cSub( cExp( z, s.Power ), s.c ));
        if (dist(z) > s.bound) i = s.maxit;
        while ( checkEvent() ){
          e=nextEvent();
          if (e.type == Expose) redraw();
          if ((e.type == ButtonPress)||( e.value == 3 )) stop2=1;
        };
      };
      while ( checkEvent() ){
        e=nextEvent();
        if (e.type == Expose) redraw();
        if ((e.type == ButtonPress)||( e.value == 3 )) stop=1;
      };
    };
  };
  if (s.method == cloud){
    while (!checkStop()){ 
      z.p=s.pmin+(drand48()*s.pspan);
      z.q=s.qmin+(drand48()*s.qspan);
      for ( i=0; i<s.maxit; i++ ){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.x=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        z=cMult( z, cSub( cExp( z, s.Power), s.c ));
        if (dist(z) > s.bound) i = s.maxit;
      };
    };
  };
}

void dlambda(){
  for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
    if (checkStop()) return;
    for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
      z.p=s.pmin+(s.pinc*(pos.x+.5));
      z.q=s.qmax-(s.qinc*(pos.y+.5));
      for (i=1; i<=s.maxit; i++){
        z= cMult( cMult(s.c, z), cSub( cOne, z));
        if(dist(z) > s.bound){
          dot(pos, i);
          i=s.maxit+1;
        };  
      };
    };
  };
}

int lambdaP(comp zpp, int iters){
  zp=zpp;
  steps=0;
  for (i=1; i<=iters; i++){
    zp=cMult( cMult(s.c, z), cSub( cOne, z));
    if (dist(zp) > s.bound){ steps=i; i=iters+1;};
  };
  return steps;
} 

void dnewton(){
  int stop, stop2;
  comp zl;
  Event e;
  if (s.method == bound){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        z.p=s.pmin+(s.pinc*(pos.x+.5));
        z.q=s.qmax-(s.qinc*(pos.y+.5));
        for (i=1; i<=s.maxit; i++){
          z=cSub( cExp(z, s.Power), s.c);
          if (dist(z) > s.bound){ dot(pos,i); i=s.maxit+1;};  
        };
      };
    }; 
  };
  if (s.method == conv){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        z.p=s.pmin+(s.pinc*(pos.x+.5));
        z.q=s.qmax-(s.qinc*(pos.y+.5));
        for (i=1; i<=s.maxit; i++){
          zl=z;
          z=cSub( cExp(z, s.Power), s.c);
          if (cDiff(z, zl) < s.bound){ dot(pos,i); i=s.maxit+1;};  
        };
      };
    }; 
  };
  if (s.method == plot){
    stop=0;
    while (!stop){     
      z=pixelComp( readPoint() );
      stop2=0;
      i=0;
      while (!stop2){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.x=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        i++;
        z=cSub( cExp(z, s.Power), s.c);
        if (dist(z) > s.bound) i = s.maxit;
        while ( checkEvent() ){
          e=nextEvent();
          if (e.type == Expose) redraw();
          if ((e.type == ButtonPress)||( e.value == 3 )) stop2=1;
        };
      };
      while ( checkEvent() ){
        e=nextEvent();
        if (e.type == Expose) redraw();
        if ((e.type == ButtonPress)||( e.value == 3 )) stop=1;
      };
    };
  };
  if (s.method == cloud){
    while (!checkStop()){
      z.p=s.pmin+(drand48()*s.pspan);
      z.q=s.qmin+(drand48()*s.qspan);
      for ( i=0; i<s.maxit; i++ ){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.x=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        z=cSub( cExp(z, s.Power), s.c);
        if (dist(z) > s.bound) i = s.maxit;
      };
    };
  };
}

void dunity(){
  int stop, stop2;
  double One;
  comp u1,z;
  Event u2;
  pixel u3;
  if (s.method == bound){
    for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
      if (checkStop()) return;
      for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
        z.p=s.pmin+(s.pinc*(pos.x+.5));
        z.q=s.qmax-(s.qinc*(pos.y+.5));
        for (i=1; i<=s.maxit; i++){
          One=(z.p*z.p)+(z.q*z.q);
          z.q=(2-One)*z.p;
          z.p=(2-One)*z.q;
          if (dist(z) > s.bound){ dot(pos,i); i=s.maxit+1;};  
        };
      };
    }; 
  };
  if (s.method == plot){
    stop=0;
    while (!stop){     
      z=pixelComp( readPoint() );
      stop2=0;
      i=0;
      while (!stop2){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.y=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        i++;
        One=(z.p*z.p)+(z.q*z.q);
        z.q=(2-One)*z.p;
        z.p=(2-One)*z.q;
        while ( checkEvent() ){
          u2=nextEvent();
          if (u2.type == Expose) redraw();
          if ((u2.type == ButtonPress)||( u2.value == 3 )) stop2=1;
        };
      };
      while ( checkEvent() ){
        u2=nextEvent();
        if (u2.type == Expose) redraw();
        if ((u2.type == ButtonPress)||( u2.value == 3 )) stop=1;
      };
    };
  };
  if (s.method == cloud){
    while (!checkStop()){
      z.p=s.pmin+(drand48()*s.pspan);
      z.q=s.qmin+(drand48()*s.qspan);
      for ( i=0; i<s.maxit; i++ ){
        pos.x=int((z.p-s.pmin)*xpix/s.pspan);
        pos.x=ypix-int((z.q-s.qmin)*ypix/s.qspan);
        dot(pos, i);
        One=(z.p*z.p)+(z.q*z.q);
        z.q=(2-One)*z.p;
        z.p=(2-One)*z.q;
        if (dist(z) > s.bound) i = s.maxit;
      };
    };
  };
}

void dexper1(){
  comp zt; 
  for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
    if (checkStop()) return;
    for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
      z.p=s.pmin+(s.pinc*(pos.x+.5));
      z.q=s.qmax-(s.qinc*(pos.y+.5));
      for (i=1; i<=s.maxit; i++){
        zt=z;
        if (zt.p > 0){
          z.p=(zt.p*zt.p)-(zt.q*zt.q)-1;
          z.q=2*zt.p*zt.q;
        }
        else{
          z.p=(zt.p*zt.p)-(zt.q*zt.q)-1+(realA*zt.p);
          z.q=2*zt.p*zt.q;
        };
        if (dist(z) > s.bound){ dot(pos,i); i=s.maxit+1;};  
      };
    };
  };
}

int exper1P(comp zpp, int iters){
  C=zpp;
  zp=s.init;
  steps=0;
  for (i=1; i<=iters; i++){
    zp=cSub( cExp(zp, s.Power), C);
    if (dist(zp) > s.bound){ steps=i; i=iters+1;};
  };
  return steps;
}

void dexper2(){
  for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
    if (checkStop()) return;
    C.p=s.pmin+(s.pinc*(pos.x+.5));
    for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
      C.q=s.qmax-(s.qinc*(pos.y+.5));
      z = s.init;
      for (i=1; i<=s.maxit; i++){
        if (z.p >= 0) z=cDiv( cSub(z, cOne), C);
        else z=cDiv( cAdd(z, cOne), cConj(C));        
        if(dist(z) > s.bound){
          dot(pos, i);
          i=s.maxit+1;
        };  
      };
    };
  };
}

int exper2P(comp zpp, int iters){
  C=zpp;
  zp=s.init;
  steps=0;
  for (i=1; i<=iters; i++){
    zp=cAdd( cExp( cAdd(zp, C), 2), C);
    if (dist(zp) > s.bound){ steps=i; i=iters+1;};
  };
  return steps;
}

void dexper3(){
  for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
    C.p=s.pmin+(s.pinc*(pos.x+.5));
    if (checkStop()) return;
    for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
      C.q=s.qmax-(s.qinc*(pos.y+.5));
      z = s.init;
      for (i=1; i<=s.maxit; i++){
        if (z.p >= 0) z=cDiv( cSub(z, cOne), C);
        else z=cDiv( cAdd(z, cOne), C);
        if(dist(z) > s.bound){
          dot(pos, i);
          i=s.maxit+1;
        };  
      };
    };
  };
}

int exper3P(comp zpp, int iters){
  C=zpp;
  zp=s.init;
  steps=0;
  for (i=1; i<=iters; i++){
    zp=cAdd( cExp(zp, 2), cExp(C, s.Power) );
    if (dist(zp) > s.bound){ steps=i; i=iters+1;};
  };
  return steps;
}

void dexper4(){
  for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
    C.p=s.pmin+(s.pinc*(pos.x+.5));
    if (checkStop()) return;
    for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
      C.q=s.qmax-(s.qinc*(pos.y+.5));
      z = s.init;
      for (i=1; i<=s.maxit; i=i++){
        z=cSub( cMult(z, s.c), cOne );
        if(dist(z) > s.bound){
          dot(pos, i);
          i=s.maxit+1;
        };  
        //z=cAdd( cMult(z, s.c), cOne );
        //if(dist(z) > s.bound){
        //  dot(pos, i);
        //  i=s.maxit+1;
        //};  
      };
    };
  };
}

void draw( set drawset ){
  s=drawset;
  clear();
  comment( "Set graphing in progress. " );
  resetMatrix();
  updateEvents();
  writeStopM();
  //if (s.mode >= 10){ openGraph(); redraw(); return; };
  if (s.mode == mandel) dmandel();
  if (s.mode == julia) djulia();
  if (s.mode == exper1) dexper1();
  if (s.mode == exper2) dexper2();
  if (s.mode == exper3) dexper3();
  if (s.mode == exper4) dexper4();
  if (s.mode == unity) dunity();
  if (s.mode == halley) dhalley();
  if (s.mode == newton) dnewton();
  redraw();
  comment( "Set graphing finished. " );
}

int setP( int setmode, comp place, int iters ){
  if (setmode == mandel) steps=mandelP(place, iters); 
  if (setmode == julia) steps=juliaP(place, iters); 
  if (setmode == exper1) steps=exper1P(place, iters); 
  if (setmode == exper2) steps=exper2P(place, iters); 
  if (setmode == exper3) steps=exper3P(place, iters); 
  return steps;
}

void points(){
  comp t;
  char pyn;
  int itersp;
  s = c;
  resetPage();
  comment( "Left-click on a point for information, right-click to quit." );
  Event e=getNextEvent();
  while ( (e.type != ButtonPress) || (e.value != 3) ){
    if ( e.type == MotionNotify ){
      t.p=c.pmin+(c.pinc*(e.x+.5));
      t.q=c.qmax-(c.qinc*(e.y+.5)); 
      resetPage();
      printString( "Point " ); printNumber( t.p ); printString( " + " );
      printNumber( t.q ); printString( "i ." ); newLine(); newLine();
    }; 
    if ( ( e.type == ButtonPress ) && ( e.value == 1 ) ){
      t.p=c.pmin+(c.pinc*(e.x+.5));
      t.q=c.qmax-(c.qinc*(e.y+.5)); 
      pyn='y';
      itersp=5000;
      while (pyn=='y'){
        steps=0;
        pyn='n'; 
        resetPage();
        comment( "See page. " );
        if (t.q >= 0){
          printString( "Point " ); printNumber( t.p ); printString( " + " );
          printNumber( t.q ); printString( "i ." ); newLine(); newLine();
        }
        else{ 
          printString( "Point " ); printNumber( t.p ); printString( " - " );
          printNumber( -t.q ); printString( "i ." ); newLine(); newLine();
       };
       steps=setP( c.mode, t, itersp );
        if (steps > 0){
          printString( "The point became unbounded on iteration #" );
          printNumber( steps ); newLine();
        };
        if (steps==0){
          printString( "The point is bounded after " );
          printNumber( itersp ); printString( " iterations. " ); newLine();
          printLine( "Increase iterations ?" );
          itersp=itersp*2;
          clearMenu();
          writeYesNo();
          getChoiceM( general );
          if (chosen( yesB )) pyn = 'y';
          else pyn = 'n';
        };
      }; 
    };
    e = nextEvent(); 
    if (e.type == Expose) redraw();
  }; 
  resetPage();
}

void IFSF(){
  draw(c);
  while(1==1){
    setMenu(panelmenu);
    writeIFSMenu();
    comment( "Choose one of the functions below." );
    getChoiceM( panelmenu );
    if (chosen(inforB)) information();
    //if (box == newse){addRangeList(); getMode(); clear(); begin();}; 
    if (chosen(zoomiB)){
      addRangeList(); zoom(); IFSconstants(); draw( c );};
    if (chosen(zoomoB)){
      addRangeList(); zoomOut(); IFSconstants(); draw( c );};
    if (chosen(opencB)) openClosed();
    if (chosen(constB)){ IFSconstants(); draw( c );};
    if (chosen(colorB)){ 
      getColors();
      c.cconst=inputNumber("Color constant = "); newLine();
      recolor();
     };
    if (chosen(windoB)){addRangeList();c = rangeF( c ); draw( c );};
    if (chosen(pointB)) points();
    if (chosen(refreB)) resetAll();
    if (backM()) return;
    if (quitM()) exit(1);
  };
}

void IFSsetInputs(){
  int ask, whichline;
  while(1==1){
    comment( "Enter set constants. " );
    setMenu(general);
    clearMenu();
    setColor( textC );
    putMenu( mandelB, "Mandelbrot" );
    putMenu( juliaB, "Julia" );
    putMenu( newtonB, "Newton" );
    putMenu( halleyB, "Halley" );
    putMenu( unityB, "Unity" );
    putMenu( othersB, "Others" );
    //putMenu( exper1B, "Experiment #1" );
    //putMenu( exper2B, "Experiment #2" );
    //putMenu( exper3B, "Experiment #3" );
    getChoiceM( general );
    if (chosen(mandelB)) c.mode = mandel;
    if (chosen(juliaB)) c.mode = julia;
    if (chosen(newtonB)) c.mode = newton;
    if (chosen(halleyB)) c.mode = halley;
    if (chosen(unityB)) c.mode = unity;
    if (chosen(othersB)){
       comment( "Choose one of the adjacent formulas. " );
       whichline=chooseLine( "fractal.txt" );
       switch (whichline){
         case 0: c.mode = exper1; break;
         case 1: c.mode = exper2; break;
         case 2: c.mode = exper3; break;
         case 3: c.mode = exper4; break;
         default: comment( "Error: bad mode." );
       };
    };
    //if ( menu[exper1B.x][exper1B.y] ) c.mode = exper1;
    //if ( menu[exper2B.x][exper2B.y] ) c.mode = exper2;
    //if ( menu[exper3B.x][exper3B.y] ) c.mode = exper3;
    if (backM()) return;
    if (quitM()) exit(1);
    clearMenu();
    setColor( textC );
    putMenu( boundB, "Bounded points" );
    putMenu( convB, "Convergent" );
    putMenu( openboundB, "Open bounded" );
    putMenu( openconvB, "Open convergent" );
    putMenu( plotB, "Plot points" );
    putMenu( cloudB, "'cloud' graph" );
    putMenu( normB, "'norm' graph" );
    getChoiceM( general );
    if (chosen(boundB)) c.method=bound;
    if (chosen(convB)) c.method=conv;
    if (chosen(openboundB)) c.method=openbound;
    if (chosen(openconvB)) c.method=openconv;  
    if (chosen(plotB)) c.method=plot;
    if (chosen(cloudB)) c.method=cloud;
    if (chosen(normB)) c.method=norm;
    c.Power=0;
    ask=1;
    if (c.mode == unity) ask=0;
    if (ask) c.Power=inputInt( "To which Power ? " );
    if (c.Power < 0){
      printLine( "The Power must be a natural number.  Assuming Power 1. " );
      c.Power=1;
    };
    if (c.mode == mandel){
      printLine( "Mandelbrot set, initial value p + iq." );
      c.init.p=inputNumber( "p = " );
      c.init.q=inputNumber( "q = " );
    };
    if (c.mode == julia){
      printLine( "Julia set complex constant c. " );
      c.c.p=inputNumber( "Enter the real component. " );
      c.c.q=inputNumber( "Enter the imaginary component. " );
    };
    if (c.mode == newton){
      printLine( "Newton set complex constant c. " );
      c.c.p=inputNumber( "Enter the real component. " );
      c.c.q=inputNumber( "Enter the imaginary component. " );

    };
    if (c.mode == halley){
      printLine( "Halley set complex constant c. " );
      c.c.p=inputNumber( "Enter the real component. " );
      c.c.q=inputNumber( "Enter the imaginary component. " );
    };
    if (c.mode == exper1) realA=inputNumber("Enter the real constant lambda.");
    if (c.mode == exper2){
      printLine( "Initial value p + iq." );
      c.init.p=inputNumber( "p = " );
      c.init.q=inputNumber( "q = " );
    };
    if (c.mode > exper2){
      printLine( "Enter the initial value p+iq. " );
      c.init.p=inputNumber( "p = " );
      c.init.q=inputNumber( "q = " );
      printLine( "Enter the complex constant c (if applicable). " );
      c.c.p=inputNumber( "p = " );
      c.c.q = inputNumber( "q = " );
    };
    c = rangeF( c );
    IFSconstants();
    IFSF();
  };
}

int listed(){
  int onlist;
  for (i=0; i <= listplace; i++){
    onlist=1;
    if (rangeList[i].s.pmin != c.pmin) onlist=0;
    if (rangeList[i].s.pmax != c.pmax) onlist=0;
    if (rangeList[i].s.pmin != c.qmin) onlist=0;
    if (rangeList[i].s.pmax != c.qmax) onlist=0;
    if (onlist) return 1;
  };
  return 0;
}

void openClosed(){
  int iters, closed, tries, outbounds;
  comp spot;
  set n;
  comment( "See the page. " );
  char ans;
  ans=inputChar("this window, change window, new set ? " );
  if ((ans != 't')&&(ans != 'c')&&(ans != 'n')) ans = 'n';
  if ((ans == 't')||(ans == 'c')){
    if ((c.mode < 1)||(c.mode > 5)){
      printLine("There is no program to examine this set. " );
      return;
    };
  };
  if ( ans == 't' ) n=c;
  if (ans == 'c'){
    n = c;
    n = rangeF( n );
  };
  if (ans == 'n'){
    n.mode=inputInt("Mandelbrot (1), Julia (2), (3), (4), (5) ? " );
    if ((n.mode > 5) || (n.mode < 1)){
      printLine("There is no program to examine the specified set. ");
      return;
    };
    newLine();
    n.Power=inputInt( "To which Power (i.e. M2, M3, J2,..)? " );
     if (n.mode == 1){
      printLine( "Mandelbrot set.  Enter the initial value p+iq. ");
      n.init.p=inputNumber( "p = " );
      n.init.q=inputNumber( "q = " );
     };
    if (n.mode == 2){
      printLine("Julia set complex constant c. " );
      n.c.p=inputNumber("Enter the real component. " );
      n.c.q=inputNumber("Enter the imaginary component. " );
    }; 
    if (n.mode > 2){
      printString("Experimental set #" ); printNumber(n.mode-2); newLine();
      int answer;
      answer=inputInt("Hold the initial value (1) or c (2) constant ? ");
      if ((answer > 2)||(answer < 1)){
        printLine("Wrong answer. " );
        return;
      };
      if (answer == 1){
        printLine("Enter the initial value p + iq. " );
        n.init.p=inputNumber( "p = " );
        n.init.q=inputNumber( "q = " );
      };
      if (answer == 2){
        printLine( "Enter the complex constant c = p + iq. " );
        n.c.p=inputNumber( "p = " );
        n.c.q=inputNumber( "q = " );
      };
    };
    n.bound=inputNumber( "Critical magnitude = ");
    n = rangeF( n );
  };
  s = n;
  closed=1;
  tries=0;
  switch (n.mode){
    case 1:
      while ((closed)&&(tries < maxtries)){
        tries++;
        spot.p=(drand48()*n.pspan)+n.pmin;
        spot.q=(drand48()*n.qspan)+n.qmin;
        iters=100;
        outbounds=0;
        while ((outbounds==0)&&(iters<maxiters)){
          if ( mandelP(spot, iters) ) outbounds=1;
          iters=iters*2;
        };
        if (!outbounds) closed=0;
      };
      break;
    case 2:
      while ((closed)&&(tries < maxtries)){
        tries++;
        spot.p=(drand48()*n.pspan)+n.pmin;
        spot.q=(drand48()*n.qspan)+n.qmin;
        iters=100;
        outbounds=0;
        while ((outbounds==0)&&(iters<maxiters)){
          if ( juliaP(spot, iters) ) outbounds=1;
          iters=iters*2;
        };
        if (!outbounds) closed=0;
      };
      break;
    case 3:
      while ((closed)&&(tries < maxtries)){
        tries++;
        spot.p=(drand48()*n.pspan)+n.pmin;
        spot.q=(drand48()*n.qspan)+n.qmin;
        iters=100;
        outbounds=0;
        while ((outbounds==0)&&(iters<maxiters)){
          if ( exper1P(spot, iters) ) outbounds=1;
          iters=iters*2;
        };
        if (!outbounds) closed=0;
      };
      break;
    case 4:
      while ((closed)&&(tries < maxtries)){
        tries++;
        spot.p=(drand48()*n.pspan)+n.pmin;
        spot.q=(drand48()*n.qspan)+n.qmin;
        iters=100;
        outbounds=0;
        while ((outbounds==0)&&(iters<maxiters)){
          if ( exper2P(spot, iters) ) outbounds=1;
          iters=iters*2;
        };
        if (!outbounds) closed=0;
      };
      break;
    case 5:
      while ((closed)&&(tries < maxtries)){
        tries++;
        spot.p=(drand48()*n.pspan)+n.pmin;
        spot.q=(drand48()*n.qspan)+n.qmin;
        iters=100;
        outbounds=0;
        while ((outbounds==0)&&(iters<maxiters)){
          if ( exper3P(spot, iters) ) outbounds=1;
          iters=iters*2;
        };
        if (!outbounds) closed=0;
      };
      break;
  };
  if (!closed){ 
    printString("The set is open.  (At least one point is bounded after ");
    printNumber( maxiters ); printLine(" iterations.) "); newLine();
  }
  else{ printString("The set appears to be closed.  (After " );
    printNumber( maxtries ); printString( " tries");
    printLine( ", still no bounded points were found.) "); newLine(); 
  };
}

int openClosedSet(){
  int closed=1, iters, outbounds, tries=0;
  comp spot;
  if (s.mode == 10){
    while ((closed)&&(tries < maxtries2)){
      tries++;
      spot.p=(drand48()*s.pspan)+s.pmin;
      spot.q=(drand48()*s.qspan)+s.qmin;
      iters=100;
      outbounds=0;
      while ((outbounds==0)&&(iters < s.maxit)){
        if ( mandelP(spot, iters) ) outbounds=1;
        iters=iters*2;
      };
      if (!outbounds) closed=0;
    };
  };
  if (s.mode == 20){
    while ((closed)&&(tries < maxtries2)){
      tries++;
      spot.p=(drand48()*s.pspan)+s.pmin;
      spot.q=(drand48()*s.qspan)+s.qmin;
      iters=100;
      outbounds=0;
      while ((outbounds==0)&&(iters < s.maxit)){
        if ( juliaP(spot,iters) ) outbounds=1;
        iters=iters*2;
      };
      if (!outbounds) closed=0;
    };
  };
  if (closed) return 0;
  else return tries;
}

void openGraph(){
  int opened;
  clear();
  for (pos.x=0; pos.x <= xpix; pos.x=pos.x+s.redfac){
    s.c.p=s.pmin+(s.pinc*(pos.x+.5));
    for (pos.y=0; pos.y <= ypix; pos.y=pos.y+s.redfac){
      s.c.q=s.qmax-(s.qinc*(pos.y+.5));
      opened=openClosedSet();
      if(opened > 0) dot(pos, opened); 
    };
  };
}

void openInputs(){
  int opened;
  setColor( textC );
  system("clear");
  comment( "Enter the set constants in the command tool. " );
  cout << "Mandelbrot (1) or Julia set (2) ? "; cin >> c.mode; cout << endl;
  c.mode=c.mode*10;
  cout << "To which Power? "; cin >> c.Power; cout << endl;
  if ((c.mode < 10)||(c.mode > 50)){
    cout << "There is no program for the specified set. " << endl;
    return;
  };
  if (c.mode > 20){
    cout << "Experimental set #" << (c.mode/10)-2 << endl;
    int answer;
    cout << "Will you hold the initial value (1) or c (2) constant ? ";
    cin >> answer; cout << endl;
    if ((answer > 2)||(answer < 1)){
      cout << "Wrong answer. " << endl;
      return;
    };
    if (answer == 1){
      cout << "Enter the initial value p + iq. " << endl;
      cout << "p = "; cin >> c.init.p; cout << endl;
      cout << "q = "; cin >> c.init.q; cout << endl;
    };
    if (answer == 2){
      cout << "Enter the complex constant c = p + iq. " << endl;
      cout << "p = "; cin >> c.c.p; cout << endl;
      cout << "q = "; cin >> c.c.q; cout << endl;
    };
  };
  c = rangeF( c );
}

void information(){
  //system("clear");
  resetPage();
  comment( "Information displayed on the page." );
  printString("This is ");
  switch (c.mode){
    case 1: printString("the Mandelbrot set M"); printNumber( c.Power );
      printLine(". ");
      printString("Starting value ( "); printNumber(c.init.p);
      printString(", "); printNumber( c.init.q ); printLine(" ).");
      break;
    case 2: cout << "the Julia set J"<<c.Power<<"("<<c.c.p;
      cout << ", "<<c.c.q<<"). " << endl;
      break;
    case 10:
      cout << "the openness graph of the Mandelbrot set. "<< endl;
      break;
    case 20:
      cout << "the openness graph of the Julia set. " << endl;
      break;
    default: 
      if (c.mode < 10){
        cout << "the experimental set #"<< c.mode-2 << endl;
        cout << "Initial value = " << c.init.p<< " ";
        if (c.init.q < 0) cout << "- " << -c.init.q << "i. " << endl;
        else cout << "+ " << c.init.q << "i. " << endl;
        cout << "Complex constant c =" << c.c.p << " ";
        if (c.c.q < 0) cout << "- " << -c.c.q << "i. " << endl;
        else cout << "+ " << c.c.q << "i. " << endl;
      };
      if (c.mode > 10){
        cout << "the openness graph of the experimental set #";
        cout << (c.mode/10)-2 << endl;
      };
  };
  newLine();
  printLine("Current viewing window: ");
  cout << "p: [ " << c.pmin << ", " << c.pmax << " ]" << endl;
  cout << "q: [ " << c.qmin << ", " << c.qmax << " ]" << endl << endl;
  cout << "Maximum number of iterations = " << c.maxit << endl;
  if (c.mode >= 10) cout << "Maximum point trials = " << maxtries2 << endl;
  cout << "Boundary radius = " << c.bound << endl;
  cout << "Color constant = " << c.cconst << endl;
  if (c.redfac == 1) cout << "Full detail. " << endl;
    else cout << "Detail reduced by a factor of " << c.redfac << endl << endl;
}

void getColors(){
  clearMenu();
  comment( "Select a color scheme for set graphing." );
  drawColors();
  getChoiceM( colormenu );
  int choice=-1;
  for (i=0; i < colorlists+blends; i++){
    if ( colmenu[i] ) choice=i;
  };
  if (choice == -1){
    comment( "Error: bad color scheme." );
    wait();
    choice = 0;
  };
  if (choice < blends){
    coloring=blend;
    lowcol=blendList[choice][0];
    highcol=blendList[choice][1];
    colspan=highcol-lowcol+1;
  };
  if (choice >= blends){ 
    coloring=list;
    colorlist=choice-blends;
    lowcol=0;
    highcol=1000;
  };
  clearMenu();
}

#include "orbit.h"
//#include "IFS.h"
//#include "growth.h"
//#include "coast.h"
//#include "distance.h"
//#include "land.h"
//#include "exper.h" 

void fractalMain(){

  pagecor.x=xpix;
  pagecor.y=0;
  //assign places for the menu areas (all the coordinates not written are 0)
  constB.x=1; inforB.x=2; newseB.x=3;
  opencB.y=1; pointB.y=1; refreB.y=1; windoB.y=1;
  pointB.x=1; refreB.x=2; windoB.x=3;
  zoomiB.y=2; zoomoB.y=2;
  zoomoB.x=1;
  orbitB.x=1; growthB.x=2; IFSrB.x=3;
  coastB.y=1; distanceB.y=1; landB.y=1; experB.y=1;
  distanceB.x=1; landB.x=2; experB.x=3;
  juliaB.x=1; newtonB.x=2; halleyB.x=3;
  unityB.y=1; exper1B.y=1; exper2B.y=1; exper3B.y=1;
  exper1B.x=1; exper2B.x=2; exper3B.x=3;
  othersB.y=1; othersB.x=1;
  convB.x=1; openboundB.x=2; openconvB.x=3;
  plotB.y=1; cloudB.y=1; normB.y=1;
  cloudB.x=1; normB.x=2;
  rotatB=opencB;

  //create color schemes
  newColorList(0, 150, backC-1, 150, backC-1, 150, backC-1);
  newColorList(1, 1, 27, 38, 73, 121, 153);
  newColorList(2, 1, 151, 40, 32, 193, 116);
  newColorList(3, 1, 8, 15, 23, 30, 38);
  newBlend( 0, 1, 160);
  newBlend( 1, 160, backC-2);
  newBlend( 2, 110, 140);
  newBlend( 3, 1, 40);

  for (i=0; i < ranges; i++) rangeList[i].pix=createPixmap(win,xpix,ypix);
  colorMenu();
  while( 1==1 ){
    setMenu(general);
    clearPage();
    locate (0, 0);
    comment( "Fractals: main menu.  Choose a fractal type." );
    clearMenu();
    setColor( textC );
    putMenu( IFSB, "Mandel-type" );
    putMenu( orbitB, "orbit" ); 
    putMenu( growthB, "particle growth" );
    putMenu( IFSrB, "IFS" ); 
    putMenu( coastB, "coast curves" );
    putMenu( distanceB, "set-distances" );
    putMenu( landB, "landscapes" );
    putMenu( experB, "experimental" );
    getChoiceM( general );
    if (chosen(IFSB)){ getColors(); IFSsetInputs(); };
    if (chosen(orbitB)) orbitMain();
    //if ( chosen( IFSrB ) ) IFSMain();
    //if ( chosen( growthB ) ) growthMain();
    //if ( chosen( coastB ) ) coastMain();
    //if ( chosen( distanceB ) ) distanceMain();
    //if ( chosen( landB ) ) landMain();
    //if ( chosen( experB ) ) experMain();
    if (backM()) return;
    if (quitM()) exit(1);
  };
}
