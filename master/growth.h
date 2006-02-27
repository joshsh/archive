int maxitG;
int lowCG=30, highCG=95, colspanG=highCG-lowCG+1;
double rad, highrad=5;

int growthmode;
int DBMpoint=1, DBMline=2, collision=3;

//area constants
pixel DBMpB, DBMlB, collisionB;
pixel colorGB, constantsGB, drawGB, refreGB;

void dotG(pixel dg1, int iters){
  setColor( (iters*colspanG/maxitG)+lowCG );
  drawPoint(w, dg1.x, dg1.y); 
  drawPoint(pix, dg1.x, dg1.y);
}

int inbounds( pixel ib1){
  if (ib1.x <= 1) return 0;
  if (ib1.x >= xpix-1) return 0;
  if (ib1.y <= 1) return 0;
  if (ib1.y >= ypix-1) return 0;
  return 1;  
}

double distG(pixel dg1){
  return sqrt(((dg1.x-xsmid)*(dg1.x-xsmid))+((dg1.y-ysmid)*(dg1.y-ysmid)));
}

int check(pixel cp){
  if (setmatrix[cp.x][cp.y-1] != 0) return 1;
  if (setmatrix[cp.x][cp.y+1] != 0) return 1;
  if (setmatrix[cp.x-1][cp.y] != 0) return 1; 
  if (setmatrix[cp.x+1][cp.y] != 0) return 1;
  return 0;        
}

int check2(pixel cp){
  if (setmatrix[cp.x][cp.y-1] != 0) return 1;
  if (setmatrix[cp.x][cp.y+1] != 0) return 1;
  if (cp.x == 0){
    if (setmatrix[xpix-1][cp.y] != 0) return 1;
  }else {if (setmatrix[cp.x-1][cp.y] != 0) return 1;}; 
  if (cp.x==xpix-1){
    if (setmatrix[0][cp.y] != 0) return 1;
  }else {if (setmatrix[cp.x+1][cp.y] != 0) return 1;};
  return 0;        
}

void dDBMpoint(){
  int dir;
  double angle, vdir;
  pixel p;
  setmatrix[xsmid][ysmid]=1;
  int count=0;
  dot( mpixel( xsmid, ysmid ) , count );
  srand48(rad+maxitG);
  int cont1=1;
  int cont2=1,status=0;
  while (cont1==1){
    if (checkStop()) return;
    angle=drand48()*2*pi;
    p.x=(highrad*1.1*cos(angle))+xsmid;
    p.y=(highrad*1.1*sin(angle))+ysmid;
    cont2=1;
    while( cont2==1){
      dir=0;
      vdir=(drand48()*4);
      for (i=0; i <= vdir; i++) dir = dir+1;
      if ( dir==5 ) dir=4;
      switch (dir){
        case 1: p.y=p.y-1; break;
        case 2: p.y=p.y+1; break;
        case 3: p.x=p.x-1; break;
        case 4: p.x=p.x+1; break;
        default: comment( "Error in dDBMpoint" );
      };  
      status=0;
      if (inbounds( p )==0){status=1; cont2=0;};
      if (distG(p) > .95*xpix/2 ){status=1; cont2=0;};
      if( check(p) == 1){status=2; cont2=0;};
    };
    if (status==2){
      count++;
      setmatrix[p.x][p.y]=1;
      dotG(p, count);
      if (count >= maxitG) cont1=0;
      if ( distG(p) >= rad) cont1=0;
      if ( distG(p) >= highrad) highrad=distG(p);
    };
  };
}

void dDBMline(){
  int dir, ytop=ypix;
  double vdir;
  pixel p;
  for (i=0; i<xpix; i++) setmatrix[i][ypix-2]=1;
  int count=0;
  srand48(rad+maxitG);
  int cont1=1, cont2=1, status=0;
  while (cont1==1){
    if (checkStop()) return;
    p.x=xpix*drand48();
    p.y=ytop-20;
    cont2=1;
    while( cont2==1){
      dir=0;
      vdir=(drand48()*4);
      for (i=0; i <= vdir; i++) dir = dir+1;
      if ( dir==5 ) dir=4;
      switch (dir){
        case 1: p.y=p.y-1; break;
        case 2: p.y=p.y+1; break;
        case 3: p.x=p.x-1; break;
        case 4: p.x=p.x+1; break;
        default: comment( "Error in dDBMline" );
      };  
      status=0;
      if (p.x >= xpix) p.x=0;
      if (p.x < 0) p.x = xpix-1;
      if (p.y < ytop-50){status=1; cont2=0;};
      if( check2(p) ){status=2; cont2=0;};
    };
    if (status==2){
      count++;
      setmatrix[p.x][p.y]=1;
      dotG(p, count);
      if (count >= maxitG) cont1=0;
      if ( p.y <= ytop) ytop=p.y;
    };
  };
}

