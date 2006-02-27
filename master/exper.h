//experimental fractals

void dprog1(){
  //realA=2*pi;
  realA=inputNumber("Function constant = " ); newLine();
  clearField();
  clear();
  writeStopM();
  for (i=0; i < xpix; i++) field[i][0]=realA*drand48();
  for (i=1; i < ypix; i++) field[0][i]=realA*drand48();
  for (i=1; i < ypix; i++) field[xpix][i]=realA*drand48();
  pixel pos;
  for (pos.y=1; pos.y < ypix; pos.y++){
    if (checkStop()) return;
    for (pos.x=1; pos.x < xpix-1; pos.x++){      realB=sin(field[pos.x-1][pos.y-1]+field[pos.x][pos.y-1]+field[pos.x+1][pos.y-1]);
    setColor( 255*(realB+1)/2 );
    drawPoint(pix, pos.x, pos.y);
    field[pos.x][pos.y]=realB*realA;
    };
  };
  redraw();
}

void dprog2(){
  coloring=bymag;
  resetPage();
  realA=inputNumber("Height constant = " ); newLine();
  clearField();
  clear();
  writeStopM();
  comment("Drawing simulation." );
  updateEvents();
  double highval=0, lowval=0;
  pixel pos;
  while (!checkStop()){
    for(i=0; i<100; i++){
      pos.x=drand48()*xpix;
      pos.y=drand48()*ypix;
      if (pos.x==0) pos.x=1;
      if (pos.x==xpix-1) pos.x=xpix-2;
      if (pos.y==0) pos.y=1;
      if (pos.y==ypix-1) pos.y=ypix-2;  realB=(field[pos.x-1][pos.y]+field[pos.x+1][pos.y]+field[pos.x][pos.y-1]+field[pos.x][pos.y+1])/4;
      realB=realB-realA+(drand48()*realA*2);
      field[pos.x][pos.y]=realB;
      if (pos.x==1) field[xpix-1][pos.y]=realB;
      if (pos.x==xpix-2) field[0][pos.y]=realB;
      if (pos.y==1) field[xpix][ypix-1]=realB;
      if (pos.y==ypix-2) field[xpix][0]=realB;
      if (realB > highval){
        highval=realB;
        initC(lowval, highval);
      };
      if (realB < lowval){
        lowval=realB;
        initC(lowval, highval);
      };
      findC(realB);
      drawPoint(pix, pos.x, pos.y);
    };
    redrawScreen();
  };
  redraw();
}

void dprog3(){
  coloring=bymag;
  resetPage();
  realA=inputNumber("Height constant = " ); newLine();
  clearField();
  clear();
  writeStopM();
  comment("Drawing simulation." );
  updateEvents();
  double highval=0, lowval=0;
  pixel pos;
  while (1==1){
    for (pos.x=1; pos.x < xpix-1; pos.x++){
      if (checkStop()) return;
      for (pos.y=-1; pos.y < ypix-1; pos.y++){  realB=(field[pos.x-1][pos.y]+field[pos.x+1][pos.y]+field[pos.x][pos.y-1]+field[pos.x][pos.y+1])/4;
        realB=realB-realA+(drand48()*realA*2);
        field[pos.x][pos.y]=realB;
        if (pos.x==1) field[xpix-1][pos.y]=realB;
        if (pos.x==xpix-2) field[0][pos.y]=realB;
        if (pos.y==1) field[xpix][ypix-1]=realB;
        if (pos.y==ypix-2) field[xpix][0]=realB;
        if (realB > highval){
          highval=realB;
          initC(lowval, highval);
        };
        if (realB < lowval){
          lowval=realB;
          initC(lowval, highval);
        };
        findC(realB);
        drawPoint(pix, pos.x, pos.y);
      };
      copyArea(pix, w, pos.x, 0, 1, ypix, pos.x, 0);
    };
  };
  redraw();
}

void experMain(){
  setMenu(general);
  resetMenu();
  pixel prog1B=addMenu(), prog2B=addMenu(), prog3B=addMenu();

  while (1==1){
    setMenu(general);
    clearMenu();
    comment( "Experimental fractals" );
    setColor( textC );
    putMenu( prog1B, "program #1" );
    putMenu( prog2B, "program #2" );
    putMenu( prog3B, "program #3" );
    getChoiceM( general );
    if (chosen( prog1B )) dprog1();
    if (chosen( prog2B )) dprog2();
    if (chosen( prog3B )) dprog3();
    if (backM()) return;
    if (quitM()) exit(1);
  };
}
