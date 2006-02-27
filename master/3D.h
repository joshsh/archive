int startatx, startaty;  //for 3-D graphing, the direction at the 'back'
int3 axisC;

point2 theta;
point3 ref; //point for direction of parallel projection

double lconst;
double r;  //length of reference vector for parallel projection
double pc1, pc2, pc3, pc4, pc5, pc6, pc7, pc8;
double angleinc=pi/20; //angle - change increment for axes positioning
double lenginc=1.1;  //length - change increment "     "
double xax, xay, yax, yay, yaz;
double minz, maxz; //minimum and maximum z-values for elevation graphing
double distCconst1, distCconst2;
double cconst3D1, cconst3D2;

//3D functions=============================================================

point3 halveV3(point3 p1){
  p1.x=p1.x/2;
  p1.y=p1.y/2;
  p1.z=p1.z/2;
  return p1;
}

//graphing functions=======================================================

void parConsts(){         //constants for the parallel projection.
  pc1=cos(theta.x);
  pc2=cos(theta.y);
  pc3=sin(theta.y);
  pc4=cos(theta.y+(pi/2));
  pc5=sin(theta.y+(pi/2));
  pc6=sin(theta.x);
  pc7=pc3*pc6;
  pc8=pc5*pc6;
  ref.x=cos(theta.y) * cos(theta.x); //ref.x=cos(-theta.x)*cos(theta.y);
  ref.y=cos(theta.y+(pi/2)) * cos(theta.x); //ref.y=cos(theta.x)*sin(-theta.y);
  ref.z=cos(theta.x); //ref.z=sin(-theta.x);
  r=(ref.x*ref.x)+(ref.y*ref.y)+(ref.z*ref.z);
}

/*                     ^                               -x
               ^       |       ^                       ^
              /    -theta.x    \                       |
    -theta.y |                  | +theta.y       -y <-----> +y
              \    +theta.x    /                       |
                       |                               v
                       v                               +x
*/

pixel proj(point3 ppj){ 
  pixel pj;
  yaz=(ppj.z-zmid)*pc1;
  xay=(ppj.y-ymid)*pc2;
  yay=(ppj.y-ymid)*pc7;
  xax=-(ppj.x-xmid)*pc4;
  yax=-(ppj.x-xmid)*pc8;
  pj.x=int((xax+xay)*lconst) + xs0;
  pj.y=ys0 - int((yay+yaz+yax)*lconst);
  return pj;
} 

void line3D(point3 lp1, point3 lp2){
  pixel lp3 = proj(lp1);
  pixel lp4 = proj(lp2);
  drawLine(win, lp3.x, lp3.y, lp4.x, lp4.y );
  drawLine(pix, lp3.x, lp3.y, lp4.x, lp4.y );
}

void dot3D(point3 pp1){
  pixel pp2 = proj(pp1);
  drawPoint(win, pp2.x, pp2.y);
  drawPoint(pix, pp2.x, pp2.y);
}

void line3DW(point3 lp1, point3 lp2){
  pixel lp3 = proj(lp1);
  pixel lp4 = proj(lp2);
  drawLine(win, lp3.x, lp3.y, lp4.x, lp4.y );
}

void dot3DW(point3 pp1){
  pixel pp2 = proj(pp1);
  drawPoint(win, pp2.x, pp2.y);
}

void inFront( point3 if1 ){
  double if2=((ref.x*(if1.x-xmid))+(ref.y*(if1.y-ymid))+(ref.z*(if1.z-zmid)))/r;
  pixel if4=proj(if1);
  if (if4.x >= xpix) return;
  if (if4.y >= ypix) return;
  if (if4.x < 0) return;
  if (if4.y < 0) return;
  if ((field[if4.x][if4.y] < if2)||(field[if4.x][if4.y]==0)){
    field[if4.x][if4.y]=if2;
    drawPoint(win, if4.x, if4.y);
    drawPoint(pix, if4.x, if4.y);
  };
}

void inFrontW( point3 if1 ){
  double if2=((ref.x*(if1.x-xmid))+(ref.y*(if1.y-ymid))+(ref.z*(if1.z-zmid)))/r;
  pixel if4=proj(if1);
  if (if4.x >= xpix) return;
  if (if4.y >= ypix) return;
  if (if4.x < 0) return;
  if (if4.y < 0) return;
  if ((field[if4.x][if4.y] < if2)||(field[if4.x][if4.y]==0)){
    field[if4.x][if4.y]=if2;
    drawPoint(win, if4.x, if4.y);
  };
}

