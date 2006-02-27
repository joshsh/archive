struct coast{
  point2 p;
  coast *next;
};

coast *beach, *shore;

int coastC = 30;
int detlev;  //number of divisions of the coastline.
double connectrad=.05; //radius for point connection of enterPolygon().
double polyrad = .8; //radius for regular polygons

int pattern;
int koch=1, rkoch=2;

//area constants
pixel lineB, rpolyB, ipolyB, figureB;
pixel kochB, rkochB;

void killCoast(){
  coast *kc1, *kc2;
  if (beach->next != NULL){
    kc1=beach->next;
    while (kc1->next != NULL){
      kc2=kc1->next;
      delete kc1;
      kc1=kc2;
    };
    delete kc1;
  };
  if (shore->next != NULL){
    kc1=shore->next;
    while (kc1->next != NULL){
      kc2=kc1->next;
      delete kc1;
      kc1=kc2;
    };
    delete kc1;
  };
}

void makeLine(){
  clear();
  killCoast();
  coast *ep1;
  beach->p.x=polyrad;
  beach->p.y=0;
  ep1=new coast;
  beach->next=ep1;
  ep1->p.x=-polyrad;
  ep1->p.y=0;
  setColor( coastC );
  line2D( beach->p, ep1->p );
}

void regularPoly(){
  clear();
  killCoast();
  coast *ep1, *ep2;
  beach->p.x=polyrad;
  beach->p.y=0;
  setColor( textC );
  int polysides=inputNumber( "How many sides should the polygon have? " );
  newLine();
  clear();
  setColor( coastC );
  ep1=beach;
  double sideangle=2*pi/polysides;
  for (i=1; i <= polysides; i++){
    ep2=new coast;
    ep1->next=ep2;
    ep2->p.x=polyrad*cos(i*sideangle);
    ep2->p.y=polyrad*sin(i*sideangle);
    line2D( ep1->p, ep2->p );
    ep1=ep2;
  };
}

void enterPolygon(){
  killCoast();
  comment( "Enter the corners of the polygon, then close it. ");
  setColor( coastC );
  coast *ep1, *ep2;
  clear();
  beach->p=pixelPoint( readPoint() );
  ep1=beach;
  int stop=0;
  while (!stop){
    ep2 = new coast;
    ep1->next=ep2;
    ep2->p=pixelPoint( readPoint() );
    ep1->next = ep2;
    if ( dist2D(beach->p, ep2->p)  < connectrad ){
      ep2->p=beach->p;
      stop=1;
    };
    line2D( ep1->p, ep2->p );
    ep1=ep2;
  };
}

void enterFigure(){
  killCoast();
  comment( "Enter the corners of the figure, right mouse button to end. " );
  setColor( coastC );
  pixel thispix;
  coast *ep1, *ep2;
  clear();
  beach->p=pixelPoint( readPoint() );
  ep1=beach;
  int stop=0;
  while (!stop){
    ep2 = new coast;
    ep1->next=ep2;
    thispix=readPoint();
    ep2->p=pixelPoint( thispix );
    if (areaName(panelmenu, thispix.x, thispix.y)==panelNumber(stopPB)) stop=1;
    ep1->next = ep2;
    line2D( ep1->p, ep2->p );
    ep1=ep2;
  };
}

void dkoch(){
  double midx, midy;
  coast *co1, *co2, *co3, *co4, *kc1, *kc2;
  int read=1;  //1 for the beach string, 2 for the shore string.
  for (i=1; i<detlev; i++){
    if (read == 1){
      co1=beach;
      co3=shore;
    };
    if (read == 2){
      co1=shore;
      co3=beach;
    };
   if ((read==2)&&(beach->next != NULL)){
      kc1=beach->next;
     while (kc1->next != NULL){
        kc2=kc1->next;
        delete kc1;
        kc1=kc2;
     };
      delete kc1;
    };
    if ((read==1)&&(shore->next != NULL)){
      kc1=shore->next;
      while (kc1->next != NULL){
        kc2=kc1->next;
        delete kc1;
        kc1=kc2;
      };
      delete kc1;
    };
    co3->p=co1->p;
    if (read == 1) read=2;
      else read=1;
    while (co1->next != NULL){
      co2=co1->next;
      midx=(co1->p.x+co2->p.x)/2;
      midy=(co1->p.y+co2->p.y)/2;
      co4=new coast;
      co3->next=co4;
      co4->p.x=co1->p.x+((co1->next->p.x-co1->p.x)/3);
      co4->p.y=co2->p.y+((co1->next->p.y-co1->p.y)/3);
      co3=new coast;
      co4->next=co3;
      co3->p.x=midx+co1->p.y-co2->p.y;
      co3->p.y=midy+co2->p.x-co1->p.x; 
      co4=new coast;
      co3->next=co4;
      co4->p.x=co1->p.x+((co1->next->p.x-co1->p.x)*2/3);
      co4->p.y=co2->p.y+((co1->next->p.y-co1->p.y)*2/3); 
      co3=new coast;
      co4->next=co3;
      co3->p=co2->p; 
      co1=co2;
    };
  };
  if (read == 1) co1=beach;
  if (read == 2) co1=shore;
  setColor( coastC );
  while (co1->next != NULL){
    line2D( co1->p, co1->next->p );
    co1=co1->next;
  };
}

void drawCoast(){
  setColor( textC );
  shore=beach;
  detlev=inputNumber( "How many times should the coast be divided? ");
  newLine();
  clear();
  comment( "Drawing coastline..." );
    dkoch();
  comment( "Graphing finished. " );
}

void coastMain(){
  int box, back;
 
  //menu positions (all coordinates not written are 0)
  rpolyB.x=1; ipolyB.x=2; figureB.x=3;
  rkochB.x=1;

  beach = new coast;
  shore = new coast; 

  // set window
  xmin=-1; xmax=1; ymin=-1; ymax=1;
  rrangeconst2D();  

  while (1==1){
    resetAll();
    clearMenu();
    setColor( textC );
    putMenu( lineB, "Line" );
    putMenu( rpolyB, "Reg. polygon" );
    putMenu( ipolyB, "Irr. polygon" );
    putMenu( figureB, "Figure" );
    box=getChoice(general);
    if (menuNumber( lineB ) == box) makeLine();
    if (menuNumber( rpolyB ) == box) regularPoly();
    if (menuNumber( ipolyB ) == box) enterPolygon();
    if (menuNumber( figureB ) == box) enterFigure();
    if (menuNumber( backB ) == box) return;
    if (menuNumber( quitB ) == box) exit(1);
    if (box > -1){
      back=0;
      while (!back){
        clearMenu();
        setColor( textC );
        putMenu( kochB, "Koch curve" );
        putMenu( rkochB, "Random koch" );
        box=getChoice( general );
        if (menuNumber( kochB ) == box){ pattern=koch; drawCoast(); };
        if (menuNumber( rkochB ) == box){ pattern=rkoch; drawCoast(); };
        if (menuNumber( backB ) == box) back=1;
        if (menuNumber( quitB ) == box) exit(1);
      };
    };
  };
  killCoast();
}
