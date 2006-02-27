#include <math.h>
#include <iostream.h>
#include "plot.h"

int main(){
  int maxit;
  double dt;
  double x, xnew, y, ynew, vx, vy, vxnew, vynew, cosar, sinar;
  double g=10;
  
  cout << endl << "x start = "; cin >> x; cout << endl;
  cout << "y start = "; cin >> y; cout << endl;
  cout << "iterations = "; cin >> maxit; cout << endl;
  cout << "time interval dt = "; cin >> dt; cout << endl;
  
  initWindow(20, 20, 500, 500); 
  setLimits(-1, 1, -1, 1);
  clearGraph();
  drawCircle(pix, 250, 250, 250);
  redraw();
  vx=0; vy=0;
  double vn, xn2, scpd, mess;
  for (int iii=0; iii<maxit; iii++){
    xnew=x+(dt*vx);
    ynew=y+(dt*vy);
    plotDirect(xnew, ynew);
    vy=vy-(dt*g);
    xn2= (xnew*xnew)+(ynew*ynew);
    if (xn2 >= 1){
      vn=sqrt((vx*vx)+(vy*vy));
      scpd=(vx*x)+(vy*y);
      mess= sqrt((vn*vn)-(scpd*scpd))/vn;
      cosar=((scpd*scpd)/(vn*vn))-mess;
      sinar=2*(scpd/vn)*mess;
      vxnew=(vx*cosar)-(vy*sinar);
      vynew=(vy*cosar)+(vx*sinar);
      vx=vxnew; vy=vynew;   
    } else {x=xnew; y=ynew; };
  };
  redraw();
  wait();
}