void initC( double minz, double maxz){
  cconst3D1=lowC+((colspan3D*maxz)/(maxz-minz));
  cconst3D2=-colspan3D/(maxz-minz);
}

void findC(double height){
  //int foundC=highC-((height-minz)*cconst3D);
  //if (height<minz) foundC=highC;
  //if (height>maxz) foundC=lowC;
  int foundC=int(cconst3D1+(cconst3D2*height));
  if (foundC > highC) foundC = highC;
  if (foundC < lowC) foundC = lowC;
  setColor( foundC );
}

int returnC( double height ){
  int foundC=int(cconst3D1+(cconst3D2*height));
  if (foundC > highC) foundC = highC;
  if (foundC < lowC) foundC = lowC;
  return foundC;
}

void findCdist( point3 fd1 ){
  double thisdist=sq(fd1.x-xmid)+sq(fd1.y-ymid)+sq(fd1.z-zmid);
  int foundC = int(distCconst1 + (thisdist*distCconst2));
  if (foundC > highC) foundC = highC;
  if (foundC < lowC) foundC = lowC;
  setColor( foundC );
}

int returnCdist( point3 fd1 ){
  double thisdist=sq(fd1.x-xmid)+sq(fd1.y-ymid)+sq(fd1.z-zmid);
  int foundC = int(distCconst1 + (thisdist*distCconst2));
  if (foundC > highC) foundC = highC;
  if (foundC < lowC) foundC = lowC;
  return foundC;
}

void graphLine(point3 gl1, point3 gl2){  //graphs a line in the proj. field.
  pixel gl3=proj(gl1);
  pixel gl4=proj(gl2);
  realA=max(fabs(gl3.x-gl4.x), fabs(gl3.y-gl4.y));
  double gintx=(gl2.x-gl1.x)/realA;
  double ginty=(gl2.y-gl1.y)/realA;
  double gintz=(gl2.z-gl1.z)/realA;
  point3 gl5;
  if (coloring == bymag){
    for (i=0; i<=realA; i++){
      gl5.x=gl1.x+(i*gintx);
      gl5.y=gl1.y+(i*ginty);
      gl5.z=gl1.z+(i*gintz);
      findC(gl5.z);
      inFront(gl5);
    }; 
  };
  if (coloring == bydist){
    for (i=0; i<=realA; i++){
      gl5.x=gl1.x+(i*gintx);
      gl5.y=gl1.y+(i*ginty);
      gl5.z=gl1.z+(i*gintz);
      findCdist(gl5);
      inFront(gl5);
    }; 
  };
  if (coloring == colconst){
    for (i=0; i<=realA; i++){
      gl5.x=gl1.x+(i*gintx);
      gl5.y=gl1.y+(i*ginty);
      gl5.z=gl1.z+(i*gintz);
      inFront(gl5);
    };    
  };
}

void graphRect(point3 gl1, point3 gl2, point3 gl6){  
  //graphs a rectangle in the p.f. Points may not be listed diagonally!
  pixel gl3=proj(gl1);
  pixel gl4=proj(gl2);
  realA=max(fabs(gl3.x-gl4.x), fabs(gl3.y-gl4.y));
  double gintx=(gl2.x-gl1.x)/realA;
  double ginty=(gl2.y-gl1.y)/realA;
  double gintz=(gl2.z-gl1.z)/realA;
  double gintx2=gl6.x-gl1.x;
  double ginty2=gl6.y-gl1.y;
  double gintz2=gl6.z-gl1.z; 
  //point end1, end2;
  point3 gl5, gl7;
  for (i=0; i<=realA; i++){
      gl5.x=gl1.x+(i*gintx);
      gl5.y=gl1.y+(i*ginty);
      gl5.z=gl1.z+(i*gintz);
      gl7.x=gl5.x+gintx2;
      gl7.y=gl5.y+ginty2;
      gl7.z=gl5.z+gintz2;
      graphLine(gl5, gl7);
  }; 
}

/*void graphCircle(point3 cent, point3 dir, double crad, int colortype){
  if (colortype == colconst){
    
  };
  if (colortype == bymag){

  };
  if (colortype == bydist){

  };  
}*/

