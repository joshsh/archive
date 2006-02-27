void dbrown2D(){
  xmin=-1; xmax=1; ymin=-1; ymax=1;
  rrangeconst2D();
  resetWindow();
  //double bconst=inputNumber("Speed constant = " );
  double bconst=controlBar("Speed constant = ", 0, .2, xpix, 0, pagelength);
  realA=controlBar("Color constant = ", 0, 10000, xpix, 0, pagelength );
  writeStop();
  point2 brown, brownl;
  brown.x=0;
  brown.y=0;
  clear();
  double colinc=256/realA;
  while (!checkStop()){
    for (realB=0; realB<256; realB=realB+colinc){
      setColor(realB);
      line2D(brown, brownl);
      brownl=brown;
      brown.y=brown.y-bconst+(drand48()*bconst*2);
      brown.x=brown.x-bconst+(drand48()*bconst*2);
    };
  };
 redraw();
}

void dbrown3D(){
  xmin=-1; xmax=1; ymin=-1; ymax=1; zmid=0;
  rrangeconst3D();
  resetWindow();
  double bconst=inputNumber("Speed constant = " );
  realA=inputNumber("Color constant = " );
  parAxes(1);
  clearField();
  clear();
  graphAxes();
  writeStop();
  comment("Graphing a 3D random path. " );
  point3 brown;
  brown.x=0; brown.y=0; brown.z=0;
  double colinc=256/realA;
  while (!checkStop()){
    for (realB=0; realB<256; realB=realB+colinc){
      setColor(realB);
      inFront(brown);
      brown.z=brown.z-bconst+(drand48()*bconst*2);
      brown.y=brown.y-bconst+(drand48()*bconst*2);
      brown.x=brown.x-bconst+(drand48()*bconst*2);
    };
  };
 redraw();
}

void dinertia2D(){
  clear();
  resetWindow();
  double inconst=inputNumber("inertia constant = " ); newLine();
  realA=inputNumber("color constant = " ); newLine();
  point2 in, inl, invel;
  xmin=-1; xmax=1; ymin=-1; ymax=1;
  rrangeconst2D();
  in.x=0; in.y=0; invel.x=0; invel.y=0;
  writeStop();
  double colinc=256/realA;
  while(!checkStop()){
    for (realB=0; realB<256; realB=realB+colinc){
      setColor(realB);
      inl=in;
      invel.x=invel.x-inconst+(drand48()*inconst*2);
      invel.y=invel.y-inconst+(drand48()*inconst*2);
      in.x=in.x+invel.x;
      in.y=in.y+invel.y;
      line2D(in, inl);
    };    
  };
  redraw();
}
 
void dinertia3D(){
  resetWindow();
  double inconst=inputNumber("inertia constant = " ); newLine();
  realA=inputNumber("color constant = " ); newLine();
  point3 in, invel;
  xmin=-1; xmax=1; ymin=-1; ymax=1; zmid=0;
  rrangeconst3D();
  in.x=0; in.y=0; in.z=0; invel.x=0; invel.y=0; invel.z=0;
  parAxes(1);
  clear();
  graphAxes();
  writeStop();
  double colinc=256/realA;
  while(!checkStop()){
    for (realB=0; realB<256; realB=realB+colinc){
      setColor(realB);
      invel.x=invel.x-inconst+(drand48()*inconst*2);
      invel.y=invel.y-inconst+(drand48()*inconst*2);
      invel.z=invel.z-inconst+(drand48()*inconst*2);
      in.x=in.x+invel.x;
      in.y=in.y+invel.y;
      in.z=in.z+invel.z;
      inFront(in);
    };    
  };
 redraw();
}

void dgrid2D(){
  resetWindow();
  realA=inputNumber("length constant = " ); newLine();
  realC=inputNumber("color constant = " ); newLine();
  point2 gd, gdl;
  xmin=-1; xmax=1; ymin=-1; ymax=1;
  rrangeconst2D();
  clear();
  writeStop();
  int dir;
  comment("Grid simulation in progress. " );
  double colinc=256/realC ;
  gd.x=0; gd.y=0;
  while (!checkStop()){
    for(realB=0; realB < 256; realB=realB+colinc ){
      setColor(realB);
      dir=randInt(3)+1;
      gdl=gd;
      if (dir==1) gd.y=gd.y+realA;
      if (dir==2) gd.x=gd.x+realA;
      if (dir==3) gd.y=gd.y-realA;
      if (dir==4) gd.x=gd.x-realA;
      line2D(gd, gdl);
    };
  };
}

void dgrid3D(){
  resetWindow();
  realA=inputNumber("length constant = " ); newLine();
  realC=inputNumber("color constant = " ); newLine();
  point3 gd;
  xmin=-1; xmax=1; ymin=-1; ymax=1; zmid=0;
  rrangeconst3D();
  clear();
  parAxes(1);
  clear();
  clearField();
  graphAxes();
  writeStop();
  int dir;
  comment("Grid simulation in progress. " );
  double colinc=256/realC ;
  gd.x=0; gd.y=0; gd.z=0;
  while (!checkStop()){
    for(realB=0; realB < 256; realB=realB+colinc ){
      setColor(realB);
      dir=randInt(5)+1;
      if (dir==1) gd.y=gd.y+realA;
      if (dir==2) gd.x=gd.x+realA;
      if (dir==3) gd.y=gd.y-realA;
      if (dir==4) gd.x=gd.x-realA;
      if (dir==5) gd.z=gd.z+realA;
      if (dir==6) gd.z=gd.z-realA;
      inFront(gd);
    };
  };
}

