//notes:
//fix trace in F2D.h! (checkStop isn't working and pixels are drawn at ypix)
//fix distribution in the collision model
//fix the string problem in distance.h and coast.h
//find out why printNumber calls "1" ".n"

int brown=1, heap=2, plasma=3;

void dbrown(){
  initC( -realA*10, realA*10 );
  writeStopM();
  comment( "Drawing a brownian landscape. " );
  point3 db, dbl;
  double brownline[500];
  clear();
  tinc=xspan/(intA*1.0);
  db.z=0;
  db.x=xmin;
  db.y=ymin;
  dbl=db;
  for (j=0; j<=intA; j++){
    findC(db.z);
    line3D(db, dbl);
    dbl=db;
    db.y=ymin+(tinc*j);
    db.z=db.z-realA+(drand48()*2*realA);
    brownline[j]=db.z;
  };
  for (i=1; i<=intA; i++){
    db.x=xmin+(tinc*i);
    db.y=ymin;
    db.z=brownline[0]-realA+(drand48()*2*realA);
    brownline[0]=db.z;
    //findC(db.z);
    //line3D(db, dbl);
    if (checkStop()) return;
    for (j=0; j<=intA; j++){
      dbl=db;
      db.y=ymin+(tinc*j);
      db.z=((dbl.z+brownline[j])/2)-realA+(realA*2*drand48());
      brownline[j]=db.z;
      findC(db.z);
      line3D(db, dbl);
    };
  };
}

/*void dheap(){
  clearField();
  double highz=0, lowz=0;
  writeStopM();
  comment( "Forming random stack landscape" );
  while (highz-lowz < realB){

  };
}*/

void dplasma(){
  double list1[256][256], list2[256][256];
  for (i=0; i<256; i++){
    for (j=0; j<256; j++) list1[i][j]=0;
  };
  list2=list1;
  resetWindow();
  //realA=inputNumber("Height constant = " );
  realA=.1;
  comment("Creating plasma field..." );
  double highval=0;
  for (int det=2; det <=256; det=det*2){
    for (i=0; i < det; i++){
      for (j=0; j < det; j++){
        list2[i][j]=list1[i/2][j/2]+(drand48()*realA);
        if (list2[i][j] > highval) highval=list2[i][j];
      };
    };
    list1=list2;
  };
  coloring=bymag;
  initC(0, highval);
  clear();
  comment("Transforming data...");
  for (i=0; i<256; i++){
    for (j=0; j <256; j++){
      findC(list2[i][j]);
      drawPoint(pix, i, j);
    };
  };
  comment("Finished.");
  redraw();
}

void landConsts(){
  if (mode == brown){
    realA=inputNumber("Height constant = " ); newLine();
    intA=inputNumber("Divisions = "); newLine();
  };
  if (mode == heap){
    realA=inputNumber("Height constant = " ); newLine();
    realB=inputNumber("Maximum elevation = " ); newLine();
    intA=inputNumber("Divisions = "); newLine();
  };
}

void landMain(){

  //assign areas for menu positions (all coordinates not written are 0)
  pixel brownB=boringB, plasmaB=boringB, heapB=boringB;
  heapB.x=1; plasmaB.x=2;

  unitRange(3);
  resetAll();
  while (1==1){
    setMenu(general);
    clearMenu();
    setColor(textC);
    putMenu2(brownB, "brownian", "landscape" );
    putMenu2(heapB, "random", "stacking" ); 
    putMenu(plasmaB, "plasma" );
    getChoiceM(general);
    if (chosen(brownB)){ 
      mode=brown; landConsts(); parAxes(1); dbrown(); };
    /*if (chosen(heapB)){
      mode=heap; landConsts(); parAxes(1), dheap(); };*/
    if (chosen(plasmaB)){ mode=plasma; landConsts(); dplasma(); }; 
    if (backM()) return;
    if (quitM()) exit(1);
  };
}