void rotControls(){
  //this is not completely general...
  int aa=ypix, bb=pmenuheight, cc=pmenuheight/2, dd=pmenuheight/3;
  rotcontrols=startGroup();
  thbP = addList(0,aa,dd,bb);
  thaP = addList(dd,aa,dd,cc);
  thaM = addList(dd,aa+cc,dd,cc);
  thbM = addList(dd*2,aa,dd,bb);
  lconstP = addList(dd*3,aa,dd,cc);
  lconstM = addList(dd*3,aa+cc,dd,cc);
}

void writeRotControls(){
  double aa=commentheight, bb=pmenuheight/150.0, cc=pmenuheight/6;          
  double dd=pmenuheight/2, ee=pmenuheight/6;
  emptyPanel();
  setColor( partC );
  //panel=drawAreas( panel, rotcontrols, 0, -ypix);
  drawAreas( pix, rotcontrols, 0, 0 );
  setColor(objectC);
  drawCircle(pix, int(75*bb), int(aa+(75*bb))+ypix, int(50*bb));
  drawEllipse(pix, int(75*bb), int(aa+(75*bb)+ypix), int(10*bb), int(50*bb));
  arrow(pix, int(ee), int(aa+dd-5+ypix), 10, up);
  arrow(pix, int(ee), int(aa+dd+ee+ypix), 10, down);
  arrow(pix, int(ee*3+5), int(aa+dd-ee+ypix), 10, up);
  arrow(pix, int(ee*3+5), int(aa+dd+ee+ypix), 10, down);
  arrow(pix, int(ee*5), int(aa+dd-5+ypix), 10, up);
  setColor(textC);
  drawString( pix, int(10*bb), int(aa+(10*bb)+ypix), "Rotation controls" );
  drawPanel();
}

/*void traceArrows(){
  int aa=pmenucor.y, bb=pmenuheight, cc=pmenuheight, dd=pmenuheight*2/3;
  tracearrows=startGroup();
  xM = addList(xpix-cc, aa, dd/2, cc);
  yP = addList(xpix-dd, aa, dd/2, cc/2+1);
  yM = addList(xpix-dd, aa+(cc/2)+1, dd/2, cc/2);
  xP = addList(xpix-dd/2, aa, dd/2, cc);
}

void writeArrows(){
  double aa=commentheight, bb=pmenuheight/150.0, cc=pmenuheight/2;
  double dd=pmenuheight/3;
  emptyPanel();
  setColor( partC );
  //panel=drawAreas( panel, tracearrows, 0, -ypix);
  pix = drawAreas( pix, tracearrows, 0, 0 );
  setColor(textC);
  drawString(pix, xpix-cc+(7*bb), aa+(10*bb)+ypix, "Trace");
  setColor(objectC);
  drawString(pix, xpix-dd/2+(7*bb), aa+(40*bb)+ypix, "x+");
  drawString(pix, xpix-cc+(7*bb), aa+(40*bb)+ypix, "x-");
  drawString(pix, xpix-dd+(10*bb), aa+(20*bb)+ypix, "y+");
  drawString(pix, xpix-dd+(10*bb), aa+(60*bb)+ypix, "y-");
  drawPanel();
}*/

