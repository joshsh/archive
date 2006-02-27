int orbitmode;
int orbitC=40, lowCO=1, highCO=150, colspanO=highCO-lowCO;
int colorprobes=400;

double cconstO1, cconstO2, cconstO3;

pixel gingerB, henonB, martinB, dynamicB, rosslerB, lorenzB, pickoverB;
  pixel kamtorusB;
pixel refreO2B, zoomiO2B, zoomoO2B, constO2B, colorO2B, pointO2B, inforO2B;
  pixel graphO2B, windoO2B;
pixel refreO3B, constO3B, colorO3B, pointO3B, inforO3B, rotatO3B, rangeO3B;
  pixel graphO3B;
pixel trailB, depthB;

//orbit modes
int ginger=1, henon=2, martin=3, dynamic=4, rossler=5, lorenz=6, pickover=7;
  int kamtorus=8;
int methodO;
int trail=1, depth=2;

//group constants
int O2D=1, O3D=2;

int coloringO;

point2 nopoint2;
point3 nopoint3;
point2 current2;
point3 current3;

int orbitGroup( int omode ){
  if ((omode==lorenz)||(omode==rossler)||(omode==pickover)) return O3D;
  else return O2D;
}

void initCO(){
  point3 ic1=current3, icl;
  double highz, lowz, thisdist, mind, maxd;
  if (orbitmode==lorenz){
    for(i=1; i< colorprobes; i++){
      icl=ic1;
      ic1.x= icl.x + (-realA*icl.x*realD) + (realA*icl.y*realD);
      ic1.y= icl.y + (realB*icl.x*realD) - (icl.y*realD) - (icl.z*icl.x*realD);
      ic1.z= icl.z + (-realC*icl.z*realD) + (icl.x*icl.y*realD);
thisdist=((ic1.x-xmid)*(ic1.x-xmid))+((ic1.y-ymid)*(ic1.y-ymid))+((ic1.z-zmid)*(ic1.z-zmid));
      if (i == 10){ lowz=ic1.z; highz=ic1.z; mind=thisdist; maxd=thisdist; };
      if (i > 10){
        if (ic1.z < lowz ) lowz=ic1.z;
        if (ic1.z > highz ) highz=ic1.z; 
        if (thisdist < mind) mind=thisdist;
        if (thisdist > maxd) maxd=thisdist; 
      };
    }; 
  };
  if (orbitmode==pickover){
    for (i=1; i<colorprobes; i++){
      icl=ic1;
      ic1.x=sin(realA*icl.y)-(icl.z*cos(realB*icl.x));
      ic1.y=(icl.z*sin(realC*icl.x))-cos(realD*icl.y);
      ic1.z=sin(icl.x);
thisdist=((ic1.x-xmid)*(ic1.x-xmid))+((ic1.y-ymid)*(ic1.y-ymid))+((ic1.z-zmid)*(ic1.z-zmid));
      if (i == 50){ lowz=ic1.z; highz=ic1.z; mind=thisdist; maxd=thisdist; };
      if (i > 50){
        if (ic1.z < lowz ) lowz=ic1.z;
        if (ic1.z > highz ) highz=ic1.z;
        if (thisdist < mind) mind=thisdist;
        if (thisdist > maxd) maxd=thisdist;
      };
    };
  };
  if (orbitmode == rossler){
    for (i=1; i < colorprobes; i++){
      icl=ic1;
      ic1.x=icl.x-(icl.y*realD)-(icl.z*realD);
      ic1.y=icl.y+(icl.x*realD)+(realA*icl.y*realD);
      ic1.z=icl.z+(realB*realD)+(icl.x*icl.z*realD)-(realC*icl.z*realD);
thisdist=((ic1.x-xmid)*(ic1.x-xmid))+((ic1.y-ymid)*(ic1.y-ymid))+((ic1.z-zmid)*(ic1.z-zmid));
      if (i == 10){ lowz=ic1.z; highz=ic1.z; mind=thisdist; maxd=thisdist; };
      if (i>10){
        if (ic1.z < lowz ) lowz=ic1.z;
        if (ic1.z > highz ) highz=ic1.z; 
        if (thisdist < mind) mind=thisdist;
        if (thisdist > maxd) maxd=thisdist;  
      };      
    };
  };
  cconstO1=lowCO+((colspanO*highz)/(highz-lowz));
  cconstO2=-(colspanO/(highz-lowz));
  distCconst1=highC+((colspan3D*mind)/(maxd-mind));
  distCconst2=-colspan3D/(maxd-mind);
}

