struct map{
  point3 p;
  int col;
};

int ypix2=650;
int interC=40, gridC;
int crossC=5;
int griddet, integdet=2000;
//int xs0,ys0;
int trdiv=3000;   //divisions of the surface for trace().
int parallel=1, central=2;
int gridgraph=1, dotgraph=2, gradis=3, thick=4;


//menu positions
pixel parallelB, centralB;
pixel planecutB, derivativeB, integralB, areaB, lengB;
pixel analysisF3DB, constF3DB, rotF3DB, traceF3DB, graphF3DB, contourB;
pixel dotsB, gridB, gradientsB, thickB;

pixel gWcor;
int inidet=100;  //detail for color determination initC().
int sightmode = 1;  //1..parallel projection 2..central projection
int drawmode; 
//double zmin,zmax,
double xmint,xmaxt,ymint,ymaxt,zmint,zmaxt;
double xspant,yspant,zspant;
double xcen, ycen, zcen;
double xt,yt,dxt,dyt;
double xinct,yinct,zinct;
double xsc, ysc;
double xsct,ysct;
double dconst;  //distance of central projection plane from midpoint
int mapdet=21;
map grid[21][21];  //[mapdet][mapdet] !

double eq3DF(double x, double y){
  //return 1/((((x*x)+(y*y)-1)*((x*x)+(y*y)-1))+1);
  //return (sq(x-1)+sq(y))*(sq(x+1)+sq(y));
  //return (1/(sq(x)+sq(y)))*sin((x*x*x)+(y*y*y*y)); 
  //return (x + (1/3))*( sq(x) + (y*y*y) - y - (2/(3*sqrt(3))));
  //return ((x*x) + (y*y/4) - 1)*((y*y) + (x*x/4) - 1);
  //return cos(x) + cos(y);
  return cos(x*x)+cos(y);
}

point2 gradientF3D( double x, double y ){
  Z=eq3DF( x, y );
  x=x+(xinc/100);
  y=y+(yinc/100);
  dz=eq3DF( x, y )-Z;
  point2 vel;
  vel.x=dz/(xinc/100);
  vel.y=dz/(yinc/100);
  return vel;
}

void range3D(){
  resetWindow();
  xmin=inputNumber( "x - minimum = " ); newLine();
  xmax=inputNumber( "x - maximum = " ); newLine();
  ymin=inputNumber( "y - minimum = " ); newLine();
  ymax=inputNumber( "y - maximum = " ); newLine();
  zmid=inputNumber( "Middle z - value = " ); newLine();
  xmid=(xmax+xmin)/2;
  ymid=(ymax+ymin)/2;
  xspan=xmax-xmin;
  yspan=ymax-ymin;
}

void initCF3D(){  //xmin, xmax, ... must be defined before use of initCF3D.
  double thisdist, mind, maxd;
  point3 inp;
  xsct=xspan/inidet;
  ysct=yspan/inidet;
  inp.z=eq3DF(xmin,xmax);
  minz=inp.z; maxz=inp.z;
  thisdist=sq(inp.x-xmid)+sq(inp.y-ymid)+sq(inp.z-zmid);
  mind=thisdist; maxd=thisdist;
  for (i=0; i<=inidet+1; i++){
    inp.x=xmin+(i*xsct);
    for (j=0; j<=inidet+1; j++){
      inp.y=ymin+(j*ysct);
      inp.z=eq3DF(inp.x, inp.y);
      thisdist=sq(inp.x-xmid)+sq(inp.y-ymid)+sq(inp.z-zmid);
      if (inp.z < minz) minz=inp.z;
      if (inp.z > maxz) maxz=inp.z;
      if (thisdist < mind) mind=thisdist;
      if (thisdist > maxd) maxd=thisdist;
    };
  };
  if ((maxz-zmid) > (2.5*(xspan+yspan))) maxz=zmid+((xspan+yspan)/2);
  if ((zmid-minz) > (2.5*(xspan+yspan))) minz=zmid-((xspan+yspan)/2);
  //cconst3D=colspan3D/(maxz-minz);
  initC( minz, maxz );
  distCconst1=highC+((colspan3D*mind)/(maxd-mind));
  distCconst2=-colspan3D/(maxd-mind);
}

void findGridC(){
  gridC=(drand48()*170)+1;
  if (gridC == backC) gridC=backC+1;
}

