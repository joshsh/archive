#include <math.h>

struct point3{
  double x, y, z;
};

struct basis23{
  point3 a, b;
};

struct basis33{
  point3 a, b, c;
};

struct p3node{
  point3 p;
  p3node *next;
};

point3 p3(double p3x, double p3y, double p3z){
  point3 thisone;
  thisone.x=p3x;
  thisone.y=p3y;
  thisone.z=p3z;
  return thisone;
}

point3 add(point3 vectora, point3 vectorb){
  point3 vectorc;
  vectorc.x=vectora.x+vectorb.x;
  vectorc.y=vectora.y+vectorb.y;
  vectorc.z=vectora.z+vectorb.z;
  return vectorc;
}

point3 reverse(point3 vectora){
  point3 vectorb;
  vectorb.x=-vectora.x;
  vectorb.y=-vectora.y;
  vectorb.z=-vectora.z;
  return vectorb;
}

point3 sub(point3 vectora, point3 vectorb){
  point3 vectorc;
  vectorc.x=vectora.x-vectorb.x;
  vectorc.y=vectora.y-vectorb.y;
  vectorc.z=vectora.z-vectorb.z;
  return vectorc;
}

double dotprod(point3 vectora, point3 vectorb){
return (vectora.x * vectorb.x)+(vectora.y * vectorb.y)+(vectora.z * vectorb.z);
}

point3 crossprod(point3 vectora, point3 vectorb){
  point3 vectorc;
  vectorc.x=(vectora.y * vectorb.z) - (vectora.z * vectorb.y);
  vectorc.y=(vectora.z * vectorb.x) - (vectora.x * vectorb.z);
  vectorc.z=(vectora.x * vectorb.y) - (vectora.y * vectorb.x);
  return vectorc;
}

point3 mult(point3 vectora, double doublea){
  vectora.x=vectora.x * doublea;
  vectora.y=vectora.y * doublea;
  vectora.z=vectora.z * doublea;
  return vectora;
}

double length(point3 va){
  return sqrt((va.x*va.x)+(va.y*va.y)+(va.z*va.z));
}

point3 norm(point3 vectora){
  return mult(vectora, 1/length(vectora));
}

basis23 getBasisWithOrientation(point3 ori){
  basis23 anna;
  double xything=sqrt((ori.x*ori.x)+(ori.y*ori.y));
  double xyzthing=sqrt((ori.x*ori.x)+(ori.y*ori.y)+(ori.z*ori.z));
  double cosa=ori.z/xyzthing;
  double sina=xything/xyzthing;
  anna.a=mult(p3(ori.x, ori.y, 0), cosa/xything);
  anna.a.z=anna.a.z+sina;
  anna.b=mult(p3(ori.y, -ori.x, 0), 1/xything);
  return anna;
}

  //give it a 2D basis, an angle and a radius, and it returns the vector
  //in the plane of that basis resulting from using the angle and the radius
  //as polar coordinates
point3 usePolar(basis23 thisbas, double angular, double radical){
  return add(mult(thisbas.a, radical*cos(angular)), mult(thisbas.b, radical*sin(angular)));
}

  //uses standard metric: norm(b-a)
double metric(point3 vectora, point3 vectorb){
  return sqrt(((vectorb.x-vectora.x)*(vectorb.x-vectora.x))+((vectorb.y-vectora.y)*(vectorb.y-vectora.y))+((vectorb.z-vectora.z)*(vectorb.z-vectora.z)));
}

  //finds the minimum distance from a point to a discrete set of points
double dist(p3node *pgroup, point3 thispoint){
  p3node *runner=pgroup;
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









