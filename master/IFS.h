int gasketC=40;

int ETAmode;
int spinski=1, babylon=2;

double gasketrad=.9;  //gasket radius for regular gaskets

int gasketcors;
int maxGaskCors=20;
point2 gasketCorners[20];  //[maxGaskCors] !
point3 gasketCorners3[20];  //[maxGaskCors] !

pixel gasketB, ETAB;
pixel regularB, irregB;
pixel gas2B, gas3B;
pixel spinskiB, babylonB;
pixel drawETAB, colorETAB, constETAB, windoETAB, zoomiETAB, zoomoETAB;
  pixel pointETAB, refreETAB, inforETAB;
 
void inputGasketCors(){
  for (i=0; i<maxGaskCors; i++ ){
    gasketCorners[i].x=0;
    gasketCorners[i].y=0;
  };
  printLine( "Set the new gasket corners. " );
  for (i=0; i<gasketcors; i++){
    gasketCorners[i]=pixelPoint( readPoint() );
  };
  double xhigh=gasketCorners[0].x;
  double xlow=gasketCorners[0].x;
  double yhigh=gasketCorners[0].y;
  double ylow=gasketCorners[0].y;
  for (i=1; i< gasketcors; i++ ){
    if (gasketCorners[i].x < xlow) xlow=gasketCorners[i].x;
    if (gasketCorners[i].x > xhigh) xhigh=gasketCorners[i].x;
    if (gasketCorners[i].y < ylow) ylow=gasketCorners[i].y;
    if (gasketCorners[i].y > yhigh) yhigh=gasketCorners[i].y;
  };
  xmin=((xlow+xhigh)/2) - ((xhigh-xlow)/2);
  xmax=((xlow+xhigh)/2) + ((xhigh-xlow)/2);
  ymin=((ylow+yhigh)/2) - ((yhigh-ylow)/2);
  ymax=((ylow+yhigh)/2) + ((yhigh-ylow)/2);
  rrangeconst2D();
}

void initGasketCors(){
  for (i=1; i < maxGaskCors; i++ ){
    gasketCorners[i].x=0;
    gasketCorners[i].y=0;
  };
  double divang=(pi*2)/gasketcors;
  for (i=0; i<gasketcors; i++){
    gasketCorners[i].x=gasketrad * cos(i*divang);
    gasketCorners[i].y=gasketrad * sin(i*divang);
  };
}

void initGasketCors3(){
  for (i=1; i < maxGaskCors; i++ ){
    gasketCorners3[i].x=0;
    gasketCorners3[i].y=0;
    gasketCorners3[i].z=0;
  };
  for (i=0; i<gasketcors; i++){
    gasketCorners3[i].x=i*3;
    gasketCorners3[i].y=i*2;
    gasketCorners3[i].z=i;
  };
}

void writeIFSOptions(){
  setColor( textC );
  putMenu( gasketB, "Gaskets" );
  putMenu( ETAB, "ETA" );
}

void writeGasketType(){
  setColor( textC );
  putMenu( regularB, "Regular" );
  putMenu( irregB, "Irregular" );
}

void drawGasket(){
  updateEvents();
  writeStopM();
  point2 gp;
  point2 nextcor;
  gp=gasketCorners[0];
  clear();
  setColor( gasketC );
  while( !checkStop() ){
    for (i=0; i<100; i++){
      drawPixel( pointPixel( gp ) );
      nextcor=gasketCorners[ randInt( gasketcors )-1 ];
      gp.x=gp.x+((nextcor.x-gp.x)/(gasketcors-1));
      gp.y=gp.y+((nextcor.y-gp.y)/(gasketcors-1));
    };
  };
}

void drawGasket3(){
  updateEvents();
  writeStopM();
  point3 gp;
  point3 nextcor;
  gp=gasketCorners3[0];
  while( !checkStop() ){
    for (i=0; i<100; i++){
      findC(gp.z);
      inFront(gp);
      nextcor=gasketCorners3[ randInt( gasketcors )-1 ];
      gp.x=gp.x+((nextcor.x-gp.x)/(gasketcors-1));
      gp.y=gp.y+((nextcor.y-gp.y)/(gasketcors-1));
    };
  };
}

void initCgas3(){
  double highz, lowz, maxd, mind, thisdist;
  mind=middist(gasketCorners3[0]);
  maxd=mind;
  highz=gasketCorners3[i].z;
  lowz=highz;
  for (i=0; i<gasketcors; i++){
     if (gasketCorners3[i].z < lowz) lowz = gasketCorners3[i].z; 
     if (gasketCorners3[i].z > highz) highz = gasketCorners3[i].z;
     thisdist=middist(gasketCorners3[i]);
     if (thisdist < mind) mind=thisdist;
     if (thisdist > maxd) maxd=thisdist;
  };
  cconst3D=colspan3D/(highz-lowz);
  distCconst1=highC+((colspan3D*mind)/(maxd-mind));
  distCconst2=-colspan3D/(maxd-mind);
}