void orbitConstants(){
  resetPage();
  if (orbitmode == dynamic){
    printString( "Maximum # iterations = " ); printNumber(intA); newLine();
    printString( "constant a = " ); printNumber(realA); newLine();
    printString( "constant b = " ); printNumber(realB); newLine();
    printString( "time interval dt = " ); printNumber(realC); newLine();
  };
  if (orbitmode == henon){
    printString( "constant a = " ); printNumber(realA); newLine();
    printString( "constant b = " ); printNumber(realB); newLine();
  };
  if (orbitmode == rossler){
    printString( "constant a = " ); printNumber(realA); newLine();
    printString( "constant b = " ); printNumber(realB); newLine();
    printString( "constant c = " ); printNumber(realC); newLine();
    printString( "time interval dt = " ); printNumber(realD); newLine();
  };
  if (orbitmode == martin){
    printString( "constant a = " ); printNumber(realA); newLine();
    printString( "constant b = " ); printNumber(realB); newLine();
    printString( "constant c = " ); printNumber(realC); newLine();
  };
  if (orbitmode == lorenz){
    printString( "constant a = " ); printNumber(realA); newLine();
    printString( "constant b = " ); printNumber(realB); newLine();
    printString( "constant c = " ); printNumber(realC); newLine();
    printString( "time interval dt = " ); printNumber(realD); newLine();
  };
  if (orbitmode == pickover){
    printString( "constant a = " ); printNumber(realA); newLine();
    printString( "constant b = " ); printNumber(realB); newLine();
    printString( "constant c = " ); printNumber(realC); newLine();
    printString( "constant d = " ); printNumber(realD); newLine();
  };
  if (orbitmode == kamtorus){
    printString( "Maximum # iterations = " ); printNumber(intA); newLine();
    printString( "constant a = " ); printNumber(realA); newLine();
    printString( "step size = " ); printNumber(realB); newLine();
  };
  initCO();
}

void orbitInputs(){
  resetPage();
  if (orbitmode == dynamic){
    intA=int(inputNumber( "Maximum # iterations = " ));
    realA=inputNumber( "constant a = " );
    realB=inputNumber( "constant b = " );
    realC=inputNumber( "time interval dt = " );
  };
  if (orbitmode == henon){
    realA=inputNumber( "constant a = " );
    realB=inputNumber( "constant b = " );
  };
  if (orbitmode == rossler){
    realA=inputNumber( "constant a = " );
    realB=inputNumber( "constant b = " );
    realC=inputNumber( "constant c = " );
    realD=inputNumber( "time interval dt = " );
  };
  if (orbitmode == martin){
    realA=inputNumber( "constant a = " );
    realB=inputNumber( "constant b = " );
    realC=inputNumber( "constant c = " );
  };
  if (orbitmode == lorenz){
    realA=inputNumber( "constant a = " );
    realB=inputNumber( "constant b = " );
    realC=inputNumber( "constant c = " );
    realD=inputNumber( "time interval dt = " );
  };
  if (orbitmode == pickover){
    realA=inputNumber( "constant a = " );
    realB=inputNumber( "constant b = " );
    realC=inputNumber( "constant c = " );
    realD=inputNumber( "constant d = " );
  };
  if (orbitmode == kamtorus){
    intA=int(inputNumber( "Maximum # iterations = " ));
    realA=inputNumber( "constant a = " );
    realB=inputNumber( "step size = " );
  };
  initCO();
}

writeOrbitSets(){
  setColor( textC );
  putMenu( henonB, "Henon" );
  putMenu( gingerB, "Ginger" );
  putMenu( lorenzB, "Lorenz" );
  putMenu( pickoverB, "Pickover" );
  putMenu( dynamicB, "Dynamic" );
  putMenu( martinB, "Martin" );
  putMenu( rosslerB, "Rossler" );
  putMenu( kamtorusB, "Kam torus" );
}

void findCmagO( double fcz ){
  setColor( int(cconstO1+(cconstO2*fcz)) );
}

void findCiterO( int fcit ){
  setColor( int(lowCO+(cconstO3*fcit)) );
}

