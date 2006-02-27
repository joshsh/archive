
int relationC;

double rel2D( point2 r1 ){
  return sq(r1.x)+sq(r1.y)-1;
}

void drelation2D(){
  point2 p, pl;
  clear();
  drawAxes();
  setColor( graphC );
  for (i=0; i<xpix; i++){
    pl=p;
    p.x=xmin+(xinc*i);
    p.y=eqF2D( p.x );
    line2D( p, pl );
  };
  redraw();
}

void traceF2D(){
  int lastx, lasty;
  Event e;
  updateEvents();
  writeStop();
  comment( "Coordinates dispayed on the page. " );
  //setDrawMode( GXxor );
  while (1==1){
    if (e.type == MotionNotify){
      resetWindow();
      printString( "x = " ); printNumber( xmin+(e.x*xspan/xpix) ); newLine();
      printString( "y = " ); printNumber( eqF2D(xmin+(e.x*xspan/xpix)));
      copyArea( pix, w, lastx, 0, 1, ypix, lastx, 0);
      if (e.x < xpix) drawLine(w, e.x, 0, e.x, ypix-1); 
      lastx=e.x;
      lasty=e.y;
    };
    if (checkStop()) return;
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
  //setDrawMode( GXcopy );
}

void colorsF2D(){
  graphC=inputNumber( "Color for function graph = " );
  axesC=inputNumber( "Color for axes = " );
  griddotC=inputNumber( "Grid color = " );
  newLine();
}

void writeF2DOptions(){
  setColor( textC );
  putMenu( drawF2B, "Graph function" );
  putMenu( interF2B, "Intercepts" );
  putMenu( changeF2B, "Change equation" );
  putMenu( colorF2B, "Colors" );
  putMenu( windoF2B, "Window" );
  putMenu( integralF2B, "Integral" );
  putMenu( derivativeF2B, "Derivative" );
  putMenu( valuesF2B, "Function values" );
  putMenu( traceF2B, "Trace" );
  putMenu( zoomiF2B, "Zoom in" );
  putMenu( zoomoF2B, "Zoom out" );
}

void relationMain(){
  int box;

  //assign menu positions (all coordinates not written are 0)

  rrange2D();
  drawF2D();
  
  resetAll();
  while (1==1){
    clearMenu();
    writeF2DOptions();
    box=getChoice( general );
    if (menuNumber( changeF2B ) == box)
    if (menuNumber( colorF2B ) == box){ colorsF2D; drawF2D(); };
    if (menuNumber( derivativeF2B ) == box) derivativeF2D();
    if (menuNumber( valuesF2B ) == box) valuesF2D();
    if (menuNumber( drawF2B ) == box) resetAll();
    if (menuNumber( integralF2B ) == box) integralF2D();
    if (menuNumber( interF2B ) == box) interceptsF2D();
    if (menuNumber( traceF2B ) == box) traceF2D();
    if (menuNumber( windoF2B ) == box){ rrange2D(); drawF2D(); };
    if (menuNumber( zoomiF2B ) == box){ zoom2D(); drawF2D(); };
    if (menuNumber( zoomoF2B ) == box){ zoomOut2D; drawF2D(); };
    if (menuNumber( backB ) == box) return;
    if (menuNumber( quitB ) == box) exit(1);
  };
}

