#include <math.h>

struct point2{
  double x, y;
};

struct basis2{
  point2 a, b;
};

struct p2node{
  point2 p;
  p2node *next;
};

point2 p2(double p2x, double p2y){
  point2 thisone;
  thisone.x=p2x;
  thisone.y=p2y;
  return thisone;
}

point2 add(point2 vectora, point2 vectorb){
  point2 vectorc;
  vectorc.x=vectora.x+vectorb.x;
  vectorc.y=vectora.y+vectorb.y;
  return vectorc;
}

point2 reverse(point2 vectora){
  point2 vectorb;
  vectorb.x=-vectora.x;
  vectorb.y=-vectora.y;
  return vectorb;
}

point2 sub(point2 vectora, point2 vectorb){
  point2 vectorc;
  vectorc.x=vectora.x-vectorb.x;
  vectorc.y=vectora.y-vectorb.y;
  return vectorc;
}

double dotprod(point2 vectora, point2 vectorb){
return (vectora.x * vectorb.x)+(vectora.y * vectorb.y);
}

point2 mult(point2 vectora, double doublea){
  vectora.x=vectora.x * doublea;
  vectora.y=vectora.y * doublea;
  return vectora;
}

double length(point2 va){
  return sqrt((va.x*va.x)+(va.y*va.y));
}

point2 norm(point2 vectora){
  return mult(vectora, 1/length(vectora));
}

  //uses standard metric: norm(b-a)
double metric(point2 vectora, point2 vectorb){
  return sqrt(((vectorb.x-vectora.x)*(vectorb.x-vectora.x))+((vectorb.y-vectora.y)*(vectorb.y-vectora.y)));
}

  //finds the minimum distance from a point to a discrete set of points
double dist(p2node *pgroup, point2 thispoint){
  p2node *runner=pgroup;
  double mindist=metric(runner->p, thispoint);
  if (runner->next == NULL) return mindist;
  else runner=runner->next;
  double barrel;
  int stop=0;
  while (!stop){
    if (runner->next == NULL) stop=1;
    barrel=metric(runner->p, thispoint);
    if (barrel < mindist) mindist=barrel;
    if (!stop) runner=runner->next;
  };
  return mindist;
}