void makeGrid(){
  point3 gp;
  xsct=xspan/(mapdet-1);
  ysct=yspan/(mapdet-1);
  if ( coloring == colconst ){
    for (i=0; i<mapdet; i++){
      for (j=0; j<mapdet; j++) grid[i][j].col=gridC;
    };
    for (i=0; i<mapdet; i++){
      gp.x=xmin+(i*xsct);
      for (j=0; j<mapdet; j++){
        gp.y=ymin+(j*ysct);
        gp.z=eq3DF(gp.x, gp.y);
        grid[i][j].p = gp;
      };
    };
  };
  if ( coloring == bymag ){
    for (i=0; i<mapdet; i++){
      gp.x=xmin+(i*xsct);
      for (j=0; j<mapdet; j++){
        gp.y=ymin+(j*ysct);
        gp.z=eq3DF(gp.x, gp.y);
        grid[i][j].p = gp;
        grid[i][j].col=returnC(gp.z);
      };
    };
  };
  if ( coloring == bydist ){
    for (i=0; i<mapdet; i++){
      gp.x=xmin+(i*xsct);
      for (j=0; j<mapdet; j++){
        gp.y=ymin+(j*ysct);
        gp.z=eq3DF(gp.x, gp.y);
        grid[i][j].p = gp;
        grid[i][j].col=returnCdist(gp);
      };
    };
  };
}


void cenAxes(){
  dconst=inputNumber( "Distance of central projection plane from midpoint = ");
  newLine();
}

void axes(){
  clear();
  if (sightmode == parallel){ parAxes(1); parConsts(); };
  if (sightmode == central) cenAxes();
}

void gridGraph(){
  point3 gp, gpl;
  if ( coloring == colconst ){
    setColor( gridC );
    for (i=0; i<=griddet+1; i++){
      gp.x=xmin+(i*xsc);
      gp.y=ymin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (j=0; j<=griddet+1; j++){
        gp.y=ymin+(j*ysc);
        gp.z=eq3DF(gp.x, gp.y);
        line3D(gpl, gp );
        gpl=gp;
      };
    };
    for (i=0; i<=griddet+1; i++){
      gp.y=ymin+(i*ysc);
      gp.x=xmin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (j=0; j<=griddet+1; j++){
        gp.x=xmin+(j*xsc);
        gp.z=eq3DF(gp.x, gp.y);
        line3D(gpl, gp );
        gpl=gp;
      };
    };
  };
  if ( coloring == bymag ){
    for (i=0; i<=griddet+1; i++){
      gp.x=xmin+(i*xsc);
      gp.y=ymin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (j=0; j<=griddet+1; j++){
        gp.y=ymin+(j*ysc);
        gp.z=eq3DF(gp.x, gp.y);
        findC( (gp.z+gpl.z)/2 );
        line3D(gpl, gp);
        gpl=gp;
      };
    };
    for (i=0; i<=griddet+1; i++){
      gp.y=ymin+(i*ysc);
      gp.x=xmin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (j=0; j<=griddet+1; j++){
        gp.x=xmin+(j*xsc);
        gp.z=eq3DF(gp.x, gp.y);
        findC( (gp.z+gpl.z)/2 );
        line3D(gpl, gp);
        gpl=gp;
      };
    };
  };
  if ( coloring == bydist ){
    for (i=0; i<=griddet+1; i++){
      gp.x=xmin+(i*xsc);
      gp.y=ymin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (j=0; j<=griddet+1; j++){
        gp.y=ymin+(j*ysc);
        gp.z=eq3DF(gp.x, gp.y);
        findCdist( gp );
        line3D(gpl, gp);
        gpl=gp;
      };
    };
    for (i=0; i<=griddet+1; i++){
      gp.y=ymin+(i*ysc);
      gp.x=xmin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (j=0; j<=griddet+1; j++){
        gp.x=xmin+(j*xsc);
        gp.z=eq3DF(gp.x, gp.y);
        findCdist( gp );
        line3D(gpl, gp);
        gpl=gp;
      };
    };
  };
}

void thickGrid(){
  point3 gp, gpl;
  if ( coloring == colconst ){
    setColor( gridC );
    for (gp.x=xmin; gp.x <= xmax; gp.x=gp.x+xsc){
      gp.y=ymin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (gp.y=ymin; gp.y <= ymax; gp.y=gp.y+ysc){
        gp.z=eq3DF(gp.x, gp.y);
        line3D(gpl, gp );
        gpl=gp;
      };
    };
  };
  if ( coloring == bymag ){
    for (gp.x=xmin; gp.x <= xmax; gp.x=gp.x+xsc){
      gp.y=ymin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (gp.y=ymin; gp.y <= ymax; gp.y=gp.y+ysc){
        gp.z=eq3DF(gp.x, gp.y);
        findC(gp.z);
        line3D(gpl, gp );
        gpl=gp;
      };
    };
  };
  if ( coloring == bydist ){
    for (gp.x=xmin; gp.x <= xmax; gp.x=gp.x+xsc){
      gp.y=ymin;
      gp.z=eq3DF(gp.x, gp.y);
      gpl=gp;
      for (gp.y=ymin; gp.y <= ymax; gp.y=gp.y+ysc){
        gp.z=eq3DF(gp.x, gp.y);
        findCdist(gp);
        line3D(gpl, gp );
        gpl=gp;
      };
    };
  };
}