void dhenon( point2 h1 ){
  int iterO=0; 
  setColor( orbitC );
  while (!checkStop()){
    if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
    drawPixel( pointPixel( h1 ));
    xl=h1.x;
    yl=h1.y;
    h1.x = 1+yl-(realA*xl*xl); 
    h1.y = realB*xl;
  };
  //default: a=1.4  b=.3
}

  
void dlorenz( point3 l1 ){
  int iterO=0;
  setColor( orbitC );
  point3 l1l;
  //comment( " Press the right mouse button to end graphing. " );
  if (methodO == trail){
    while (!checkStop()){
      l1l=l1;
      l1.x= l1l.x + (-realA*l1l.x*realD) + (realA*l1l.y*realD);
      l1.y= l1l.y + (realB*l1l.x*realD) - (l1l.y*realD) - (l1l.z*l1l.x*realD);
      l1.z= l1l.z + (-realC*l1l.z*realD) + (l1l.x*l1l.y*realD);
      if (coloringO == bymag) findCmagO( l1.z );
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      if (coloringO == bydist) findCdist( l1 );
      line3D( l1, l1l );
     };
  };
  if (methodO == depth){
    clearField();
    graphAxes();
    while (!checkStop()){
      l1l=l1;
      l1.x= l1l.x + (-realA*l1l.x*realD) + (realA*l1l.y*realD);
      l1.y= l1l.y + (realB*l1l.x*realD) - (l1l.y*realD) - (l1l.z*l1l.x*realD);
      l1.z= l1l.z + (-realC*l1l.z*realD) + (l1l.x*l1l.y*realD);
      if (coloringO == bymag) findCmagO( l1.z );
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      if (coloringO == bydist) findCdist( l1 );
      inFront(l1);
     };
  };
/*
 dx/dt = -a*x + a*y
 dy/dt = b*x - y - z*x
 dz/dt = -c*z + x*y

  xnew= x + (-a*x*dt) + (a*y*dt)
  ynew= y + (b*x*dt) - (y*dt) - (z*x*dt)
  znew= z + (-c*z*dt) + (x*y*dt)

  default values: dt=.02, a = 5, b = 15, c = 1;
  connect successive points with a line
*/
}

void dmartin(){
  int iterO=0;
  setColor( orbitC );
  point2 m1;
  m1.x=0;
  m1.y=0;
  while (!checkStop()){
    if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
    drawPixel( pointPixel( m1 ));
    xl=m1.x;
    yl=m1.y;
    m1.x=realA-xl;
    m1.y=(yl-m1.x)/(sign(xl)*sqrt( fabs((realB*xl)-realC)));
  };

//  x(0)=y(0)=0
//  x(n+1) = y(n) - sign(x(n))*sqrt(abs(b*x(n)-c))y(n+1)=a - x(n)
}

void dpickover( point3 p1 ){
  clearField();
  graphAxes();
  int iterO=0;
  point3 p1l;
  setColor( orbitC );
  for (i=1; i < 100; i++){
    p1l=p1;
    p1.x=sin(realA*p1l.y)-(p1l.z*cos(realB*p1l.x));
    p1.y=(p1l.z*sin(realC*p1l.x))-cos(realD*p1l.y);
    p1.z=sin(p1l.x);
  };
  while (!checkStop()){
    for (i=1; i<100; i++){
      if (coloringO == bymag) findCmagO( p1.z );
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      if (coloringO == bydist) findCdist( p1 );
      inFront( p1 ); //dot3D( p1 );
      p1l=p1;
      p1.x=sin(realA*p1l.y)-(p1l.z*cos(realB*p1l.x));
      p1.y=(p1l.z*sin(realC*p1l.x))-cos(realD*p1l.y);
      p1.z=sin(p1l.x);
    };
  };
}

  //possible constants: .02, 5, 5, .02
