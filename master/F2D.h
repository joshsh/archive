//area constants
pixel xvalB, yvalB;
pixel lagrangeB;

double eqF2D( double xeq ){

  return xeq*cos(sin(xeq*xeq));
  
}

void drawF2D(){
  point2 p, pl;
  clear();
  drawAxes();
  setColor( graphC );
  p.x=xmin;
  p.y=eqF2D(p.x);
  for (i=0; i<xpix; i++){
    pl=p;
    p.x=xmin+(xinc*i);
    p.y=eqF2D( p.x );
    line2D( p, pl );
  };
  redraw();
}

void integralF2D(){
  resetPage();
  setColor( textC );
  double xmini=inputNumber( "x minimum = " );
  double xmaxi=inputNumber( "x maximum = " );
  int intdiv=inputNumber( "How many divisions do you want to use? " );
  double xinci=(xmaxi-xmini)/(intdiv*1.0);
  double intarea=0;
  for (i=0; i<intdiv; i++){
    intarea=intarea+(xinci*(eqF2D(xmini+(i*xinci))));
  };
  printString("The area under the curve is "); printNumber( intarea );
  newLine(); newLine();
}

void derivativeF2D(){
  resetPage();
  setColor( textC );
  double xder=inputNumber( "Find the first derivative at x = ");
  newLine();
  double rder, lder, lastder, dxd=1;
  while ( trunc(lastder, disdigits) != trunc(rder, disdigits) ){
    lastder=rder;
    dxd=dxd/10;
    rder=(eqF2D(xder+dxd)-eqF2D(xder))/(dxd);
  };
  dxd=1;
  while ( trunc(lastder, disdigits) != trunc(lder, disdigits) ){
    lastder=lder;
    dxd=dxd/10;
    lder=(eqF2D(xder)-eqF2D(xder-dxd))/(dxd);
  };
  printString("Right-sided derivative = "); printNumber(rder); newLine();
  printString("Left-sided derivative = "); printNumber(lder); newLine();
  newLine();
}

void lengthF2D(){
  resetPage();
  realA=inputNumber( "lower x - value = " );
  realB=inputNumber( "upper x - value = " );
  if (realB < realA){
    realC=realA;
    realA=realB;
    realB=realC;
  };
  if (realA == realB){
    printLine( "The length of the curve is 0 (as you'd expect)." );
    return;
  };
  intA=inputNumber( "How many divisions do you want to use?" );
  double tinc=(realB-realA)/(intA*1.0);
  double xl, yl;
  x=realA;
  y=eqF2D(x);
  realC=0;
  for (i=1; i <= intA; i++){
    xl=x;
    yl=y;
    x=realA+(tinc*i);
    y=eqF2D(x);
    realC=realC+sqrt(sq(x-xl)+sq(y-yl));
  };
  newLine();
  printString("The length of the curve is "); printNumber(realC); newLine();
}

void valuesF2D(){
  setMenu(general);
  resetPage();
  setColor( textC );
  printLine( "Find an x- or a y-value (see below) ? " );
  clearMenu();
  setColor( textC );
  putMenu( xvalB, "x - value" );
  putMenu( yvalB, "y - value" );
  char valtype = 'n';
  while ((valtype != 'x')&&(valtype != 'y')){
    getChoice( general );
    if (chosen( xvalB )) valtype='x';
    if (chosen( yvalB )) valtype='y';
    if (backM()) return;
    if (quitM()) exit(1);
  };
  if ( valtype == 'y' ){
     double xval = inputNumber( "At which x - value ? " );
     printString( "y = " ); printNumber( eqF2D(xval) ); newLine();
  };
  if ( valtype == 'x' ){
    //write something in here.
  };
  newLine();
}