void dotGraph(){
  point3 dg;
  writeStopM();
  comment( "Point graphing in progress. ");
  clearField();
  graphAxes();
  if (coloring == bymag){
    while (1==1){
      if (checkStop()) return;
      for (i=1; i<300; i++){
        dg.x=xmin+(drand48()*xspan);
        dg.y=ymin+(drand48()*yspan);
        dg.z=eq3DF(dg.x, dg.y);
        findC(dg.z);
        inFront(dg);
      };
    };
  };
  if (coloring == bydist){
    while (1==1){
      if (checkStop()) return;
      for (i=1; i<300; i++){
        dg.x=xmin+(drand48()*xspan);
        dg.y=ymin+(drand48()*yspan);
        dg.z=eq3DF(dg.x, dg.y);
        findCdist(dg);
        inFront(dg);
      };
    };
  }; 
  if (coloring == colconst)
   comment("Drawing a point graph with constant color is pointless. ");
}

void gradientsF3D(){
  point2 vel;
  point3 pb, p, pl;
  writeStopM();
  clear();
  comment( "Graphing gradient paths. " );
  while (!checkStop()){
    pb.x=xmin+(xspan*drand48());
    pb.y=ymin+(yspan*drand48());
    p=pb;
    if (coloring == bymag){
      while ((p.x >= xmin)&&(p.x <= xmax)&&(p.y >= ymin)&&(p.y <= ymax)){
        p.z=eq3DF( p.x, p.y );
        findC(p.z);
        inFront(p);
        vel=gradientF3D( p.x, p.y );
        p.x=p.x+(dt*vel.x);
        p.y=p.y+(dt*vel.y);
      };
    };
    if (coloring == bydist){
      while ((p.x >= xmin)&&(p.x <= xmax)&&(p.y >= ymin)&&(p.y <= ymax)){
        p.z=eq3DF( p.x, p.y );
        findCdist(p);
        inFront(p);
        vel=gradientF3D( p.x, p.y );
        p.x=p.x+(dt*vel.x);
        p.y=p.y+(dt*vel.y);
      };
    };
    if (coloring == colconst){
      setColor(gridC);
      while ((p.x >= xmin)&&(p.x <= xmax)&&(p.y >= ymin)&&(p.y <= ymax)){
        p.z=eq3DF( p.x, p.y );
        pl=p;
        vel=gradientF3D( p.x, p.y );
        p.x=p.x+(dt*vel.x);
        p.y=p.y+(dt*vel.y);
        line3D( p, pl );
      };
    };
    p=pb;
    if (coloring == bymag){
      while ((p.x >= xmin)&&(p.x <= xmax)&&(p.y >= ymin)&&(p.y <= ymax)){
        p.z=eq3DF( p.x, p.y );
        findC(p.z);
        inFront(p);
        vel=gradientF3D( p.x, p.y );
        p.x=p.x-(dt*vel.x);
        p.y=p.y-(dt*vel.y);
      };
    };
    if (coloring == bydist){
      while ((p.x >= xmin)&&(p.x <= xmax)&&(p.y >= ymin)&&(p.y <= ymax)){
        p.z=eq3DF( p.x, p.y );
        findCdist(p);
        inFront(p);
        vel=gradientF3D( p.x, p.y );
        p.x=p.x-(dt*vel.x);
        p.y=p.y-(dt*vel.y);
      };
    };
    if (coloring == colconst){
      setColor(gridC);
      while ((p.x >= xmin)&&(p.x <= xmax)&&(p.y >= ymin)&&(p.y <= ymax)){
        p.z=eq3DF( p.x, p.y );
        pl=p;
        vel=gradientF3D( p.x, p.y );
        p.x=p.x-(dt*vel.x);
        p.y=p.y-(dt*vel.y);
        line3D( p, pl );
      };
    };
  };
}

