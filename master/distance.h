struct dot{
  double x, y;
  dot *next;
};

dot *first=new dot;

int dotC=backC-1;
int distmode;
int pointset=1, dirichlet=2;

pixel posD;

//=========================================================================

void killDots(){
  dot *kd1, *kd2;
  if (first->next != NULL){
    kd1=first->next;
    while (kd1->next != NULL){
      kd2=kd1->next;
      delete kd1;
      kd1=kd2;
    };
    delete kd1;
  };
}

void makePointList(){
  killDots();
  int lastC=getColor();
  dot *mp1=first, *mp2;
  pixel mp3;
  point2 mp4, mp5;
  clear();
  setColor( dotC );
  comment( "Enter the points with the mouse");
  writeStopM();
  Event e=getNextEvent();
  while ((e.type != ButtonPress)||(e.value != 3)){
    if ((e.type == ButtonPress)&&(e.value == 1)){
      drawPoint(win, e.x, e.y);
      drawPoint(pix, e.x, e.y);
      mp2=new dot;
      mp1->next=mp2;
      mp3.x=e.x;
      mp3.y=e.y;
      mp4=pixelPoint( mp3 );
      mp2->x=mp4.x;
      mp2->y=mp4.y;
      mp1=mp2;
    };
    if (checkStop()) return;
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
  //connect dots, to see whether the storage worked.
  mp1=first;
  mp4.x=mp1->x;
  mp4.y=mp1->y;
  while (mp1->next != NULL){
     mp1=mp1->next;
     mp5.x=mp1->x;
     mp5.y=mp1->y;
     line2D( mp4, mp5 );
     mp4=mp5;
  };
  setColor( lastC );
}

void dpoints(){
  dot *dp1;
  point2 dp2, dp3;
  double lowdist, thisdist;
  comment( "Point-distance graphing in progress.");
  for (posD.x=0; posD.x <= xpix; posD.x=posD.x+redfac){
    dp3.x=xmin+(xinc*(posD.x+.5));
    for (posD.y=0; posD.y <= ypix; posD.y=posD.y+redfac){
      dp3.y=ymax-(yinc*(posD.y+.5));
      if (checkStop()) return;
      dp1=first;
      dp2.x=dp1->x;
      dp2.y=dp1->y;
      lowdist=dist2D( dp3, dp2 );
      while (dp1->next != NULL){
        dp1=dp1->next;
        dp2.x=dp1->x;
        dp2.y=dp1->y;
        thisdist=dist2D( dp3, dp2 );
        if (thisdist < lowdist) lowdist=thisdist;
      };
      dotR( pos, lowdist );
    };
  }; 
}

void ddirichlet(){
  chain *firstchain, *thischain=firstchain, *chain2;
  dot *dd1=first;
  thischain.v=randInt(255);
  while (dd1->next != NULL){
    dd1=dd1->next;
    chain2=new chain;
    thischain->next=chain2;
    thischain=chain2;
    thischain.v=randInt(255);
  };
  
  int tileC;
  comment( "Dirichlet tiling procedure in progress.");
  for (posD.x=0; posD.x <= xpix; posD.x=posD.x+redfac){
    dp3.x=xmin+(xinc*(posD.x+.5));
    for (posD.y=0; posD.y <= ypix; posD.y=posD.y+redfac){
      dp3.y=ymax-(yinc*(posD.y+.5));
      if (checkStop()) return;
      dp1=first;
      thischain=firstchain;
      dp2.x=dp1->x;
      dp2.y=dp1->y;
      lowdist=dist2D( dp3, dp2 );
      while (dp1->next != NULL){
        dp1=dp1->next;
        thischain=thischain->next;
        dp2.x=dp1->x;
        dp2.y=dp1->y;
        thisdist=dist2D( dp3, dp2 );
        if (thisdist < lowdist){ lowdist=thisdist; tileC=thischain.v; };
      };
      setColor(tileC);
      drawPoint(win, posD.x, posD.y);
      drawPoint(pix, posD.x, posD.y);
    };
  };  
}

void drawDist(){
  //comment("Graphing in progress." );
  clear();
  writeStopM();
  switch (distmode){
    case pointset: dpoints(); break;
    case dirichlet: ddirichlet(); break;
  };
}

void distConsts(){
  setColor( textC );
  cconst=inputNumber( "Color constant = " );
  redfac=inputNumber( "Reduce detail by a factor of..." );
}

void distanceMain(){
  setPage(mpage);

  maxit=1; //!

  pixel pointsB, dirichletB;
  pixel refreDB, zoomiDB, zoomoDB, constDB, colorDB, pointDB, inforDB;
  pixel graphDB, windoDB;
  setMenu(general);
  resetMenu();
  pointsB=addMenu();
  dirichletB=addMenu();
  
  setMenu(panelmenu)
  resetMenu();
  colorDB=addMenu();
  constDB=addMenu();
  graphDB=addMenu();
  inforDB=addMenu();
  pointDB=addMenu();
  refreDB=addMenu();
  zoomiDB=addMenu();
  zoomoDB=addMenu();
 
  getColors();
  int box, back;
  while(1==1){
    resetAll();
    setColor(textC);
    setMenu(general);
    putMenu(pointsB, "Points" );
    putMenu(dirichletB, "Dirichlet");
    box = getChoice( general );
    if (menuNumber(pointsB) == box){
      rrange2D();
      makePointList();
      distConsts();
      distmode=pointset;
      drawDist();
    };
    if (menuNumber(dirichletB) == box){
      rrange2D();
      makePointList();
      distmode=dirichlet;
      drawDist();    
    };
    if (backM(box)) return;
    if (quitM(box)) exit(1);
    back=0;
    while (!back){
      setMenu(panelmenu);
      clearMenu();
      setColor( textC );
      putMenu( refreDB, "refresh screen" );
      putMenu( zoomiDB, "zoom in" );
      putMenu( zoomoDB, "zoom out" );
      putMenu( constDB, "constants" );
      putMenu( colorDB, "colors" );
      putMenu( pointDB, "point change" );
      putMenu( inforDB, "information" );
      putMenu( graphDB, "draw graph" ); 
      putMenu( windoDB, "window" );
      writeBackM();
      writeQuitM();
      box=getChoice( panelmenu );
      if (box == menuNumber( refreDB )) resetAll();
      if (box == menuNumber( constDB )) distConsts();
      if (box == menuNumber( colorDB )){ getColors(); recolorR(); };
      if (box == menuNumber( graphDB )) drawDist();
      //if (box == menuNumber( inforDB )) orbitInfo();
      if (box == menuNumber( windoDB )) rrange2D();
      if (box == menuNumber( zoomiDB )){ zoom2D(); drawDist(); };
      if (box == menuNumber( zoomoDB )){ zoomOut2D(); drawDist(); };
      if (backM(box)) back=1;
      if (quitM(box)) exit(1);
    };
  };
  killDots();
}