void traceF2D(){
  int lastx, lasty;
  Event e;
  updateEvents();
  writeStopM();
  comment( "Coordinates dispayed on the page. " );
  while (1==1){
    if (e.type == MotionNotify){
      resetPage();
      printString( "x = " ); printNumber( xmin+(e.x*xspan/xpix) ); newLine();
      printString( "y = " ); printNumber( eqF2D(xmin+(e.x*xspan/xpix)));
      copyArea( pix, w, lastx, 0, 1, ypix, lastx, 0);
      if (e.x < xpix) drawLine(w, e.x, 0, e.x, ypix-1); 
      lastx=e.x;
      lasty=e.y;
    };
    if (checkStop()){
      copyArea( pix, w, lastx, 0, 1, ypix, lastx, 0);
      resetPage();
      return;
    };
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
}

void interceptsF2D(){
  pixel cor1, cor2;
  resetPage();
  comment( "Box in the intercept with the mouse, right button to quit.");
  Event e=getNextEvent();
  cor1=readPoint();
  setDrawMode(GXxor);
  e=nextEvent();
  if (e.type == Expose) redraw();
  int ybot, ytop, xleft, xright;
  int lastC = getColor();
  while (e.type != ButtonPress){
    if (e.type == Expose) redraw();
    xleft=e.x; ytop=e.y;
    if (cor1.x < xleft) xleft=cor1.x;
    if (cor1.y < ytop) ytop=cor1.y;
    drawRectangle(w, xleft, ytop, fabs(e.x-cor1.x), fabs(e.y-cor1.y) );   
  }; 
  if (e.value == 3){ setDrawMode(GXcopy); setColor(lastC); return; };
  //else
  xleft=e.x;
  if (cor1.x < xleft) xleft=cor1.x;
  double xlow=(xleft*xspan/xpix) + xmin;
  double tinc=((fabs(e.x-cor1.x)/xpix)*xspan)/1000;     //....
  double xtop=xlow+(tinc*1000);                         //....same number!
  int stop=0;
  while (!stop){
    x=xlow;
    while ((sign(eqF2D(x)) == sign(eqF2D(xlow)))&&(!stop)){
      x=x+tinc;
      if (x >= xtop + tinc){
        printLine( "No intercept found in the area boxed-in." );
        stop=1;
      };
    };
    if (trunc(x, disdigits) == trunc(xlow, disdigits)){
      printString( "Intercept found at x = " ); printNumber( x ); newLine();
      stop=1;
    };
    xlow=x-tinc;
    tinc=tinc/10;
  };
  setDrawMode(GXcopy);
  setColor( lastC );
}

void F2DMain(){

  //assign menu positions
  setMenu(general);
  resetMenu();
  draw2DB=addMenu();
  color2DB=addMenu();
  derivative2DB=addMenu();
  window2DB=addMenu();
  values2DB=addMenu();
  trace2DB=addMenu();
  length2DB=addMenu();
  inter2DB=addMenu();
  integral2DB=addMenu();
  lagrangeB=addMenu();
  zoomi2DB=addMenu();
  zoomo2DB=addMenu();
  newMainMenu();
  xvalB=addMenu();
  yvalB=addMenu();
  
  rrange2D();
  drawF2D();
  
  resetAll();
  while (1==1){
    setMenu(general);
    clearMenu();
    setColor( textC );
    putMenu( draw2DB, "Graph function" );
    putMenu( inter2DB, "Intercepts" );
    //putMenu( change2DB, "Change equation" );
    putMenu( color2DB, "Colors" );
    putMenu( windo2DB, "Window" );
    putMenu( integral2DB, "Integral" );
    putMenu( derivative2DB, "Derivative" );
    putMenu( values2DB, "Function values" );
    putMenu( trace2DB, "Trace" );
    putMenu( zoomi2DB, "Zoom in" );
    putMenu( zoomo2DB, "Zoom out" );
    putMenu2( length2DB, "Length of", "curve" );
    putMenu2( lagrangeB, "Lagrange", "interpolation" );
    
    getChoice( general );
    //if (chosen( change2DB ))
    if (chosen( color2DB )){ colorsF2D; drawF2D(); };
    if (chosen( derivative2DB )) derivativeF2D();
    if (chosen( values2DB )) valuesF2D();
    if (chosen( draw2DB )) resetAll();
    if (chosen( integral2DB )) integralF2D();
    if (chosen( inter2DB )) interceptsF2D();
    if (chosen( trace2DB )) traceF2D();
    if (chosen( windo2DB )){ rrange2D(); drawF2D(); };
    if (chosen( zoomi2DB )){ zoom2D(); drawF2D(); };
    if (chosen( zoomo2DB )){ zoomOut2D; drawF2D(); };
    if (chosen( length2DB )) lengthF2D();
    if (backM()) return;
    if (quitM()) exit(1);
  };
}