void graphF3D(){
  resetAll();
  clear();
  comment( "Grid graphing in progress. " );
  //printLine( "  description of constants ..etc
  if (drawmode == gridgraph) gridGraph();
  if (drawmode == dotgraph) dotGraph();
  if (drawmode == gradis) gradientsF3D();
  if (drawmode == thick) thickGrid();
}

void contourF3D(){
  int lev, levl;
  double jj;
  point2 pin;
  resetWindow();
  rrange2D();
  realC=inputNumber("Color constant = " );
  realD=inputNumber("Distance between elevation lines = " );
  double xmint=inputNumber( "x - minimum = " ); newLine();
  double xmaxt=inputNumber( "x - maximum = " ); newLine();
  double ymint=inputNumber( "y - minimum = " ); newLine();
  double ymaxt=inputNumber( "y - maximum = " ); newLine();
  double xinct=(xmaxt-xmint)/xpix;
  double yinct=(ymaxt-ymint)/ypix;
  writeStopM();
  clear();
  comment("Drawing contour graph." );
  for (i=0; i<=xpix; i++){
    pin.x=xmint+(xinct*(i+.5));
    pin.y=ymint;
    lev=brack(eq3DF(pin.x, pin.y)/realD);
    if (checkStop()) i=xpix+1;
    for (jj=.25; jj <= ypix; jj=jj+.5){
      pin.y=ymaxt-(yinct*jj);
      levl=lev;
      lev=brack(eq3DF(pin.x, pin.y)/realD);     
      if (lev != levl){
        setColor(fabs(max(lev, levl)*realC));
        drawPoint(w, i, jj);
        drawPoint(pix, i, jj);
      };
    };
  };
  roam( xmint, xmaxt, ymint, ymaxt, "x", "y" ); 
  redraw();
}

void intergraph(){
  int box;
  point2 pin;
  int signl, sign, pinyl;
  int askrange=1;
  double iconst, jj;
  char yn, yn2 = 'y';
  char xyz;
  while (yn2 == 'y'){
    yn2 = 'n';
    yn = 'n';
    clear();
    setColor(interC);
    yn='n';
    printLine( "Find the intercept with a projective plane (y/n)? " );
    clearMenu();
    writeYesNo();
    getChoiceM( general );
    if (chosen( yesB )) yn='y';
    xyz = 'n';
    if (yn == 'y'){
      xyz=inputChar( "Is x, y, or z constant (x/y/z)? " ); newLine();
      iconst=inputNumber( "And what is the value of the constant? "); newLine();
    };
    if (xyz == 'x'){
      ymint=inputNumber( "y - minimum = " ); newLine();
      ymaxt=inputNumber( "y - maximum = " ); newLine();
      zmint=inputNumber( "z - minimum = " ); newLine();
      zmaxt=inputNumber( "z - maximum = " ); newLine();
      yinct=(ymaxt-ymint)/xpix;
      zinct=(zmaxt-zmint)/ypix;
      xt=iconst;
      for (i = 0; i<xpix; i++){
        yt=ymint+((i+.5)*yinct);
        pinyl=pin.y;
        pin.y=((zmaxt-eq3DF( xt, yt ))/zinct)-.5;
        if (i>0){
          drawLine(w, i, pin.y, i-1, pinyl);
          drawLine(pix, i, pin.y, i-1, pinyl);
        };
      };      
    };
    if (xyz == 'y'){
      xmint=inputNumber( "x - minimum = " ); newLine();
      xmaxt=inputNumber( "x - maximum = " ); newLine();
      zmint=inputNumber( "z - minimum = " ); newLine();
      zmaxt=inputNumber( "z - maximum = " ); newLine();
      xinct=(xmaxt-xmint)/xpix;
      zinct=(zmaxt-zmint)/ypix;
      yt=iconst;
      for (i = 0; i<xpix; i++){
        xt=xmint+((i+.5)*xinct);
        pinyl=pin.y;
        pin.y=((zmaxt-eq3DF( xt, yt ))/zinct)-.5;
        if (i>0){
          drawLine(w, i, pin.y, i-1, pinyl);
          drawLine(pix, i, pin.y, i-1, pinyl);
        };
      };
    };
    if (xyz == 'z'){
      xmint=inputNumber( "x - minimum = " ); newLine();
      xmaxt=inputNumber( "x - maximum = " ); newLine();
      ymint=inputNumber( "y - minimum = " ); newLine();
      ymaxt=inputNumber( "y - maximum = " ); newLine();
      xinct=(xmaxt-xmint)/xpix;
      yinct=(ymaxt-ymint)/ypix;
      writeStopM();
      comment("Drawing intercept graph." );
      for (i=0; i<=xpix; i++){
        pin.x=xmint+(xinct*(i+.5));
        if (eq3DF(pin.x, ymax) == iconst) sign=0;
          else sign=(fabs(eq3DF(pin.x, ymax)-iconst))/(eq3DF(pin.x, ymax));
        if (checkStop()) i=xpix+1;
        for (jj=.25; jj <= ypix; jj=jj+.5){
          pin.y=ymaxt-(yinct*jj);
          signl=sign;
          if(eq3DF( pin.x, pin.y) == iconst) sign=0;
            else sign=(fabs(eq3DF(pin.x,pin.y)-iconst))/(eq3DF( pin.x, pin.y)-iconst);
          if (sign*signl <= 0){
            drawPoint(w, i, jj);
            drawPoint(pix, i, jj);
          };
        };
      };
      roam( xmint, xmaxt, ymint, ymaxt, "x", "y" );
    };
    if (yn != 'y'){
                        //general plane
    };
    yn2='n';
    printLine( "Calculate another intercept ?" );
    clearMenu();
    writeYesNo();
    getChoiceM( general );
    if (chosen( yesB )) yn2='y';
  };
}