void parAxes( int resetvalues ){
  comment("Positon the axes with the rotation controls." );
  clear();
  int box;
  if (resetvalues){
    theta.x=0; theta.y=0;
    double topspan=xspan;
    if (yspan > topspan) topspan=yspan;
    topspan=topspan*1.2/2;
    lconst=((xpix+ypix)/4)/topspan;
  };                                   
  Event e=getNextEvent();
  zmin=zmid-((xspan+yspan)/4);
  zmax=zmid+((xspan+yspan)/4);
  point3 pap1, pap2;
  writeRotControls();
  while ( (e.type != ButtonPress) || (e.value != 3) ){
    if ( ( e.type == ButtonPress ) && ( e.value == 1 ) ){
      while( e.type != ButtonRelease){
        box = areaName( rotcontrols, e.x, e.y);
        if ( box == thaM ) theta.x = theta.x+angleinc;
        if ( box == thaP ) theta.x = theta.x-angleinc;
        if ( box == thbM ) theta.y = theta.y+angleinc;
        if ( box == thbP ) theta.y = theta.y-angleinc;
        if ( box == lconstM ) lconst=lconst/lenginc;
        if ( box == lconstP ) lconst=lconst*lenginc;
        parConsts();
        setColor( backC );
        fillRectangle( win, 0, 0, xpix, ypix );
      
        resetPage();
        printNumber(theta.x); newLine();
        printNumber(theta.y);

        //draw the axes.
        pap1.x=xmid; pap2.x=xmid;pap1.y=ymid;
        pap2.y=ymid;pap1.z=zmid;pap2.z=zmid;
        pap1.x=xmin; pap2.x=xmax;
        setColor( axisC.x );
        line3DW(pap1, pap2);
        setColor( backC-1 );
        dot3DW(pap2);
        pap1.x=xmid; pap2.x=xmid; pap1.y=ymin; pap2.y=ymax;
        setColor( axisC.y );
        line3DW(pap1, pap2);
        setColor( backC-1 );
        dot3DW(pap2);
        pap1.y=ymid; pap2.y=ymid; pap1.z=zmin; pap2.z=zmax;
        setColor( axisC.z );
        line3DW(pap1, pap2);
        setColor( backC-1 );
        dot3DW(pap2);  
        e=nextEvent();
        if (e.type == Expose) redraw();
      };   
    };
    e = nextEvent();
    if (e.type == Expose) redraw(); 
  };     
  theta.x=fixAngle(theta.x);
  theta.y=fixAngle(theta.y);

  //"startat" values
  if ((theta.x < pi/2)||(theta.x > pi*3/2)){
     if ((theta.y < pi/2)||(theta.y > pi*3/2)) startatx=-1;
     else startatx=1;
     if (theta.y < pi) startaty=1;
     else startaty=-1;
  }else{
     if ((theta.y < pi/2)||(theta.y > pi*3/2)) startatx=1;
     else startatx=-1;
     if (theta.y < pi) startaty=-1;
     else startaty=1;
  };

  //draw axes on pixmap and screen
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
  resetAll();
}

void graphAxes(){
  double axinc=xspan/50;
  point3 ax, ax2;
  ax.x=xmid; ax.y=ymid; ax.z=zmid;
  ax2=ax;
  ax2.x=xmid+axinc;
  while ( fabs( proj(ax).x - proj(ax2).x) > 1){
    axinc=axinc/2; ax2.x=xmid+axinc; };
  while ( fabs( proj(ax).y - proj(ax2).y ) > 1){
    axinc=axinc/2; ax2.x=xmid+axinc; };
  axinc=axinc/2;
  setColor(axisC.x);
  for (ax.x=xmid; ax.x <= xmax; ax.x=ax.x+axinc) inFront( ax );
  for (ax.x=xmid; ax.x >= xmin; ax.x=ax.x-axinc) inFront( ax );
  axinc=yspan/50;
  ax.x=xmid; ax.y=ymid; ax.z=zmid;
  ax2=ax;
  ax2.y=ymid+axinc;
  while ( fabs( proj(ax).x - proj(ax2).x) > 1){
    axinc=axinc/2; ax2.y=ymid+axinc; };
  while ( fabs( proj(ax).y - proj(ax2).y ) > 1){
    axinc=axinc/2; ax2.y=ymid+axinc; };
  axinc=axinc/2;
  setColor(axisC.y);
  for (ax.y=ymid; ax.y <= ymax; ax.y=ax.y+axinc) inFront( ax );
  for (ax.y=ymid; ax.y >= ymin; ax.y=ax.y-axinc) inFront( ax );
  axinc=(xspan+yspan)/100;
  ax.x=xmid; ax.y=ymid; ax.z=zmid;
  ax2=ax;
  ax2.z=zmid+axinc;
  while ( fabs( proj(ax).x - proj(ax2).x) > 1){
    axinc=axinc/2; ax2.z=zmid+axinc; };
  while ( fabs( proj(ax).y - proj(ax2).y ) > 1){
    axinc=axinc/2; ax2.z=zmid+axinc; };
  axinc=axinc/2;
  setColor(axisC.z);
  for (ax.z=zmid; ax.z <= zmax; ax.z=ax.z+axinc) inFront( ax );
  for (ax.z=zmid; ax.z >= zmin; ax.z=ax.z-axinc) inFront( ax );  
}
