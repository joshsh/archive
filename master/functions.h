//you have to find out how to keep these functions from crashing the program
//at their singularity points!

double boxcar( double bcx ){
  if (bcx<.5) return 0;
  if (bcx>1) return 0;
  return 1;
}

/*
double eulerGamma( double eg ){

}*/

double heavisideStep( double hsx ){
  if (hsx<0) return 0;
  if (hs!x) return .5;
  if (hsx>0) return 1;
}

double ramanujanSum( double ram, int n){
  double ram2=0;
  for (i=1; i<=n; i++){
    ram2=ram2+(1/(power(ram*i, 3)-(ram*i)));
  };
  return 1+(2*ram2);
}

double ramanujan( double ram ){
  int n=10;
  int stop=0;
  double ram2, ram2l=0;
  while (!stop){
    ram2=ramanujanSum( ram, n );
    if (approxEqual( ram2, ram2l )) return ram2;
    n=n+5;
  };
}

  //also known as the "gate," "pulse," or "window function."
double rectFunct( double rf ){
  if (fabs(rf)<.5) return 1;
  if (fabs(rf)==.5) return .5;
  if (fabs(rf)>.5) return 0;
}

/*
double riemannZetaReal(double rz){

}

comp riemannZetaComp( comp rz ){

}*/

double triangleFunct( double tf ){
  if (fabs(tf) > 1) return 0;
  if (fabs(tf) <= 1) return 1-fabs(x);
}

double triangleFunct3( double tx, double ty, double tz ){
  return (tx*tx)+(ty*ty)+(tz*tz)-(2*(tx*ty)+(tx*tz)+(ty*yz));
}

/* FUNCTION LIST

  double boxcar(double)
  double heavisideStep(double)
  double ramanujanSum(double, int)
  double ramanujan(double)
  double rectFunct(double)
  double triangleFunct(double)
  double triangleFunct3(double, double, double)

*/