void derivative(){
  double dzdx,dzdxl,dzdy,dzdyl,dxtl,dytl;
  char yn='y';
  while (yn == 'y'){
    resetWindow();
    comment( "See page. " );
    printLine( "Enter the coordinates of a point. " );
    xt=inputNumber( "x = " ); newLine();
    yt=inputNumber( "y = " ); newLine();
    dxt=1;
    dzdxl=0; dzdx=.12345;
    while (dzdxl != dzdx){
      dzdxl=dzdx;
      dzdx=(eq3DF(xt+dxt, yt)-eq3DF(xt, yt))/dxt;
      dxtl=dxt;
      dxt=dxt/2;
      if (dxt == dxtl) dzdx=dzdxl;
    };
    printString( "dz/dx = " ); printNumber( dzdx ); newLine();
    dyt=1;
    dzdyl=0; dzdy=.12345;
    while (dzdyl != dzdy){
      dzdyl=dzdy;
      dzdy=(eq3DF(xt, yt+dyt)-eq3DF(xt, yt))/dyt;
      dytl=dyt;
      dyt=dyt/2;
      if (dyt == dytl) dzdy=dzdyl;
    };
    printString( "dz/dy = "); printNumber( dzdy ); newLine();
    yn='n';
    if ( getYesNo( "Find another derivative value ?" )) yn = 'y';
  };
}

void integral(){
  int intdiv, dim23;
  double intvol=0, intarea=0, valconst;
  double xsct,ysct;
  char yn='y';
  char xyconst;
  while (yn == 'y'){
    resetWindow();
    dim23=inputNumber( "2 - or 3 - dimensional integral? " ); newLine();
    if (dim23 == 2){
      intarea=0;
      xyconst=inputChar( "With x or y constant? " ); newLine();
      valconst=inputNumber( "And what is the value of the constant? " );
      newLine();
      if (xyconst == 'x'){
        ymint=inputNumber( "y - minimum = " );
        ymaxt=inputNumber( "y - maximum = " );
        ysct=(ymaxt-ymint)/integdet;
        for (i=1; i <= integdet; i++){
          yt=ymint+(i*ysct);
          intarea=intarea+(ysct*yt);
        };
      };
      if (xyconst == 'y'){
        xmint=inputNumber( "x - minimum = " ); newLine();
        xmaxt=inputNumber( "x - maximum = " ); newLine();
        xsct=(xmaxt-xmint)/integdet;
        for (i=1; i <= integdet; i++){
          xt=xmint+(i*xsct);
          intarea=intarea+(xsct*xt);
        };
      };
      if ((xyconst == 'x')||(xyconst == 'y')){
        printString( "The area under the curve is " );
        printNumber( intarea ); newLine();
        yn='n';
        if (getYesNo( "Find another integral ?" )) yn='y';
      };
    };
    if (dim23 == 3){
    xmint=inputNumber( "x - minimum = " ); newLine();
    xmaxt=inputNumber( "x - maximum = " ); newLine();
    ymint=inputNumber( "y - minimum = " ); newLine();
    ymaxt=inputNumber( "y - maximum = " ); newLine();
    intdiv=inputNumber( "How many divisions do you want to use? " ); newLine();
    xsct=(xmaxt-xmint)/intdiv;
    ysct=(ymaxt-ymint)/intdiv;
    intvol=0;
    for (i=1; i<=intdiv; i++){
      xt=xmint+(i*xsct);
      for (j=1; j<=intdiv; j++){
        yt=ymint+(i*ysct);
        intvol=intvol+(xsct * ysct * eq3DF(xt, yt));
      };
    };
    printString( "The volume under the curve is " );
    printNumber( intvol ); newLine();
    yn='n';
    if (getYesNo( "Find another integral (y/n)? " )) yn='y';
    };
  };
}