void gaskets(){
  int box, dimen;
  while(1==1){
    setMenu(general);
    resetAll();
    setColor(textC);
    putMenu(gas2B, "2-dimensional");
    putMenu(gas3B, "3-dimensional");
    getChoiceM(general);
    dimen=2;
    if (chosen(gas2B)) dimen=2;
    if (chosen(gas3B)) dimen=3;
    if (backM()) return;
    if (quitM()) exit(1);
    if (dimen == 2){
      resetAll();
      writeGasketType();
      getChoiceM( general );
      newLine();
      if (chosen( regularB )){
        gasketcors=inputNumber( "How many sides will the gasket have? " );
        comment( "Drawing regular gasket." );
        unitRange(2);
        initGasketCors();
      };
      if (chosen( irregB )){
        gasketcors=inputNumber( "How many sides will the gasket have? " );
        inputGasketCors();
        comment( "Drawing irregular gasket. " );
      };
      if (chosen( backB )) return;
      if (chosen( quitB )) exit(1);
      drawGasket();
    };
    if (dimen == 3){
      gasketcors=inputNumber( "How many vertices will the gasket have? ");
      initCgas3();
      unitRange(3);
      parAxes(1);
      clearField();
      clear();
      graphAxes();
      drawGasket3();
    };
  };
}

void dsierpinski(){
  double bound2=radius*radius;
  point2 p;
  for( pos.x=0; pos.x <= xpix; pos.x=pos.x+redfac){
    if ( checkStop() ) return;
    for( pos.y=0; pos.y <= ypix; pos.y=pos.y+redfac){
      p=pixelPoint( pos );
      for (i=0; i <= maxit; i++){
        if (p.y > .5){
          p.x=2*p.x;
          p.y=2*p.y-1;
        }
        else{
          if (p.x > .5){
            p.x=2*p.x-1;
            p.y=2*p.y;
          }
          else{
            p.x=2*p.x;
            p.y=2*p.y;
          };
        };
        if ((p.x*p.x)+(p.y*p.y) > bound2){
          dotR(pos, i);
          i=maxit;
        };
      };
    };
  };
}

void dbabylon(){
  double thisroot;
  point2 p;
  for( pos.x=0; pos.x <= xpix; pos.x=pos.x+redfac){
    if ( checkStop() ) return;
    for( pos.y=0; pos.y <= ypix; pos.y=pos.y+redfac){
      p=pixelPoint( pos );
      if (p.x >= 0) thisroot=sqrt(p.x);
      else thisroot=sqrt(-p.x);
      i=0;
      while (fabs(p.y-thisroot) > radius){
        i++;
        p.y=(p.y/2) + (p.x/p.y);
      };
      dotR(pos, i);
    };
  };
}

void drawETA(){
  clear();
  comment( "IFS attractor: graphing in progress. " );
  resetMatrix();
  updateEvents();
  writeStopM();
  if (ETAmode == spinski) dsierpinski();
  if (ETAmode == babylon) dbabylon();
  redraw();
  comment( "IFS graphing finished. " );
}

void ETAInfo(){
  resetWindow();
  setColor( textC );
  printLine( "No news" );
  newLine();
}

void ETAConsts(){
  setColor( textC );
  maxit=inputNumber( "Maximum iterations = " );
  radius=inputNumber( "Boundary radius = " );
  cconst=inputNumber( "color constant = " );
  redfac=inputNumber( "detail reduced by..." );
  newLine();
}

void writeETAMenu(){
  setColor( textC );
  putMenu( drawETAB, "draw graph" );
  putMenu( colorETAB, "colors" );
  putMenu( constETAB, "constants" );
  putMenu( windoETAB, "window" );
  putMenu( zoomiETAB, "zoom in" );
  putMenu( zoomoETAB, "zoom out" );
  putMenu( inforETAB, "information" );
  putMenu( pointETAB, "point analysis" );
  putMenu( refreETAB, "refresh screen" );
}

void ETAF(){
  int box;
  while (1==1){
    setMenu(panelmenu);
    clearMenu();
    writeETAMenu();
    getChoiceM( panelmenu );
    if (chosen(drawETAB)) drawETA();
    if (chosen(constETAB)) ETAConsts();
    if (chosen(colorETAB)){ getColors(); recolorR(); };
    if (chosen(inforETAB)== box) ETAInfo();
    if (chosen(windoETAB)) rrange2D();
    if (chosen(refreETAB)) resetAll();
    //if (chosen(pointETAB))
    if (chosen(zoomiETAB)){ zoom2D(); drawETA(); };
    if (chosen(zoomoETAB)){ zoomOut2D(); drawETA(); };
    if (backM()) return;
    if (quitM()) exit(1);
  };
}

void ETA(){  //Escape Time Algorithm
  int box;
  while (1==1){
    resetAll();
    setColor( textC );
    putMenu( spinskiB, "Sierpinski" );
    putMenu( babylonB, "Babylon" );
    getChoiceM(general);
    if (chosen(spinskiB)) ETAmode=spinski;
    if (chosen(babylonB)) ETAmode=babylon;
    if (backM()) return;
    if (quitM()) exit(1);
    rrange2D();
    ETAConsts();
    drawETA();
    ETAF();
  };
}

void IFSMain(){

  //menu positions (all coordinates not written are 0. )
  ETAB.x=1;
  irregB.x=1;
  constETAB.x=1; drawETAB.x=2; pointETAB.x=3;
  refreETAB.y=1; windoETAB.y=1; zoomiETAB.y=1; zoomoETAB.y=1;
  windoETAB.x=1; zoomiETAB.x=2; zoomoETAB.x=3;
  inforETAB.y=2;
  babylonB.x=1;
  gas3B.x=1;

  while(1==1){
    setMenu(general);
    resetAll();
    writeIFSOptions();
    getChoiceM( general );
    if (chosen( gasketB )){ gaskets(); box=-5;};
    if (chosen( ETAB )) ETA();
    if (chosen( backB )) return;
    if (chosen( quitB )) exit(1);
  };
}
