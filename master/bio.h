struct rabbit{
  point2 p;
  point2 vel;
}

struct fox{
  point2 p;
  point2 vel;
  double energy;
};

int maxrabbits=10, maxfoxes=10;
rabbit rlist[10]; //maxrabbits!
fox flist[10];  //maxfoxes!
int totalrabbits, totalfoxes;

double rabbitvel, foxvel;
double rabbitacc, foxacc;
double foxlife;
double rabradius, foxradius;

void eatRabbit(int fox1, int rab1 ){
  for (i = rab1; i < maxrabbits-1; i++) rlist[i]=rlist[i+1];
  totalrabbits=totalrabbits-1;
}

void thinkRabbit( int rab1 ){  //decide what to do and move

}

void thinkFox( int fox1 ){  //decide what to do and move

}

void bioMain(){
  resetAll();
  comment("Rabbits and foxes simulation.  Enter constants. " );

  xmin=0; ymin=0;
  realA=inputNumber("Length of the simulation square (meters) = ");
  xmax=realA; ymax=realA;
  rrangeconst2D();

  totalrabbits=inputNumber("Begin with # rabbits =" );  newLine();
  totalfoxes=inputNumber("Begin with # foxes = " );  newLine();

 //place rabbits and foxes
  for (i=0; i < totalrabbits){
    rlist[i].p.x=xmin+(drand48()*xspan);
    rlist[i].p.y=ymin+(drand48()*yspan);
    rlist[i].vel.x=0;
    rlist[i].vel.y=0;
  };
  for (i=0; i < totalfoxes){
    flist[i].p.x=xmin+(drand48()*xspan);
    flist[i].p.y=ymin+(drand48()*yspan);
    flist[i].vel.x=0;
    flist[i].vel.y=0;
    flist[i].energy=1;
  };

  dt=inputNumber( "Time length of animation frame = " ); newLine();
  rabbitvel=inputNumber("Maximum velocity for rabbit (m/s) = " ); newLine();
  foxvel=inputNumber("Maximum velocity for fox (m/s) = " ); newLine();
  rabbitacc=inputNumber("Maximum acceleration of rabbit (m/s/s) = "); newLine();
  foxacc=inputNumber("Maximum acceleration of fox (m/s/s) = "); newLine();
  foxlife=inputNumber("Life-time of a fox without food (days) = " ); newLine();
  rabradius=inputNumber("Sight radius for rabbit (meters) = " ); newLine();
  foxradius=inputNumber("Sight radius for fox (meters) = "); newLine();

  int rabbitC=backC-1, foxC=10, meadowC=50;
  double rabbitlen=.25, foxlen=.5;

  resetAll();
  writeStop();
  comment("Running simulation.");
  updateEvents();
  while(!checkStop()){
    setColor(meadowC);
    fillRectangle(pix, 0, 0, xpix, ypix);
    for (i=0; i < totalrabbits; i++){
      thinkRabbit(i);
      //draw rabbit
    };
    for (i=0; i < totalfoxes; i++){
      thinkFox(i);
      //draw fox
    };
    copyArea(pix, w, 0, 0, xpix, ypix, 0, 0);
  };
}
