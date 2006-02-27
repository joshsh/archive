int dotside=5;
int deditside=10;
int xdots=xpix/dotside;
int ydots=ypix/dotside;

int const namelength = 30;
char chosenName[namelength]; //for chooseLifeForm

void drawLifeMatrix(){
  setColor(whiteC);
  fillRectangle(pix, 0, 0,  xpix, ypix);
  setColor(backC);
  for (i=0; i<xdots; i++){
    for (j=0; j<ydots; j++){
      if (setmatrix[i][j]==1) fillRectangle(pix, i*dotside, j*dotside, dotside, dotside);
    };
  };
  redraw();
}

void chooseLifeForm(){
  int chosenline=chooseLine( "Conway/life.list" ); 
  
  int linefeeds=0;
  fstream fileA;
  fileA.open( "Conway/life.list" , ios::in );
  char ch=fileA.get();
  while (linefeeds < chosenline){
    if (ch == 10) linefeeds++;
    ch=fileA.get();
  };
  for (i=0; i<namelength; i++) chosenName[i]=0;
  chosenName[0]='C';
  chosenName[1]='o';
  chosenName[2]='n';
  chosenName[3]='w';
  chosenName[4]='a';
  chosenName[5]='y';
  chosenName[6]='/';     
  i=7;
  while (ch != 10){
    chosenName[i]=ch;
    ch=fileA.get();
    i++;
  };
  for (i=0; i<namelength; i++) if( chosenName[i]==' ') chosenName[i]='_';

    //add the suffix '.life'
  i=0;
  while (chosenName[i]>0) i++;
  chosenName[i]='.';
  chosenName[i+1]='l'; 
  chosenName[i+2]='i'; 
  chosenName[i+3]='f'; 
  chosenName[i+4]='e'; 
}

void getLifeForm(){
  resetMatrix2();
  setColor(whiteC);
  fillRectangle(pix, pagecor.x, pagecor.y, pagelength, pageheight);
  fstream fileA;
  chooseLifeForm();
  while (countLines(chosenName) < 2) chooseLifeForm();
  fileA.open( chosenName, ios::in );
  if (!fileA.good){
    cout << endl << "The life form chosen is not a valid file." << endl;
    exit(1);
  };
  char ch=fileA.get();
  
    //find reference corner and matrix reference corner
  int lifetall, lifewide;
  lifetall=countLines(chosenName);
  lifewide=0;
  while (ch != 10){
    lifewide++;
    ch=fileA.get();
  };
  pixel refcor;
  refcor.x=pagecor.x+(pagelength/2.0)-(lifewide*deditside/2.0);
  refcor.y=pagecor.y+(pageheight/2.0)-(lifetall*deditside/2.0);
  int matx=(refcor.x-pagecor.x)/deditside;
  int maty=(refcor.y-pagecor.y)/deditside;
  refcor.x=pagecor.x+(matx*deditside);
  refcor.y=pagecor.y+(maty*deditside);
  //cout << endl << "x = " << refcor.x << " y = " << refcor.y << endl;
  
  setColor(whiteC);
  fillRectangle(pix, pagecor.x, pagecor.y, pagelength, pageheight);
  fileA.close();
  fileA.open( chosenName, ios::in);
  i=0; j=0;
  ch=fileA.get();
  //resetMatrix();
  setColor(backC);
  while(!fileA.eof()){
    if (ch == '1'){
      fillRectangle(pix, refcor.x+(deditside*i), refcor.y+(deditside*j), deditside, deditside);
      setmatrix2[matx+i][maty+j]=1;
    };
    ch=fileA.get();
    i++;
    if (ch == 10){ i=0; j++; ch=fileA.get(); };
  };
  redraw();
}

