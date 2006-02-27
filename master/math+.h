#include <math.h>

double const pi=3.1415;

struct point2{
  double x, y;
};

struct point3{
  double x, y, z;
};

struct chain{
  double v;
  chain *next;
};

  //preliminary declarations
double sq(double);
double tenpow(int);
int trunc(double, int);

  //rand48.h
#ifndef	__random_h_info_I
#ifdef __cplusplus
extern "C" {
#endif
#define	__random_h_info_I
extern void srand48( long seedval );
extern double drand48( void );
#endif /* defined(__cplusplus) */
#ifdef __cplusplus
}
#endif

int approxEqual( double ae1, double ae2, int digits ){
  return (trunc(ae1, digits) == trunc(ae2, digits));
}

int brack( double b1 ){
  int b2=int(b1);
  return b2; 
}

double cut( double c1, int digits){
  return trunc(c1, int(digits-tenpow(int(c1))));
}

double dist2D( point2 d1, point2 d2){
  return sqrt( sq(d1.x-d2.x) + sq(d1.y-d2.y) );
}

double dist3D( point3 d1, point3 d2 ){
  return sqrt( sq(d1.x-d2.x) + sq(d1.y-d2.y) + sq(d1.z-d2.z) );
}

double fixAngle(double preAngle){
  if ((preAngle >=0 ) && (preAngle <= (2*pi))) return preAngle;
  while (preAngle > (2*pi)) preAngle=preAngle-(2*pi);
  while (preAngle < 0) preAngle=preAngle+(2*pi);
  return preAngle;
}

double max( double m1, double m2){
  if (m1 > m2) return m1;
  else return m2;
}

double min( double m1, double m2 ){
  if (m1 < m2) return m1;
  else return m2;
}

point2 mpoint2( double mp1, double mp2 ){
  point2 mp3;
  mp3.x=mp1;
  mp3.y=mp2;
  return mp3;
}

point3 mpoint3( double mp1, double mp2, double mp3 ){
  point3 mp4;
  mp4.x=mp1;
  mp4.y=mp2;
  mp4.z=mp3;
  return mp4;
}

double norm2D( point2 d1 ){
  return sqrt( sq(d1.x) + sq(d1.y) );
}

double norm3D( point3 d1 ){
  return sqrt( sq(d1.x) + sq(d1.y) + sq(d1.z) );
}

  //only for integer expontents!
double power( double po, int pon){
  double po2=po;
  int pom=0;
  if (pon==0) return 1;
  if (pon<0){ pom=1; pon=-pon; };
  for (int poi=1; poi<pon; poi++){
    po2=po2*po;
  };
  if (pom) return 1/po2;
  return po2;
}

int randInt( int ri1 ){
  return int(drand48()*(ri1+1));
}

int sign( double si1){
  if (!si1) return 0;
  if (si1 < 0) return -1;
  return 1;
}

double sq( double sq1 ){
  return sq1*sq1;
}

double tenpow( int tpower ){
  double tp1=1, tpi;
  if ( tpower >= 1 ){
    for (tpi=0; tpi < tpower; tpi++) tp1=tp1*10;
  };
  if ( tpower < 1 ){
    for (tpi=0; tpi > tpower; tpi=tpi-1) tp1=tp1/10;
  };
  return tp1;
}

int topten( double tt1 ){
  if (tt1 < 0) return -1;
  if (tt1 < 10) return 0;
  int ttj=0;
  int tti=10;
  while (tti <= tt1) tti=tti*10;
  return ttj;
}

int trunc( double t1, int digits){
  int t2=int(t1*tenpow(digits));
  return int(t2/(tenpow(digits)*1.0));
}

/* FUNCTION LIST

  int approxEqual(double, double)
  int brack(double)
  double cut(double, int)
  double dist2D(point2, point2)
  double dist3D(point3, point3)
  double fixAngle(double)
  double max(double, double)
  double min(double, double)
  point2 mpoint2(double, double)
  point3 mpoint3(double, double, double)
  double norm2D(point2)
  double norm3D(point3)
  double power(double, int)
  int randInt(int)
  int sign(double)
  double sq(double)
  double tenpow(double)
  int topten(double)
  int trunc(double)


  MATH.H

  extern double acos __P((double));
  extern double asin __P((double));
  extern double atan __P((double));
  extern double atan2 __P((double, double));
  extern double cos __P((double));
  extern double sin __P((double));
  extern double tan __P((double));

  extern double cosh __P((double));
  extern double sinh __P((double));
  extern double tanh __P((double));

  extern double exp __P((double));
  extern double frexp __P((double, int *));
  extern double ldexp __P((double, int));
  extern double log __P((double));
  extern double log10 __P((double));
  extern double modf __P((double, double *));

  extern double pow __P((double, double));
  extern double sqrt __P((double));

  extern double ceil __P((double));
  extern double fabs __P((double));
  extern double floor __P((double));
  extern double fmod __P((double, double));

  extern double erf __P((double));
  extern double erfc __P((double));
  extern double gamma __P((double));
  extern double hypot __P((double, double));
  extern int isnan __P((double));
  extern double j0 __P((double));
  extern double j1 __P((double));
  extern double jn __P((int, double));
  extern double lgamma __P((double));
  extern double y0 __P((double));
  extern double y1 __P((double));
  extern double yn __P((int, double));

  extern double acosh __P((double));
  extern double asinh __P((double));
  extern double atanh __P((double));
  extern double cbrt __P((double));
  extern double logb __P((double));
  extern double nextafter __P((double, double));
  extern double remainder __P((double, double));
  extern double scalb __P((double, double));

  extern double expm1 __P((double));
  extern int ilogb __P((double));
  extern double log1p __P((double));
  extern double rint __P((double));

  extern int matherr __P((struct exception *));

  extern double significand __P((double));

  extern double copysign __P((double, double));
  extern double scalbn __P((double, int));

  extern double gamma_r __P((double, int *));
  extern double lgamma_r __P((double, int *));

  extern float modff __P((float, float *));

*/