void lengthF3D(){
  resetWindow();
  point2 p1, p2;
  printLine( "First point:" );
  p1.x=inputNumber( "x = " );
  p1.y=inputNumber( "y = " );
  printLine( "Second point:" );
  p2.x=inputNumber( "x = " );
  p2.y=inputNumber( "y = " );
  if ((p1.x==p2.x)&&(p1.y==p2.y)){
    printLine( "The length of the curve is 0 (as you'd expect)." );
    return;
  };
  intA=inputNumber( "How many divisions do you want to use?" );
  double tincx=(p2.x-p1.x)/(intA*1.0);
  double tincy=(p2.y-p1.y)/(intA*1.0);
  point3 p, pl;
  p.x=p1.x;
  p.y=p1.y;
  p.z=eq3DF(p1.x, p1.y);
  realC=0;
  for (i=1; i <= intA; i++){
    pl=p;
    p.x=p1.x+(tincx*i);
    p.y=p1.y+(tincy*i);
    p.z=eq3DF(p.x, p.y);
    realC=realC+dist3D(p, pl);
  };
  newLine();
  printString("The length of the curve is "); printNumber(realC); newLine();
  waitGeneral();
}

void surfaceF3D(){
  resetWindow();
  realA=inputNumber( "lower x - value = " );
  realB=inputNumber( "upper x - value = " );
  double realyA=inputNumber( "lower y - value = " );
  double realyB=inputNumber( "upper y - value = " );
  if (realB < realA){
    realC=realA;
    realA=realB;
    realB=realC;
  };
  if (realyB < realyA){
    realC=realyA;
    realyA=realyB;
    realyB=realC;
  };  
  if (realA == realB){
    printLine( "The area of the curve is 0 (as you'd expect)." );
    return;
  };
  if (realyA == realyB){
    printLine( "The area of the curve is 0 (as you'd expect)." );
    return;
  };
  intA=inputNumber( "How many divisions do you want to use?" );
  double tincx=(realB-realA)/(intA*1.0);
  double tincy=(realyB-realyA)/(intA*1.0);
  point3 p, pl, pll;
  realC=0;
  for (i=0; i < intA; i++){
    pl.x=realA;
    pl.y=realyA+(tincy*i);
    pl.z=eq3DF(pl.x, pl.y);
    pll.x=realA+tincx;
    pll.y=realyA+(tincy*(i+1));
    pll.z=eq3DF(pll.x, pll.y);
    p.y=pl.y;
    for (j=0; j < intA; j++){
      p.x=xmin+(tincx*(j+1));
      p.z=eq3DF(p.x, p.y);
      realC=realC+(dist3D(p, pl)*dist3D(pl, pll));
      pl=p;
      pll.x=p.x;
      pll.z=eq3DF(pll.x, pll.y);
    };
  };
  newLine();
  printString("The area of the curve is "); printNumber(realC); newLine();
  waitGeneral();
}

void analysis(){
  while (1==1){
    setMenu(general);
    resetAll();
    setColor( textC );
    putMenu( planecutB, "plane cut" );
    putMenu( derivativeB, "derivative" );
    putMenu( integralB, "integral" );
    putMenu( areaB, "Surface area" );
    putMenu( lengB, "Curve length" );
    getChoiceM( general );
    if (chosen( planecutB ) ) intergraph();
    if (chosen( derivativeB ) ) derivative();
    if (chosen( integralB ) ) integral();
    if (chosen( areaB ) ) surfaceF3D();
    if (chosen( lengB ) ) lengthF3D();
    if (backM() ) return;
    if (quitM() ) exit(1);
  };
}

/*void parproj(){
  constantsF3D(); axes(); gridGraph(); 
}

void cenproj(){
  system("clear");
  printLine( "Enter the coordinates of your viewing point (x,y,z). " );
  xcen=inputNumber( "x = " ); newLine();
  ycen=inputNumber( "y = " ); newLine();
  zcen=inputNumber( "z = " ); newLine();
  switch (drawmode){
    case 1: constantsF3D(); axes(); gridGraph(); clear(); break;
    default: comment( "Invalid drawing mode (cenproj())" );
  };
}*/