void editLife(){  
  int thisx, thisy;
  int box, backCt;
  Event e;
  updateEvents();
  setColor(backC);
  int stop=0;
  while(!stop){
    e=getNextEvent();
    if (e.type == ButtonPress){
      if ((e.value == 1 )&&(e.x >= pagecor.x)&&(e.y >= pagecor.y)){
        thisx=(e.x-pagecor.x)/deditside;
        thisy=(e.y-pagecor.y)/deditside;
        i=setmatrix2[thisx][thisy];
        if (!i) j=1; else j=0;
        setmatrix2[thisx][thisy]=j;
        k=getColor();
        if (!j) setColor(whiteC); else setColor(backC);
        drawLargePointOffset(pix, thisx, thisy, deditside, pagecor);
        drawLargePointOffset(win, thisx, thisy, deditside, pagecor);
        setColor(k);
      };
      if (e.value == 3) return;
    };
    if (e.type == Expose) redraw();
  };
}

transplantLifeForm(){
  pixel p1, p2, p3;
  updateEvents();
  
    //for setmatrix2
  Event e = nextEvent();
  while (e.type != ButtonPress) e=nextEvent();
  p1.x=(e.x-pagecor.x)/deditside;
  p1.y=(e.y-pagecor.y)/deditside;
  e=nextEvent();
  while (e.type != ButtonPress) e=nextEvent();
  p2.x=(e.x-pagecor.x)/deditside;
  p2.y=(e.y-pagecor.y)/deditside;
  if (p1.x > p2.x){
    i=p1.x;
    p1.x=p2.x;
    p2.x=i;
  };
    if (p1.y > p2.y){
    i=p1.y;
    p1.y=p2.y;
    p2.y=i;
  };
  int lifelong, lifetall;
  lifelong=p2.x-p1.x;
  lifetall=p2.y-p1.y;

    //for setmatrix  
  e=nextEvent();
  while (e.type != ButtonPress) e=nextEvent();
  p3.x=e.x/dotside;
  p3.y=e.y/dotside;
  for( i=0; i<lifelong; i++){
    for( j=0; j<lifetall; j++ ){
      if (setmatrix2[i+p1.x][j+p1.y] == 1) setmatrix[i+p3.x][j+p3.y]=1;
    };
  };
  drawLifeMatrix();  
}

void conwayInputs(){
  setMenu(panelmenu);
  int thisx, thisy;
  pixel clearPB;
  //clearPB.y=backPB.y;
  //clearPB.x=backPB.x-1;
  clearPB.y=menu[mplace(panelmenu)].rows-1;
  clearPB.x=menu[mplace(panelmenu)].cols-3;
  int box, backCt;
  setColor(textC);
  clearMenu();
  putMenu(clearPB, "clear" );
  putMenu(lastM(2), "back" );
  putMenu(lastM(1), "quit" );
  Event e;
  updateEvents();
  setColor(backC);
  int stop=0;
  while(!stop){
    e=getNextEvent();
    if (e.type == ButtonPress){
      if ((e.x<xpix)&&(e.y<ypix)){
        thisx=e.x/dotside;
        thisy=e.y/dotside;
        i=setmatrix[thisx][thisy];
        if (!i) j=1; else j=0;
        setmatrix[thisx][thisy]=j;
        k=getColor();
        if (!j) setColor(whiteC); else setColor(backC);
        drawLargePoint(pix, thisx, thisy, dotside);
        drawLargePoint(win, thisx, thisy, dotside);
        setColor(k);
      };
      if (onPanel(e.x, e.y)){
        box = areaName(panelmenu, e.x, e.y);
        if (menuNumber(clearPB) == box){
          resetMatrix();
          backCt=backC;
          backC=whiteC;
          clear();
          backC=backCt;
        };
        if (chosen(lastM(2), box)) stop=1;
        if (chosen(lastM(1), box)) exit(1);
      };
    };
    if (e.type == Expose) redraw();
  };
}

 //uses setmatrix and setmatrix2 alternately, using setmatrix in the first
 //generation.