void drossler( point3 r1 ){   //realD:=dt
  int iterO=0;
  point3 r1l;
  setColor( orbitC );
  if (methodO == trail){
    while (!checkStop()){
      r1l=r1;
      r1.x=r1l.x-(r1l.y*realD)-(r1l.z*realD);
      r1.y=r1l.y+(r1l.x*realD)+(realA*r1l.y*realD);
      r1.z=r1l.z+(realB*realD)+(r1l.x*r1l.z*realD)-(realC*r1l.z*realD);
      if (coloringO == bymag) findCmagO( r1.z );
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      if (coloringO == bydist) findCdist( r1 );
      line3D( r1, r1l );    
    };
  };
  if (methodO == depth){
    clearField();
    graphAxes();
    while (!checkStop()){
      r1l=r1;
      r1.x=r1l.x-(r1l.y*realD)-(r1l.z*realD);
      r1.y=r1l.y+(r1l.x*realD)+(realA*r1l.y*realD);
      r1.z=r1l.z+(realB*realD)+(r1l.x*r1l.z*realD)-(realC*r1l.z*realD);
      if (coloringO == bymag) findCmagO( r1.z );
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      if (coloringO == bydist) findCdist( r1 );
      inFront( r1 );    
    };
  };
}

void dkamtorus(){    //realA constant, realB step size, intA maxit
/*  not working for some mysterious reason having to do with the k1
  double Orbit=0;
  double sina=sin(realA), cosa=cos(realA);
  point2 k1;
  int iterO;
  setColor( orbitC );
  while (!checkStop()){
    k1.x=Orbit/3;
    k1.y=Orbit/3;
    Orbit=Orbit+realB;
    iterO=0;
    for (i=1; i < intA; i++){
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      drawPixel( pointPixel( k1 ));
      xl=k1.x;
      yl=k1.y;
      k1.x=(xl*cosa) + (((xl*xl)-yl)*sina);
      k1.y=(xl*sina) + (((xl*xl)-yl)*cosa);      
    };
  };*/
  /*
  x(0) = y(0) = orbit/3;
  x(n+1) = x(n)*cos(a) + (x(n)*x(n)-y(n))*sin(a);
  y(n+1) = x(n)*sin(a) - (x(n)*x(n)-y(n)*cos(a);
  after each orbit, 'orbit is incremented by a step size (constant).
  parameters: ..... stop value for 'orbit' (try 5) and points per orbit
  the 3-D variant is produced by representing 'orbit' as the z-coordinate.*/

}

double Fdynamic1( double fd11 ){
  return cos(fd11); 
}

double Fdynamic( double fd1 ){
  return sin( fd1 + (realA*Fdynamic1(realB*fd1)));
}

void ddynamic(){
  //realA and realB are two constants, intA is maxit, realC is dt.
/* see above for the "reason" why this doesn't work now
  int iterO=0;
  setColor( orbitC );
  point2 d1;
  while (!checkStop()){
    d1.x=xmin+(xspan*drand48());
    d1.y=ymin+(yspan*drand48()); 
    for (i=0; i < intA; i++){
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      //drawPixel( pointPixel( d1 ) );
      xl=d1.x;
      yl=d1.y;
      //d1.x=yl+(realC*Fdynamic( xl ));
      //d1.y=(xl-d1.x)/(realC*Fdynamic( yl ));
      d1.x=xl-(realC*Fdynamic(yl));
      d1.y=yl+(realC*Fdynamic(xl));
      line2D( d1, makePoint(xl, yl) );
    };
  };*/  
  /*
  based on a cyclic system of differential equations x'(t)=-f(y(t))y'(t)=f(x(t))
  approximated by using a small time step dt....x(n+1)=x(n)-dt*f(y(n))y(n+1)=
     =y(n)+dt*f(x(n))
  the start values x(0) and y(0) correspond to pixels.
  f(k)=sin(k+a*fn1(b*k)) is one possibility.*/
}

void dginger( point2 g1 ){
  int iterO=0;
  //comment( "Press the right mouse button to end graphing. " );
  setColor( orbitC );
  while (!checkStop()){
    for (i=1; i<100; i++){
      if (coloringO == byiter){ iterO++; findCiterO( iterO ); };
      drawPixel( pointPixel( g1 ));
      xl=g1.x;
      yl=g1.y;
      g1.x = 1 - yl + fabs( xl );
      g1.y = xl;
    };
  };
}

char *orbitName( int omode ){
  if (omode == ginger) return "the Ginger attractor";
  if (omode == henon) return "the Henon attractor";
  if (omode == lorenz) return "the Lorenz attractor";
  if (omode == pickover) return "the Pickover attractor";
  if (omode == dynamic) return "a Dynamic fractal" ;
  if (omode == rossler) return "the Rossler attractor";
  if (omode == martin) return "the Martin attractor";
  if (omode == kamtorus) return "the Kam torus fractal";
}