void getModeF3D(){
  int box=-2;
  setMenu(general);
  clearMenu();
  setColor( textC );
  putMenu( parallelB, "parallel" );
  putMenu( centralB, "central" );
  while (box < -1){
    box=getChoice( general );
    if (menuNumber( centralB ) == box) sightmode = central;
    if (menuNumber( parallelB ) == box) sightmode = parallel;
    if (menuNumber(quitB) == box) exit(1);
  };
  box=-2;
  clearMenu();
  setColor( textC );
  putMenu( dotsB, "point graph" );
  putMenu( gridB, "grid graph" );
  putMenu( gradientsB, "gradient map" );
  putMenu( thickB, "fine grid" );
  while (box < -1){
    box=getChoice( general );
    if (menuNumber( gridB ) == box) drawmode = gridgraph;
    if (menuNumber( dotsB ) == box) drawmode = dotgraph;
    if (menuNumber( gradientsB ) == box) drawmode = gradis;
    if (menuNumber( thickB ) == box) drawmode = thick;
    if (menuNumber(quitB) == box) exit(1);
  };
}

void mapGraph(){
  for (i=1; i<mapdet; i++){
    for (j=1; j<mapdet; j++){
      setColor( grid[i][j].col );
      line3D(grid[i][j].p, grid[i][j-1].p);
      line3D(grid[i][j].p, grid[i-1][j].p);
    };
  };
  for (i=1; i< mapdet; i++){
    setColor( grid[i][0].col );
    line3D(grid[i][0].p, grid[i-1][0].p);
  };
  for (j=1; j<mapdet; j++){
    setColor( grid[0][j].col );
    line3D(grid[0][j].p, grid[0][j-1].p);
  };
}

