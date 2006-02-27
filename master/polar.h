//later, add integrals, intercepts, etc. to this

  //returns the function value
double eqPolar( double teq ){
  return sin(teq)*cos(teq); 
}

  //returns the point of the parametrization
point2 polPoint( double teq );
  double req=eqPolar( teq );
  return mpoint2( r*cos(t), r*sin(t));
}

void drawPolar(){
  point2 p, pl;
  clear();
  drawAxes();
  setColor( graphC );
  t=tmin;
  p=polPoint(t);
  for (i=0; t<=tmax; i++){
    pl=p;
    t=tmin+(tinc*i);
    p=polPoint(t);
    line2D( p, pl );
  };
  redraw();
}

void derivativePolar(){
  resetWindow();
  setColor( textC );
  double tder=inputNumber( "Find the first derivative at t = ");
  newLine();
  double rder, lder, lastder, dtd=1;
  point2 pder, q;
  lastder=1; rder=2; lder=3; //(arbitrary)

  realA=eqPolar(tder);
  while ( trunc(lastder, disdigits) != trunc(rder, disdigits) ){
    lastder=rder;
    dtd=dtd/10;
    rder=(eqPolar(tder+dtd)-realA)/dtd;
  };
  dtd=1;
  while ( trunc(lastder, disdigits) != trunc(lder, disdigits) ){
    lastder=lder;
    dtd=dtd/10;
    lder=(realA-eqPolar(tder-dtd))/dtd;
  };
  printString("Right-sided derivative for r = "); printNumber(rder); newLine();
  printString("Left-sided derivative for r = "); printNumber(lder); newLine();
  newLine();

  lastder=1; rder=2; lder=3; //(arbitrary)
  while ( trunc(lastder, disdigits) != trunc(rder, disdigits) ){
    lastder=rder;
    dtd=dtd/10;
    qder=polPoint(tder+dtd);
    rder=(qder.x-pder.x)/dtd;
  };
  dtd=1;
  while ( trunc(lastder, disdigits) != trunc(lder, disdigits) ){
    lastder=lder;
    dtd=dtd/10;
    qder=polPoint(tder-dtd);
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
    qder=polPoint(tder+dtd);
    rder=(qder.y-pder.y)/dtd;
  };
  dtd=1;
  while ( trunc(lastder, disdigits) != trunc(lder, disdigits) ){
    lastder=lder;
    dtd=dtd/10;
    qder=polPoint(tder-dtd);
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

void lengthPolar(){
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
  p=polPoint(t);
  realC=0;
  for (i=1; i <= intA; i++){
    pl=p;
    t=realA+(tinct*i);
    p=polPoint(t);
    realC=realC+dist2D(pl, p);
  };
  newLine();
  printString("The length of the curve is "); printNumber(realC); newLine();
}

void valuesPolar(){
  resetWindow();
  setColor( textC );
  double t =inputNumber( "enter a value for t " ); newLine(); newLine();
  point3 p = polPoint(t);
  printString( "r = " ); printNumber( eqPolar(t) ); newLine();
  printString( "x = " ); printNumber(p.x); newLine();
  printString( "y = " ); printNumber(p.y); newLine();
}

void tracePolar(){
  int lastx, lasty;
  point2 p;
  pixelq;
  Event e;
  updateEvents();
  writeStop();
  comment( "Coordinates dispayed on the page. " );
  int tpx=xpix/2;
  int tpy=ypix/2;
  while (1==1){
    if (e.type == MotionNotify){
      resetWindow();
      t=Arctan2((e.x-tpx, e.y-tpy);
      p=polPoint(t);
      q.x=xpix*(p.x-xmin)/xspan;
      q.y=ypix*(p.y-ymin)/yspan;
      printString( "t = " ); printNumber(t), newLine();
      printString( "r = " ); printNumber( eqPolar(t) ); newLine(); newLine();
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

void polarMain(){
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
    if (menuNumber( color2DB ) == box){ colors2D; drawPolar(); };
    if (menuNumber( derivative2DB ) == box) derivativePolar();
    if (menuNumber( values2DB ) == box) valuesPolar();
    if (menuNumber( draw2DB ) == box) resetAll();
    if (menuNumber( trace2DB ) == box) tracePolar();
    if (menuNumber( range2DB ) == box){ rrange2D(); trange(); drawPolar(); };
    if (menuNumber( zoomi2DB ) == box){ zoom2D(); drawPolar(); };
    if (menuNumber( zoomo2B ) == box){ zoomOut2D; drawPolar(); };
    if (menuNumber( length2DB ) == box) lengthPolar();
    if (menuNumber( backB ) == box) return;
    if (menuNumber( quitB ) == box) exit(1);
  };
}

