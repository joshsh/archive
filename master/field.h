//convert this to vectors! (using vectors.h)
//write programs for finding the divergence and curl.

int pathC = 40, path2C = 1;
int normalize;

double funct1(double fx, double fy){
  //return 1/sqrt(((fy*fy)/(fx*fx))+1);
  return 1/(sq((fx*fx)+(fy*fy)-1)+.5);
  //return (-1/(sq(fx)+sq(fy)))*fx/fy;
}

double funct2(double fx, double fy){
  //return 1/sqrt(1+((fx*fx)/(fy*fy)));
  return fx*fy+sin(fx);
  //return (-1/(sq(fx)+sq(fy)))*fy/fx;
}

double funct3D1(double fx, double fy, double fz){
  return sin(fx*fy) + fz;
}

double funct3D2(double fx, double fy, double fz){
  return cos(fz) + (fx*fy);
}

double funct3D3(double fx, double fy, double fz){
  return cos(fy) + (fx*fz);
}

void manualField(){
  double dx, dy;
  point2 po, pol, poll;
  Event e;
  resetAll();
  setColor( textC );
  double dt=inputNumber("Time interval dt = " );
  newLine();
  
  //initialize colors
  po.x=xmin+(xspan*drand48());
  po.y=ymin+(yspan*drand48());
  realA=sqrt(sq(dt*funct1(po.x, po.y))+sq(dt*funct2(po.x, po.y)));
  realB=realA;
  for (i=0; i < 200; i++){
    po.x=xmin+(xspan*drand48());
    po.y=ymin+(yspan*drand48());
    realC=sqrt(sq(dt*funct1(po.x, po.y))+sq(dt*funct2(po.x, po.y)));
    if (realC < realA) realA=realC;
    if (realC > realB) realB=realC;
  };
  initC(realA, realB);

  clear();
  writeStopM();
  comment( "Enter starting points with the mouse." );
  //setColor( pathC );
  while(1==1){
    if ((e.type == ButtonPress)&&(e.value == 1)){
      po=eventPoint(e);
      poll=po;
      if (normalize){
        printLine( "Error in chooseField(). " );
      }else{
        while (inWindow( po )){
          pol=po;
          dx=dt*funct1( po.x, po.y );
          dy=dt*funct2( po.x, po.y );
          po.x=po.x+dx;
          po.y=po.y+dy;
          findC(sqrt(sq(dx)+sq(dy)));
          line2D( po, pol );
          if (checkStop()) return;
        };
      };
      po=poll;
      if (normalize){
        printLine( "Error in chooseField(). " );
      }else{
        while (inWindow( po )){
          pol=po;
          dx=-dt*funct1( po.x, po.y );
          dy=-dt*funct2( po.x, po.y );
          po.x=po.x+dx;
          po.y=po.y+dy;
          findC(sqrt(sq(dx)+sq(dy)));
          line2D( po, pol );
          if (checkStop()) return;
        };
      };
    };
    if (checkStop()) return;
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
}

void randField(){
  double dx, dy;
  point2 po, pol, poll;
  resetAll();
  setColor( textC );
  double dt=inputNumber("Time interval dt = " );
  newLine();

  //initialize colors
  po.x=xmin+(xspan*drand48());
  po.y=ymin+(yspan*drand48());
  realA=sqrt(sq(dt*funct1(po.x, po.y))+sq(dt*funct2(po.x, po.y)));
  realB=realA;
  for (i=0; i < 200; i++){
    po.x=xmin+(xspan*drand48());
    po.y=ymin+(yspan*drand48());
    realC=sqrt(sq(dt*funct1(po.x, po.y))+sq(dt*funct2(po.x, po.y)));
    if (realC < realA) realA=realC;
    if (realC > realB) realB=realC;
  };
  initC(realA, realB);

  clear();
  writeStopM();
  comment( "Graphing field." );
  //setColor( pathC );
  while(1==1){
    po.x=(drand48()*xspan)+xmin;
    po.y=(drand48()*yspan)+ymin;
    poll=po;
    if (normalize){
      printLine( "Error in chooseField(). " );
    }else{
      while (inWindow( po )){
        pol=po;
        dx=dt*funct1( po.x, po.y );
        dy=dt*funct2( po.x, po.y );
        po.x=po.x+dx;
        po.y=po.y+dy;
        findC(sqrt(sq(dx)+sq(dy)));
        line2D( po, pol );
      };
    };
    if (checkStop()) return;
    po=poll;
    if (normalize){
      printLine( "Error in chooseField(). " );
    }else{
      while (inWindow( po )){
        pol=po;
        dx=-dt*funct1( po.x, po.y );
        dy=-dt*funct2( po.x, po.y );
        po.x=po.x+dx;
        po.y=po.y+dy;
        findC(sqrt(sq(dx)+sq(dy)));
        line2D( po, pol );
      };
    };
    if (checkStop()) return;
  };
}

void randField3D(){
  coloring=colconst;
  double dx, dy, dz;
  point3 po, pol, poll;
  resetAll();
  setColor( textC );
  double dt=inputNumber("Time interval dt = " );
  newLine();

  //initialize colors
  po.x=xmin+(xspan*drand48());
  po.y=ymin+(yspan*drand48());
  po.z=zmin+(zspan*drand48());
  realA=sqrt(sq(dt*funct3D1(po.x, po.y, po.z))+sq(dt*funct3D2(po.x, po.y, po.z))+sq(dt*funct3D3(po.x, po.y, po.z)));
  realB=realA;
  for (i=0; i < 200; i++){
    po.x=xmin+(xspan*drand48());
    po.y=ymin+(yspan*drand48());
    po.z=zmin+(zspan*drand48());
    realC=sqrt(sq(dt*funct3D1(po.x, po.y, po.z))+sq(dt*funct3D2(po.x, po.y, po.z))+sq(dt*funct3D3(po.x, po.y, po.z)));
    if (realC < realA) realA=realC;
    if (realC > realB) realB=realC;
  };
  initC(realA, realB);

  clear();
  clearField();
  graphAxes();
  writeStopM();
  comment( "Graphing field." );
  //setColor( pathC );
  while(1==1){
    po.x=(drand48()*xspan)+xmin;
    po.y=(drand48()*yspan)+ymin;
    po.z=(drand48()*zspan)+zmin;
    poll=po;
    if (normalize){
      printLine( "Error in chooseField(). " );
    }else{
      while (inWindow3D( po )){
        pol=po;
        dx=dt*funct3D1( po.x, po.y, po.z );
        dy=dt*funct3D2( po.x, po.y, po.z );
        dz=dt*funct3D3( po.x, po.y, po.z );
        po.x=po.x+dx;
        po.y=po.y+dy;
        po.z=po.z+dz;
        findC(sqrt(sq(dx)+sq(dy)+sq(dz)));
        graphLine( po, pol );
      };
    };
    if (checkStop()) return;
    po=poll;
    if (normalize){
      printLine( "Error in chooseField(). " );
    }else{
      while (inWindow3D( po )){
        pol=po;
        dx=-dt*funct3D1( po.x, po.y, po.z );
        dy=-dt*funct3D2( po.x, po.y, po.z );
        dz=-dt*funct3D3( po.x, po.y, po.z );
        po.x=po.x+dx;
        po.y=po.y+dy;
        po.z=po.z+dz;
        findC(sqrt(sq(dx)+sq(dy)+sq(dz)));
        graphLine( po, pol );
      };
    };
    if (checkStop()) return;
  };
}

void arrowField(){
  double dx, dy;
  point2 po, pol, po2;
  resetAll();
  setColor( textC );
  double dt=inputNumber("Time interval dt = " );
  newLine();
  clear();
  writeStopM();
  comment( "Graphing field. Red indicates the direction of the paths." );
  setColor( pathC );
  while(1==1){
    po.x=(drand48()*xspan)+xmin;
    po.y=(drand48()*yspan)+ymin;
    if (normalize){
      printLine( "Error in chooseField(). " );
    }else{
      pol=po;
      dx=dt*funct1( po.x, po.y );
      dy=dt*funct2( po.x, po.y );
      po.x=po.x+dx;
      po.y=po.y+dy;
      setColor( pathC );
      line2D( po, pol );
      po2.x=(po.x+pol.x)/2;
      po2.y=(po.y+pol.y)/2;
      setColor( path2C );
      line2D( po2, po );
    };
  if (checkStop()) return;
  };
}

void arrowField3D(){
  coloring=colconst;
  double dx, dy, dz;
  point3 po, pol, po2;
  resetAll();
  setColor( textC );
  double dt=inputNumber("Time interval dt = " );
  newLine();
  clearField();
  clear();
  writeStopM();
  comment( "Graphing field. Red indicates the direction of the paths." );
  setColor( pathC );
  while(1==1){
    po.x=(drand48()*xspan)+xmin;
    po.y=(drand48()*yspan)+ymin;
    po.z=(drand48()*zspan)+zmin;
    if (normalize){
      printLine( "Error in chooseField(). " );
    }else{
      pol=po;
      dx=dt*funct3D1( po.x, po.y, po.z );
      dy=dt*funct3D2( po.x, po.y, po.z );
      dx=dt*funct3D3( po.x, po.y, po.z );
      po.x=po.x+dx;
      po.y=po.y+dy;
      po.z=po.z+dz;
      setColor( pathC );
      graphLine( po, pol );
      po2.x=(po.x+pol.x)/2;
      po2.y=(po.y+pol.y)/2;
      po2.z=(po.z+pol.z)/2;
      setColor( path2C );
      graphLine( po2, po );
    };
  if (checkStop()) return;
  };
}

void fieldSection(){
  point2 po;
  resetPage();
  char ch = 'n';
  while ((ch != 'x')&&(ch != 'y')&&(ch != 'z'))
    ch=inputChar("With x, y, or z constant? ");
  realD=inputNumber("and what is the value of the constant? " );
  if (ch == 'z') rrange2D();
  if (ch == 'y'){
    xmin=inputNumber( "x minimum = " ); newLine();
    xmax=inputNumber( "x maximum = " ); newLine();
    ymin=inputNumber( "z minimum = " ); newLine();
    ymax=inputNumber( "z maximum = " ); newLine();
    rrangeconst2D();
  };
  if (ch == 'x'){
    xmin=inputNumber( "z minimum = " ); newLine();
    xmax=inputNumber( "z maximum = " ); newLine();
    ymin=inputNumber( "y minimum = " ); newLine();
    ymax=inputNumber( "y maximum = " ); newLine();
    rrangeconst2D();
  };
  newLine();
  int finc=inputNumber( "Detail reduced by a factor of... " );

  //initialize colors
  po.x=xmin+(xspan*drand48());
  po.y=ymin+(yspan*drand48());
  if (ch == 'z')
    realA=sqrt(sq(funct3D1(po.x, po.y, realD))+sq(funct3D2(po.x, po.y, realD))+sq(funct3D3(po.x, po.y, realD)));
  if (ch == 'y')
    realA=sqrt(sq(funct3D1(po.x, realD, po.y))+sq(funct3D2(po.x, realD, po.y))+sq(funct3D3(po.x, realD, po.y)));
  if (ch == 'x')
    realA=sqrt(sq(funct3D1(realD, po.y, po.x))+sq(funct3D2(realD, po.y, po.x))+sq(funct3D3(realD, po.y, po.x)));
  realB=realA;
  for (i=0; i < 200; i++){
    po.x=xmin+(xspan*drand48());
    po.y=ymin+(yspan*drand48());
  if (ch == 'z')
    realC=sqrt(sq(funct3D1(po.x, po.y, realD))+sq(funct3D2(po.x, po.y, realD))+sq(funct3D3(po.x, po.y, realD)));
  if (ch == 'y')
    realC=sqrt(sq(funct3D1(po.x, realD, po.y))+sq(funct3D2(po.x, realD, po.y))+sq(funct3D3(po.x, realD, po.y)));
  if (ch == 'x')
    realC=sqrt(sq(funct3D1(realD, po.y, po.x))+sq(funct3D2(realD, po.y, po.x))+sq(funct3D3(realD, po.y, po.x)));
    if (realC < realA) realA=realC;
    if (realC > realB) realB=realC;
  };
  initC(realA, realB);
  
  updateEvents();
  clear();
  writeStopM();
  pixel pos;
  double RealE;
  for (pos.x=0; pos.x <= xpix-finc; pos.x=pos.x+finc){
    for (pos.y=0; pos.y <= ypix-finc; pos.y=pos.y+finc){
      po=pixelPoint(pos);
      if (ch == 'z'){
        realA=funct3D1(po.x, po.y, realD);
        realB=funct3D2(po.x, po.y, realD);
        RealE=funct3D3(po.x, po.y, realD);
      };
      if (ch == 'y'){
        realA=funct3D1(po.x, realD, po.y);
        realB=funct3D2(po.x, realD, po.y);
        RealE=funct3D3(po.x, realD, po.y);
      };
      if (ch == 'x'){
        realA=funct3D1(realD, po.y, po.x);
        realB=funct3D2(realD, po.y, po.x);
        RealE=funct3D3(realD, po.y, po.x);
      };
      realC=sqrt(sq(realA)+sq(realB)+sq(RealE));
      findC(realC);
      fillRectangle(pix, pos.x, pos.y, finc, finc);
    };
    copyArea(pix, w, pos.x, 0, finc, ypix, pos.x, 0); 
    if (checkStop()) pos.x=xpix;
  };
  redraw();

  updateEvents();
  Event e;
  comment("Press the right mouse button to exit. " );
  resetPage();
  e=nextEvent();
  if (e.type == Expose) redraw();
  while ((e.type != ButtonPress)||(e.value != 3)){
    resetTop(10);
    po=eventPoint(e);
    if (ch == 'z'){
      realA=funct3D1(po.x, po.y, realD);
      realB=funct3D2(po.x, po.y, realD);
      RealE=funct3D3(po.x, po.y, realD);
    };
    if (ch == 'y'){
      realA=funct3D1(po.x, realD, po.y);
      realB=funct3D2(po.x, realD, po.y);
      RealE=funct3D3(po.x, realD, po.y);
    };
    if (ch == 'x'){
      realA=funct3D1(realD, po.y, po.x);
      realB=funct3D2(realD, po.y, po.x);
      RealE=funct3D3(realD, po.y, po.x);
    };
    realC=sqrt(sq(realA)+sq(realB)+sq(RealE));
    if (ch == 'z'){
      printString("x = "); printNumber( po.x ); newLine();
      printString("y = "); printNumber( po.y ); newLine(); newLine();
    };
    if (ch == 'y'){
      printString("x = "); printNumber( po.x ); newLine();
      printString("z = "); printNumber( po.y ); newLine(); newLine();
    };
    if (ch == 'x'){
      printString("z = "); printNumber( po.x ); newLine();
      printString("y = "); printNumber( po.y ); newLine(); newLine();
    };
    printString("fx = " ); printNumber( realA ); newLine();
    printString("fy = " ); printNumber( realB ); newLine();
    printString("fz = " ); printNumber( RealE ); newLine(); newLine();
    printString("||f|| = " ); printNumber( realC );
    e=nextEvent();
    if (e.type == Expose) redraw();   
  };  
  resetPage();
}

void fieldIntensity(){
  point2 po;
  resetPage();
  rrange2D();
  newLine();
  int finc=inputNumber( "Detail reduced by a factor of... " );

  //initialize colors
  po.x=xmin+(xspan*drand48());
  po.y=ymin+(yspan*drand48());
  realA=sqrt(sq(funct1(po.x, po.y))+sq(funct2(po.x, po.y)));
  realB=realA;
  for (i=0; i < 200; i++){
    po.x=xmin+(xspan*drand48());
    po.y=ymin+(yspan*drand48());
    realC=sqrt(sq(funct1(po.x, po.y))+sq(funct2(po.x, po.y)));
    if (realC < realA) realA=realC;
    if (realC > realB) realB=realC;
  };
  initC(realA, realB);
  
  updateEvents();
  clear();
  writeStopM();
  pixel pos;
  for (pos.x=0; pos.x <= xpix-finc; pos.x=pos.x+finc){
    for (pos.y=0; pos.y <= ypix-finc; pos.y=pos.y+finc){
      po=pixelPoint(pos);
      realA=funct1(po.x, po.y);
      realB=funct2(po.x, po.y);
      realC=sqrt(sq(realA)+sq(realB));
      findC(realC);
      fillRectangle(pix, pos.x, pos.y, finc, finc);
    };
    copyArea(pix, w, pos.x, 0, finc, ypix, pos.x, 0); 
    if (checkStop()) pos.x=xpix;
  };
  redraw();

  updateEvents();
  Event e;
  comment("Press the right mouse button to exit. " );
  resetPage();
  e=nextEvent();
  if (e.type == Expose) redraw();
  while ((e.type != ButtonPress)||(e.value != 3)){
    resetTop(9);
    po=eventPoint(e);
    realA=funct1(po.x, po.y);
    realB=funct2(po.x, po.y);
    realC=sqrt(sq(realA)+sq(realB));
    printString("x = "); printNumber( po.x ); newLine();
    printString("y = "); printNumber( po.y ); newLine(); newLine();
    printString("fx = " ); printNumber( realA ); newLine();
    printString("fy = " ); printNumber( realB ); newLine(); newLine();
    printString("||f|| = " ); printNumber( realC );
    e=nextEvent();
    if (e.type == Expose) redraw();   
  };  
  resetPage();
}

void kinematics(){
  point2 vel;
  resetPage();
  dt=inputNumber("time interval dt = " ); newLine();
  double mass=inputNumber("mass constant = " );
  comment("Enter the starting point with the mouse" );
  point2 place=pixelPoint( readPoint() );
  pixel placepixel;
  writeStopM();
  setColor(10);
  comment("Simulation in progress" );
  vel.x=0; vel.y=0;
  while (!checkStop()){
    copyArea(pix, w, placepixel.x-5, placepixel.y-5, 10, 10, placepixel.x-5, placepixel.y-5);
    vel.x=vel.x+(funct1(place.x, place.y)*dt/mass);
    vel.y=vel.y+(funct2(place.x, place.y)*dt/mass);
    place.x=place.x+(vel.x*dt);
    place.y=place.y+(vel.y*dt);
    placepixel=pointPixel(place);
    fillCircle(w, placepixel.x, placepixel.y, 4); 
  };
  redraw();
}

void fieldMain(){
  //mathmode = real;
  int box2, box3, back;
  normalize=0; //!
  //modes
  int arrows=1, random=2, manual=3, random3D=4, arrows3D=5;
  
  //assign places for menu areas (all coordinates not written are 0)
  pixel windowB = boringB, kinemB=boringB;
  pixel randomB=boringB, arrowsB=boringB, manualB=boringB, intensityB=boringB;
  pixel f2DB=boringB, f3DB=boringB;
  pixel sliceB=boringB;
  arrowsB.x=1; manualB.x=2, intensityB.x=3; 
  kinemB.x=1;
  f3DB.x=1;
  sliceB.x=2;

  while(1==1){
    setMenu(general);
    clearMenu();
    setColor( textC );
    comment( "Vector fields: main menu" );
    putMenu( f2DB, "2D fields" );
    putMenu( f3DB, "3D fields" );
    getChoice(general);
    mode=0;
    if (backM()) return;
    if (quitM()) exit(1);
    boxM=-2;
    if (chosen(f2DB)){
      setMenu(general);
      comment("2-D vector fields: main menu" );
      back=0;
      while (!back){
      clearMenu();
      setColor( textC );
      putMenu( randomB, "random paths" );
      putMenu( arrowsB, "arrows" );
      putMenu( manualB, "manual paths" );
      putMenu( intensityB, "field strength" );
      getChoice( general );
      mode=0;
      if (chosen(randomB)){ mode=random; rrange2D(); randField();};
      if (chosen(arrowsB)){ mode=arrows; rrange2D(); arrowField();};
      if (chosen(manualB)){
        mode=manual; rrange2D(); manualField();
      };
      if (chosen(intensityB)) fieldIntensity();
      if (backM()) back=1;
      if (quitM()) exit(1);
      boxM=-2;
      while (mode){
        setMenu(panelmenu);
        clearMenu();
        setColor( textC );
        putMenu( windowB, "window" );
        putMenu( kinemB, "kinematics" );
        getChoice( panelmenu );
        if (chosen( windowB )){
          rrange2D();
          if (mode == random) randField();
          if (mode == manual) manualField();
          if (mode == arrows) arrowField();
        };
        if (chosen( kinemB )) kinematics();
        if (backM()) mode=0;
        if (quitM()) exit(1);
        boxM=-2;
      };
      };
    };
    if (chosen(f3DB)){
      comment("3-D vector fields: main menu." );
      back=0;
      while (!back){
      setMenu(general);
      clearMenu();
      setColor( textC );
      putMenu( randomB, "random paths" );
      putMenu( arrowsB, "arrows" );
      //putMenu( manualB, "manual paths" );
      putMenu2( sliceB, "field intensity:", "projective section" );
      getChoice( general );
      mode=0;
      if (chosen(randomB)){
        mode=random3D; rrange3D(); parAxes(1); randField3D();
      };
      if (chosen(arrowsB)){
        mode=arrows3D; rrange3D(); parAxes(1); arrowField3D();
      };
      if (chosen(sliceB)) fieldSection();
      if (backM()) back=1;
      if (quitM()) exit(1);
      boxM=-2;
      while (mode){
        setMenu(panelmenu);
        clearMenu();
        setColor( textC );
        putMenu( windowB, "window" );
        //putMenu( kinemB, "kinematics" );
        getChoice( panelmenu );
        if (chosen( windowB )){
          rrange3D();
          if (mode == random3D) randField();
          //if (mode == manual) manualField();
          if (mode == arrows3D) arrowField();
        };
        //if (chosen( kinemB )) kinematics();
        if (backM()) mode=0;
        if (quitM()) exit(1);
      };
      boxM=-2;
      };
    };

  };
}