void drawOrbit2( int do1, point2 do2 ){
  clear();
  orbitConstants();
  writeStopM();
  comment( "Graphing " ); addComment( orbitName( do1 ) );
  if (do1 == henon) dhenon( do2 );
  if (do1 == ginger) dginger( do2 );
  if (do1 == dynamic) ddynamic();
  if (do1 == martin) dmartin();
  if (do1 == kamtorus) dkamtorus();
  redraw();
  comment( "Orbit graphing finished. " );
}

void drawOrbit3( int do1, point3 do2 ){
  clear();
  orbitConstants();
  writeStopM();
  comment( "Graphing " ); addComment( orbitName( do1 ) );
  if (do1 == lorenz) dlorenz( do2 );
  if (do1 == pickover) dpickover( do2 );
  if (do1 == rossler) drossler( do2 );
  redraw();
  comment( "Orbit graphing finished." );
}

void orbitInfo(){
  resetPage();
  setColor( textC );
  printString( "This is " ); printLine( orbitName( orbitmode ) ); 
}

void orbitColors(){
  int maxitO;
  comment("Orbit coloring..." );
  setMenu(general);
  clearMenu();
  putMenu( cconstB, "constant" );
  putMenu( iterB, "by iteration" );
  if (orbitGroup( orbitmode ) == O3D){
    putMenu( magB, "by magnitude" );
    putMenu( distB, "by distance" );
  };
  getChoiceM( general );
  if (chosen( cconstB )){
    comment( "Enter the new orbit color. " ); 
    orbitC=findColor();
    coloringO=colconst;
  };
  if (chosen( iterB )){
    coloringO=byiter;
    maxitO=int(inputNumber( "Which iteration should reach the end color ? " ));
    newLine();
    cconstO3=colspanO/(maxitO*1.0);
  };   
  if (orbitGroup( orbitmode ) == O3D){
    if (chosen( magB )) coloringO=bymag; 
    if (chosen( distB )) coloringO=bydist;
  };
  if ((orbitmode==lorenz)||(orbitmode==rossler)){
    methodO=0;
    comment( "Graphing method..." );
    while (!methodO){
      setMenu(general);
      clearMenu();
      putMenu( depthB, "depth" );
      putMenu( trailB, "trail" );
      getChoiceM( general );
      if (chosen( depthB )) methodO=depth;
      if (chosen( trailB )) methodO=trail; 
    };
  };
}

void writeOrbit2Menu(){
  setColor( textC );
  putMenu( refreO2B, "refresh screen" );
  putMenu( zoomiO2B, "zoom in" );
  putMenu( zoomoO2B, "zoom out" );
  putMenu( constO2B, "constants" );
  putMenu( colorO2B, "colors" );
  putMenu( pointO2B, "point change" );
  putMenu( inforO2B, "information" );
  putMenu( graphO2B, "draw graph" ); 
  putMenu( windoO2B, "window" );
}

void writeOrbit3Menu(){
  setColor( textC );
  putMenu( refreO3B, "refresh screen" );
  putMenu( constO3B, "constants" );
  putMenu( colorO3B, "colors" );
  putMenu( pointO3B, "point change" );
  putMenu( inforO3B, "information" );
  putMenu( rotatO3B, "rotation" );  
  putMenu( rangeO3B, "range" );
  putMenu( graphO3B, "draw graph" );
}