void rotationF3D(){
  int box;
  point3 pap1, pap2;
  Event e=nextEvent();
  if (e.type == Expose) redraw();
  zmin=zmid-((xspan+yspan)/4);
  zmax=zmid+((xspan+yspan)/4);
  makeGrid();
  writeRotControls();
  while ( (e.type != ButtonPress) || (e.value != 3) ){
    if ( ( e.type == ButtonPress ) && ( e.value == 1 ) ){
      box=areaName( rotcontrols, e.x, e.y );
      if ( box == thbM ) theta.y=theta.y-angleinc;
      if ( box == thbP ) theta.y=theta.y+angleinc;
      if ( box == thaM ) theta.x=theta.x-angleinc;
      if ( box == thaP ) theta.x=theta.x+angleinc;
      if ( box == lconstM ) lconst=lconst/lenginc;
      if ( box == lconstP ) lconst=lconst*lenginc;
      parConsts();
      clear();
      pap1.x=xmid; pap2.x=xmid; pap1.y=ymid;pap2.y=ymid;pap1.z=zmid;pap2.z=zmid;
      pap1.x=xmin; pap2.x=xmax;
      setColor( axisC.x );
      line3D(pap1, pap2);
      setColor( backC-1 );
      dot3D(pap2);
      pap1.x=xmid; pap2.x=xmid; pap1.y=ymin; pap2.y=ymax;
      setColor( axisC.y );
      line3D(pap1, pap2);
      setColor( backC-1 );
      dot3D(pap2);
      pap1.y=ymid; pap2.y=ymid; pap1.z=zmin; pap2.z=zmax;
      setColor( axisC.z );
      line3D(pap1, pap2);
      setColor( backC-1 );
      dot3D(pap2);
      mapGraph(); 
    };
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
  drawPanel();
}
  
 void traceF3D(){
  int crossC=40;
  int tracedivs=50; //divisions for the trace graph. 
  resetWindow();
  comment( "Initializing trace window" );
  saveLimits();
  setLimits( xpix, wide, 0, lheight*6 );
  double trxsc=xspan/(tracedivs*1.0);
  double trysc=yspan/(tracedivs*1.0);
  double ii, jj;
  int txcor=0, tycor=0;
  int sqlength=1.1*pagelength/(tracedivs*1.0);
  double sqlenr=pagelength/(tracedivs*1.0);
  for (ii=xmin+(trxsc/2); ii < xmax; ii=ii+trxsc ){
    tycor=0;
    for (jj=ymax-(trysc/2); jj > ymin; jj=jj-trysc ){
      Z=eq3DF( ii, jj );
      findC( Z );
      fillRectangle(w, xpix+(txcor*sqlenr), tall-pagelength+(tycor*sqlenr), sqlength, sqlength);
      fillRectangle(pix, xpix+txcor*sqlenr, tall-pagelength+(tycor*sqlenr), sqlength, sqlength);
      tycor++;
    };
    txcor++;
  };
  Event e;
  updateEvents();
  comment( "Coordinates displayed on page.  Right button to quit. " );
  point3 place;
  pixel cross;
  cross.x=xpix;
  cross.y=ypix;
  while ((e.type != ButtonPress)||(e.value != 3)){
    if (checkEvent()){
      e=nextEvent();
      if (e.type == Expose) redraw();
      if (e.type == MotionNotify){
        place.x=xmin+(xspan*(e.x-xpix)/pagelength);
        place.y=ymin+(yspan*(tall-e.y)/pagelength);
        place.z=eq3DF( place.x, place.y );
        resetWindow();
        printString( "x = " ); printNumber( place.x ); newLine();
        printString( "y = " ); printNumber( place.y ); newLine();
        printString( "z = " ); printNumber( place.z ); newLine();
        copyArea(pix, w,cross.x-10, cross.y-10, 20, 20, cross.x-10, cross.y-10);
        cross=proj( place );
        setColor( crossC );
        drawLine( w, cross.x-9, cross.y, cross.x+9, cross.y );
        drawLine( w, cross.x, cross.y-9, cross.x, cross.y+9 );
      };
    };
  };  
  resetLimits();
  resetWindow();
  clearMenu();
  redraw();
}

void constantsF3D(){  
  resetWindow();
  if (drawmode == gridgraph){
    griddet=inputNumber( "Detail for grid (# divisions) = " ); newLine();
    xsc=xspan/griddet;
    ysc=yspan/griddet;
  };
  if (drawmode == thick){
    xsc=xspan/(inputNumber( "# grid lines = " )*1.0);
    ysc=yspan/(inputNumber( "Detail for grid lines (# divisions) = " )*1.0);
    newLine();
  };
  if (drawmode == gradis){
    dt=inputNumber( "Time interval dt = " ); newLine();
  };
  comment( "Coloring... " );
  coloring=0;
  while (!coloring){
    setMenu(general);
    clearMenu();
    setColor( textC );
    putMenu( cconstB, "Constant");
    putMenu( magB, "by magnitude" );
    putMenu( distB, "by distance" );
    getChoiceM(general);
    if (chosen( magB )) coloring=bymag;
    if (chosen( cconstB )) coloring=colconst;
    if (chosen( distB )) coloring=bydist;
  };
  if ( coloring == colconst ) findGridC();
  else initCF3D();
}

void F3DMain(){
  int graphdrawn=0;
  gWcor.x=20; gWcor.y=20;
  srand48(backC);

  //assign menu positions (all coordinates not written are 0)
  setMenu(general);
  resetMenu();
  cconstB=addMenu(); magB=addMenu(); distB=addMenu(); centralB=addMenu();
  integralB=addMenu(); planecutB=addMenu(); areaB=addMenu(); lengB=addMenu();
  constF3DB=addMenu(); graphF3DB=addMenu(); rotF3DB=addMenu();
  traceF3DB=addMenu(); contourF3DB=addMenu(); dotsB=addMenu();
  gradientsB=addMenu(); thickB=addMenu();
  
  int firsttime=1; 
  while (1==1){
    setMenu(general);
    resetAll();
    setColor( textC );
    putMenu( analysisF3DB, "analysis" );
    putMenu( constF3DB, "constants" );
    putMenu( graphF3DB, "draw graph" );
    putMenu( rotF3DB, "rotate graph" );
    putMenu( traceF3DB, "trace" );
    putMenu( contourB, "contour graph" );
    getChoiceM( general );
    if ( chosen( analysisF3DB) ) analysis();
    if ( chosen( constF3DB ) ){
      getModeF3D(); range3D(); constantsF3D(); };
    if ( chosen( graphF3DB ) ){
      if (firsttime){
         getModeF3D();
         range3D(); 
         realA=xspan;
         if (yspan > realA) realA=yspan;
         realA=realA*1.2/2;
         lconst=((xpix+ypix)/4)/realA;
         constantsF3D();
         firsttime=0; axes();
      };
      graphF3D();
    };
    if ( chosen( contourB ) ) contourF3D();
    if ( chosen( rotF3DB ) ) rotationF3D();
    if ( chosen( traceF3DB ) ) traceF3D();
    if ( backM() ) return;
    if ( quitM() ) exit(0);
  };
}
