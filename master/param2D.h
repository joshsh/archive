//later, add integrals, intercepts, etc. to this

point2 eqParam2D( double teq ){
  return mpoint2(sin(teq)*sq(cos(teq)), cos(teq)*sq(sin(teq))); 
}

void drawParam2D(){
  point2 p, pl;
  clear();
  drawAxes();
  setColor( graphC );
  t=tmin;
  p=eqParam2D(t);
  for (i=0; t<=tmax; i++){
    pl=p;
    t=tmin+(tinc*i);
    p=eqParam2D(t);
    line2D( p, pl );
  };
  redraw();
}

void derivativeParam2D(){
  resetWindow();
  setColor( textC );
  double tder=inputNumber( "Find the first derivative at t = ");
  newLine();
  double rder, lder, lastder, dtd=1;
  point2 pder, q;
  lastder=1; rder=2; lder=3; //(arbitrary)
  while ( trunc(lastder, disdigits) != trunc(rder, disdigits) ){
    lastder=rder;
    dtd=dtd/10;
    qder=eqParam2D(tder+dtd);
    rder=(qder.x-pder.x)/dtd;
  };
  dtd=1;
  while ( trunc(lastder, disdigits) != trunc(lder, disdigits) ){
    lastder=lder;
    dtd=dtd/10;
    qder=eqParam2D(tder-dtd);
    lder=(pder.x-qder.x)/dtd;
  };
  printString("Right-sided derivative for x = "); printNumber(rder); newLine();
  printString("Left-sided derivative for x = "); printNumber(lder); newLine();
  double rderx=rder;
  double lderx=lder;
  newLine();
  dtd=1;
  lastder=1; rder=2; lder=3; //(arbitrary)
  while ( trunc(lastder, disdigits) != trunc(rder, disdigits) ){
    lastder=rder;
    dtd=dtd/10;
    qder=eqParam2D(tder+dtd);
    rder=(qder.y-pder.y)/dtd;
  };
  dtd=1;
  while ( trunc(lastder, disdigits) != trunc(lder, disdigits) ){
    lastder=lder;
    dtd=dtd/10;
    qder=eqParam2D(tder-dtd);
    lder=(pder.y-qder.y)/dtd;
  };
  printString("Right-sided derivative for y = "); printNumber(rder); newLine();
  printString("Left-sided derivative for y = "); printNumber(lder); newLine();
  newLine();
  printLine( "So the combinations of dy/dx are: " ); newLine();
  printNumber(rder/rderx); newLine();
  printNumber(rder/lderx); newLine();
  printNumber(lder/rderx); newLine();
  printNumber(lder/lderx); newLine();
  newLine();
}

void lengthParam2D(){
  resetWindow();
  realA=inputNumber( "lower value for t = " );
  realB=inputNumber( "upper value for t = " );
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
  double tinct=(realB-realA)/(intA*1.0);
  point2 p, pl;
  t=realA;
  p=eqParam2D(t);
  realC=0;
  for (i=1; i <= intA; i++){
    pl=p;
    t=realA+(tinct*i);
    p=eqParam2D(t);
    realC=realC+dist2D(pl, p);
  };
  newLine();
  printString("The length of the curve is "); printNumber(realC); newLine();
}

void valuesParam2D(){
  resetWindow();
  setColor( textC );
  double t =inputNumber( "enter a value for t " ); newLine(); newLine();
  point3 p = eqParam2D(t);
  printString( "x = " ); printNumber(p.x); newLine();
  printString( "y = " ); printNumber(p.y); newLine();
}

void traceParam2D(){
  int lastx, lasty;
  point2 p;
  pixelq;
  Event e;
  updateEvents();
  writeStop();
  comment( "Coordinates dispayed on the page. " );
  while (1==1){
    if (e.type == MotionNotify){
      resetWindow();
      t=tmin+(e.x*tspan/xpix);
      p=eqParam(t);
      q.x=xpix*(p.x-xmin)/xspan;
      q.y=ypix*(p.y-ymin)/yspan;
      printString( "t = " ); printNumber(t), newLine(); newLine();
      printString( "x = " ); printNumber(p.x); newLine();
      printString( "y = " ); printNumber(p.y);
      copyArea( pix, w, lastx, 0, 1, ypix, lastx, 0);
      copyArea( pix, w, 0, lasty, xpix, 1, 0, lasty);
      if ((q.x>0)&&(q.x < xpix) drawLine(w, q.x, 0, q.x, ypix-1); 
      if ((q.y>0)&&(q.y < ypix) drawLine(w, 0, q.y, xpix-1, q.y);
      lastx=q.x;
      lasty=q.y;
    };
    if (checkStop()){
      copyArea( pix, w, lastx, 0, 1, ypix, lastx, 0);
      copyArea( pix, w, 0, lasty, xpix, 1, 0, lasty);      
      resetWindow();
      return;
    };
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
}

void param2DMain(){
  int box;

  //assign menu positions
  newMainMenu();
  draw2DB=addMainMenu();
  color2DB=addMainMenu();
  range2DB=addMainMenu();
  derivative2DB=addMainMenu();
  values2DB=addMainMenu();
  trace2DB=addMainMenu();
  length2DB=addMenu();
  zoomi2DB=addMainMenu();
  zoomo2DB=addMainMenu();

  rrange2D();
  trange();
  drawParam2D();
  
  resetAll();
  while (1==1){
    clearMenu();
    setColor( textC );
    putMenu( draw2DB, "Graph function" );
    putMenu( change2DB, "Change equation" );
    putMenu( color2DB, "Colors" );
    putMenu( range2DB, "Range" );
    putMenu( derivative2DB, "Derivative" );
    putMenu( values2DB, "Function values" );
    putMenu( trace2DB, "Trace" );
    putMenu( zoomi2DB, "Zoom in" );
    putMenu( zoomo2DB, "Zoom out" );
    putMenu2( length2DB, "Length of", "curve" );
    
    box=getChoice( general );
    if (menuNumber( color2DB ) == box){ colors2D; drawParam2D(); };
    if (menuNumber( derivative2DB ) == box) derivativeParam2D();
    if (menuNumber( values2DB ) == box) valuesParam2D();
    if (menuNumber( draw2DB ) == box) resetAll();
    if (menuNumber( trace2DB ) == box) traceParam2D();
    if (menuNumber( range2DB ) == box){ rrange2D(); trange(); drawParam2D(); };
    if (menuNumber( zoomi2DB ) == box){ zoom2D(); drawParam2D(); };
    if (menuNumber( zoomo2B ) == box){ zoomOut2D; drawParam2D(); };
    if (menuNumber( length2DB ) == box) lengthParam2D();
    if (menuNumber( backB ) == box) return;
    if (menuNumber( quitB ) == box) exit(1);
  };
}