void life(){
  //int generations=0;
  int ii;
  int imin=xpix-1, jmin=ypix-1, imax=0, jmax=0, imid=xpix/2, jmid=ypix/2;
  for (i=0; i< xpix; i++){
    for (j=0; j < ypix; j++){
      if (setmatrix[i][j]==1){
        if( i < imin ) imin=i;
        if( j < jmin ) jmin=j;
        if( i > imax ) imax=i;
        if( j > jmax ) jmax=j;
      };
    };
  }; 
  
  /*
  cout << endl;
  cout << imin << endl;
  cout << imax << endl;
  cout << jmin << endl;
  cout << jmax << endl;
  exit(1);
  */
   
  updateEvents();
  while (1==1){
    //generations++;
    //cout << endl << generations;
    resetMatrix2();
    for (i=imin-1; i<=imax+1; i++){
      for (j=jmin-1; j <=jmax+1; j++){
        k=0;
        if ((i>0)&&(j>0)&&(i<xdots-1)&&(j<ydots-1)){
          k=setmatrix[i-1][j]+setmatrix[i+1][j];
          for(ii=i-1; ii <= i+1; ii++) k=k+setmatrix[ii][j-1]+setmatrix[ii][j+1];         
        }; //just kill the cells at the edge.
        if ((k<2)||(k>3)){
          if (setmatrix[i][j]){
           setmatrix2[i][j]=0; 
           setColor(whiteC);
           k=1;
          }
          else k=0;
        };
        if (k==2){
          setmatrix2[i][j]=setmatrix[i][j];
          k=0;
        };
        if (k==3){
          setmatrix2[i][j]=1; 
          if (i<imin) imin=i;
          if (i>imax) imax=i;
          if (j<jmin) jmin=j;
          if (j>jmax) jmax=j;
          setColor(backC);
        };
        if (k>0) drawLargePoint(pix, i, j, dotside);
      };
    };
    redraw();
    if (checkStop()) return;
    resetMatrix();
    for (i=imin-1; i<=imax+1; i++){
      for (j=jmin-1; j <=jmax+1; j++){
        k=0;
        if ((i>0)&&(j>0)&&(i<xdots-1)&&(j<ydots-1)){
          k=setmatrix2[i-1][j]+setmatrix2[i+1][j];
          for(ii=i-1; ii <= i+1; ii++) k=k+setmatrix2[ii][j-1]+setmatrix2[ii][j+1];         
        }; //just kill the cells at the edge.
        if ((k<2)||(k>3)){
          if (setmatrix2[i][j]){
            setmatrix[i][j]=0; 
            setColor(whiteC);
            k=1;
          }
          else k=0;
        };
        if (k==2){
          setmatrix[i][j]=setmatrix2[i][j];
          k=0;
        };
        if (k==3){
          setmatrix[i][j]=1;
          if (i<imin) imin=i;
          if (i>imax) imax=i;
          if (j<jmin) jmin=j;
          if (j>jmax) jmax=j;
          setColor(backC);
        };
        if (k>0) drawLargePoint(pix, i, j, dotside);
      };
    };
    redraw();
    if (checkStop()) return;
  };
}

void conwayMain(){
  int box;
  
  //assign menu positions
  setMenu(general);
  resetMenu();
  pixel conInputB=addMenu();
  pixel simB=addMenu();
  pixel conClearB=addMenu();
  pixel importB=addMenu();
  
  resetMatrix();
  int backCt=backC;
  backC=whiteC;
  clear();
  backC=backCt;
  
  while (1==1){
    setMenu(general);
    resetAll();
    setColor(textC);
    putMenu(conInputB, "edit field" );
    putMenu(conClearB, "clear field" );
    putMenu(simB, "run simulation" );
    putMenu2(importB, "import", "pattern" );
    putMenu(lastM(2), "back");
    putMenu(lastM(1), "quit");
    box=getChoice(general);
    if (menuNumber(conInputB) == box) conwayInputs();
    if (menuNumber(conClearB) == box){
      resetMatrix();
      setColor(whiteC);
      fillRectangle(pix, 0, 0, xpix, ypix);
      redraw();
    };
    if (menuNumber(simB) == box) life();
    if (menuNumber(importB) == box){
      getLifeForm();
      editLife();
      transplantLifeForm();
    };
    if (chosen(lastM(2), box)) return;
    if (chosen(lastM(1), box)) exit(1);
  };
}