void ddirection2D(){
  resetWindow();
  realA=inputNumber("length increment = " );
  realB=inputNumber("angle increment = " );
  realC=inputNumber("color constant = " );
  point2 in, inl;
  unitRange(2);
  in.x=0; in.y=0;
  inl=in;
  writeStop();
  double colinc=256/realC;
  double angle = 0;
  while(!checkStop()){
    for (realD=0; realD<256; realD=realD+colinc){
      setColor(realD);
      inl=in;
      angle=angle-realB+(drand48()*realB*2);
      in.x=in.x+(realA*cos(angle));
      in.y=in.y+(realA*sin(angle));
      line2D(in, inl);
    };  
    angle = fixAngle(angle);  
  };
  redraw();  
}

void ddirection3D(){
  resetWindow();
  realA=inputNumber("length increment = " );
  realB=inputNumber("angle increment = " );
  realC=inputNumber("color constant = " );
  point3 in;
  unitRange(3);
  clearField();
  parAxes(1);
  clear();
  graphAxes();
  in.x=0; in.y=0; in.z=0;
  writeStop();
  double colinc=256/realC;
  double angle1 = 0, angle2 = 0;
  while(!checkStop()){
    for (realD=0; realD<256; realD=realD+colinc){
      setColor(realD);
      angle1=angle1-realB+(drand48()*realB*2);
      angle2=angle2-realB+(drand48()*realB*2);
      in.x=in.x+(realA*cos(angle1)*cos(angle2));
      in.y=in.y+(realA*sin(angle1)*cos(angle2));
      in.z=in.z+(realA*sin(angle2));
      inFront(in);
    };  
    angle1 = fixAngle(angle1); 
    angle2 = fixAngle(angle2); 
  };
  redraw();  
}

void dtube(){
  resetWindow();
  realA=inputNumber("length increment = " );
  realB=inputNumber("angle increment = " );
  double radinc=inputNumber("radius increment = " );
  realC=inputNumber("color constant = " );
  point3 in;
  unitRange(3);
  clearField();
  parAxes(1);
  clear();
  graphAxes();
  in.x=0; in.y=0; in.z=0;
  writeStop();
  double colinc=256/realC;
  double angle1 = 0, angle2 = 0;
  double Radius=0;
  while(!checkStop()){
    for (realD=0; realD<256; realD=realD+colinc){
      setColor(realD);
      angle1=angle1-realB+(drand48()*realB*2);
      angle2=angle2-realB+(drand48()*realB*2);
      in.x=in.x+(realA*cos(angle1)*cos(angle2));
      in.y=in.y+(realA*sin(angle1)*cos(angle2));
      in.z=in.z+(realA*sin(angle2));
      inFront(in);
    };  
    angle1 = fixAngle(angle1); 
    angle2 = fixAngle(angle2); 
  };
  redraw();  
}

void randomMain(){
  int box;

  //assign areas for menu positions (all coordinates not written are 0)
  pixel brown2B=boringB, brown3B=boringB, inertia2B=boringB, inertia3B=boringB;
  pixel grid2B=boringB, grid3B=boringB, direct2B=boringB, direct3B=boringB;
  pixel tubeB=boringB;
  brown2B.x=1; inertia2B.x=2; inertia3B.x=3;
  grid2B.y=1; grid3B.y=1; direct2B.y=1; direct3B.y=1;
  grid3B.x=1; direct2B.x=2; direct3B.x=3;
  tubeB.y=2;

  resetAll();
  while (1==1){
    comment( "Randomness main menu." );
    setColor(textC);
    putMenu2(brown2B, "2D brownian", "motion" );
    putMenu2(brown3B, "3D brownian", "motion" );
    putMenu(inertia2B, "2D inertia" );
    putMenu(inertia3B, "3D inertia" );
    putMenu(grid2B, "2D grid" );
    putMenu(grid3B, "3D grid" );
    putMenu(direct2B, "2D direction" );
    putMenu(direct3B, "3D direction" );
    putMenu(tubeB, "'Tube'" );
    box=getChoice( general );
    if (menuNumber(brown2B) == box) dbrown2D();
    if (menuNumber(brown3B) == box) dbrown3D();
    if (menuNumber(inertia2B) == box) dinertia2D();
    if (menuNumber(inertia3B) == box) dinertia3D();
    if (menuNumber(grid2B) == box) dgrid2D();
    if (menuNumber(grid3B) == box) dgrid3D();
    if (menuNumber(direct2B) == box) ddirection2D();
    if (menuNumber(direct3B) == box) ddirection3D();
    if (menuNumber(tubeB) == box) dtube();
    if (menuNumber(backB) == box) return;
    if (menuNumber(quitB) == box) exit(1);
  };
}