void orbitMain(){
  int back;

  resetPixels();

  gingerB.x=1; henonB.x=2; lorenzB.x=3;
  martinB.y=1; pickoverB.y=1; rosslerB.y=1; kamtorusB.y=1;
  pickoverB.x=1; rosslerB.x=2; kamtorusB.x=3;

  constO2B.x=1; graphO2B.x=2; inforO2B.x=3;
  pointO2B.y=1; refreO2B.y=1; windoO2B.y=1; zoomiO2B.y=1;
  refreO2B.x=1; windoO2B.x=2; zoomiO2B.x=3;
  zoomoO2B.y=2;
  
  constO3B.x=1; graphO3B.x=2; inforO3B.x=3;
  pointO3B.y=1; refreO3B.y=1; rotatO3B.y=1; rangeO3B.y=1;
  refreO3B.x=1; rotatO3B.x=2, rangeO3B.x=3;
 
  magB.x=1; iterB.x=2; distB.x=3;
  
  trailB.x=1;

  while (1==1){
    resetAll();
    comment( "Orbit fractals: main menu. ");
    setMenu(general);
    writeOrbitSets();
    getChoiceM( general );
    if (chosen( lorenzB )) orbitmode = lorenz;
    if (chosen( henonB )) orbitmode = henon;
    if (chosen( pickoverB )) orbitmode = pickover;
    if (chosen( gingerB )) orbitmode = ginger;
    if (chosen( rosslerB )) orbitmode = rossler;
    if (chosen( dynamicB )) orbitmode = dynamic;
    if (chosen( martinB )) orbitmode = martin;
    if (chosen( kamtorusB )) orbitmode = kamtorus;
    if (backM()) return;
    if (quitM()) exit(1);
    orbitInputs();
    if ( orbitGroup( orbitmode ) == O2D ) rrange2D();
    if ( orbitGroup( orbitmode ) == O3D ) rrange3D();
    orbitColors();
    if ((orbitmode==martin)||(orbitmode==dynamic)||(orbitmode==kamtorus)){
       drawOrbit2( orbitmode, nopoint2 );
       back=0;
       while (!back){
         setMenu(panelmenu);
         clearMenu();
         comment( "2-dimensional orbit fractals: function menu. " );
         writeOrbit2Menu();
         getChoiceM( panelmenu );
         if (chosen( refreO2B )) resetAll();
         if (chosen( constO2B )){ resetPage(); orbitInputs(); };
         if (chosen( colorO2B )) orbitColors();
         if (chosen( graphO2B )) drawOrbit2( orbitmode, nopoint2 );
         if (chosen( inforO2B )) orbitInfo();
         if (chosen( windoO2B )) rrange2D();
         if (chosen( zoomiO2B )){
           zoom2D; drawOrbit2( orbitmode, nopoint2 );};
         if (chosen( zoomoO2B )){
           zoomOut2D; drawOrbit2( orbitmode, nopoint2 );};
         if (backM()) back=1;
         if (quitM()) exit(1);
         boxM=-2;
       };
    };
    if ((orbitmode==lorenz)||(orbitmode==pickover)||(orbitmode==rossler)){
       setMenu(panelmenu);
       writeRotControls();
       parAxes(1);
       current3 = getPoint3();
       drawOrbit3( orbitmode, current3 );
       back=0;
       while (!back){
         clearMenu();
         comment( "3-dimensional orbit fractals: function menu. " );
         writeOrbit3Menu();
         getChoiceM( panelmenu );
         if (chosen( refreO3B )) resetAll();
         if (chosen( constO3B )){ resetPage(); orbitInputs(); };
         if (chosen( colorO3B )) orbitColors();
         if (chosen( graphO3B )) drawOrbit3( orbitmode, current3 );
         if (chosen( inforO3B )) orbitInfo();
         if (chosen( rangeO3B )){ rrange3D(); parConsts(); };
         if (chosen( rotatO3B )){ writeRotControls(); parAxes(0);};
         if (chosen( pointO3B )) current3=getPoint3();
         if (backM()) back=1;
         if (quitM()) exit(1);
         boxM=-2;
       };
    };
    if ((orbitmode==ginger)||(orbitmode==henon)){
       current2=pixelPoint( readPoint() );
       drawOrbit2( orbitmode, current2 );
       back=0;
       while (!back){
         setMenu(panelmenu);
         clearMenu();
         comment( "2-dimensional orbit fractals: function menu. " );
         writeOrbit2Menu();
         getChoiceM( panelmenu );
         if (chosen( refreO2B )) resetAll();
         if (chosen( constO2B )){ resetPage(); orbitInputs(); };
         if (chosen( colorO2B )) orbitColors();
         if (chosen( graphO2B )) drawOrbit2( orbitmode, current2 );
         if (chosen( inforO2B )) orbitInfo();
         if (chosen( windoO2B )) rrange2D();
         if (chosen( zoomiO2B )){
           zoom2D(); drawOrbit2( orbitmode, nopoint2 );};
         if (chosen( zoomoO2B )){
           zoomOut2D(); drawOrbit2( orbitmode, nopoint2 );};
         if (chosen( pointO2B )){
           current2=pixelPoint(readPoint()); 
           drawOrbit2( orbitmode, current2 );
         };
         if (backM()) back=1;
         if (quitM()) exit(1);
         boxM=-2;
       };
    };
  };
}


