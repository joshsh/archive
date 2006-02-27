//you should have a choice betweem 2D and 3d simulation.

struct massPoint{
  point3 r; 
  point3 v; 
  double mass; 
  double charge;
}

int masses=5;
massPoint mpList[5]; //[masses]!

void clearMpList(){
  for (i=0; i<masses; i++){
    mpList[i].r=zero3;
    mpList[i].v=zero3;
    mpList[i].mass=0;
    mpList[i].charge=0;
  };
}

void stellarSim(){
  
}

void coloumbSim(){

}

void mixedSim(){

}

void mechanicsMain(){
  int box, goon, stop;
  //mathmode=real;
  //assign menu positions
  
  while (1==1){
    resetAll();
    setColor(textC);
    putMenu2(kinemB, "kinematics");
    box=getChoice(general);
    if (menuNumber(box) == backB)) return;
    if (menuNumber(box) == quitB)) exit(1);
    if (menuNumber(box) == kinemB)){
      stop=0;
      while (stop=0){
        resetAll();
        setColor(textC);
        putMenu2( numInputB, "numerical", "input" );
        putMenu2( manualB, "manual", "input" );
        box=getChoice(general);
        goon=0;
        if (menuNumber( backB ) == box) stop=1;
        if (menuNumber( quitB ) == box) exit(1);
        if (menuNumber( numInputB ) == box ){ rrange2D(); numericalInput(); 
          goon=1; };
        if (menuNumber( manualB ) == box ){ rrange2D(); manualInput(); goon=1; };
        if (goon){
          inputNumber( "time interval dt (sec) = "); newLine();
          
        };
      };
    };
  };
}