void dcollision(){
  double angle1, angle2;
  pixel p;
  point2 pr, vel;
  setmatrix[xsmid][ysmid]=1;
  int count=0;
  dotG( mpixel(xsmid, ysmid), count );
  srand48(rad+maxitG);
  int cont1=1;
  int cont2=1,status=0;
  while (cont1){ 
    if (checkStop()) return;
    angle1=drand48()*2*pi;
    pr.x=(highrad*1.1*cos(angle1))+xsmid;
    pr.y=(highrad*1.1*sin(angle1))+ysmid;
    p.x=(highrad*1.1*cos(angle1))+xsmid;
    p.y=(highrad*1.1*sin(angle1))+ysmid;
    angle2=(pi/2)+angle1+(drand48()*pi);
    vel.x=1;
    vel.y=vel.x*tan(angle2);
    while (vel.y>1){
      vel.x=vel.x/2.0;
      vel.y=vel.x*tan(angle2);
    };
    while (vel.y<-1){
      vel.x=vel.x/2.0;
      vel.y=vel.x*tan(angle2);
    };   
    cont2=1;
    while( cont2==1){
      pr.x=pr.x+vel.x;
      pr.y=pr.y+vel.y;
      p.x=pr.x;
      p.y=pr.y;
      status=0;
      if (inbounds( p )==0){status=1; cont2=0;};
      if ( distG(p) > .95*xpix/2 ){status=1; cont2=0;};
      if( check(p) == 1){status=2; cont2=0;};
    };
    if (status==2){
      count++;
      setmatrix[p.x][p.y]=1;
      dotG(p, count);
      if (count >= maxitG) cont1=0;
      if ( distG(p) >= rad) cont1=0;
      if (distG(p) >= highrad) highrad=distG(p);
    };
  };
}

void clearGrid(){
  for (i=0; i < xpix; i++){
    for (j=0; j < ypix; j++) setmatrix[i][j]=0;
  };
}

void drawGrowth(){
  clear();
  clearGrid();
  writeStopM();
  comment( "Growth simulation in progress. " );
  if (growthmode==DBMpoint) dDBMpoint();
  if (growthmode==DBMline) dDBMline();
  if (growthmode==collision) dcollision();
  comment( "Graphing finished. " );
}

void growthColors(){
  lowCG=inputNumber( "Color for iteration #1 = " );
  highCG=inputNumber( "Color for the last iteration = " );
  newLine();
  colspanG=highCG-lowCG;
}

void growthConsts(){
  maxitG=inputNumber( "Maximum number of iterations = " ); 
  rad=(min(xpix, ypix)-2)/2;
}

void growthMain(){
  //assign menu areas (all coordinates not written are 0)
  DBMlB.x=1; collisionB.x=2;
  constantsGB.x=1; drawGB.x=2; refreGB.x=3;

  int back; 
  while(1==1){
    setMenu(general);
    resetAll();
    setColor( textC );
    putMenu( DBMpB, "DGM point" );
    putMenu( DBMlB, "DGM line" );
    putMenu2( collisionB, "`collision'",  "model" );
    getChoiceM( general );
    if (chosen( DBMpB )) growthmode=DBMpoint;
    if (chosen( DBMlB )) growthmode=DBMline;
    if (chosen( collisionB )) growthmode=collision;
    if (backM()) return;
    if (quitM()) exit(1);
    growthConsts();
    back=0;
    while(!back){
      setMenu(panelmenu);
      clearMenu();
      setColor( textC );
      putMenu( colorGB, "colors" );
      putMenu( constantsGB, "constants" );
      putMenu( drawGB, "draw simulation" );
      putMenu( refreGB, "refresh screen" );
      box=getChoice( panelmenu );
      if (chosen( colorGB )) growthColors();
      if (chosen( constantsGB )) growthConsts();
      if (chosen( drawGB )) drawGrowth();
      if (chosen( refreGB )) redraw();
      if (backM()) back=1;
      if (quitM()) exit(1);
    };
  };
}
