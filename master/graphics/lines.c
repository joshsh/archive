#include <graphics.h>
#include <stdlib.h>

const int N = 2;
const int T = 200;
const int Width = 500;
const int Height = 400;
const double PI = 3.1416;
const double d = 10.0;


struct vector 
{
  double x, y; 
};


void nextPosition( vector& p, vector&v )
{
  p.x = p.x + d*v.x;
  if ((p.x<0) || (p.x>Width))
    v.x = - v.x;
  p.y = p.y + d*v.y;
  if ((p.y<0) || (p.y>Height))
    v.y = - v.y;
}

void drawAndCalculate( Window w, vector p[N], vector v[N], int& color )
{
  setColor(color++);
  color = color%numberOfColors();
  drawLine(w, (int)p[0].x, (int)p[0].y, (int)p[1].x, (int)p[1].y);
  nextPosition( p[0], v[0] );
  nextPosition( p[1], v[1] );
}

int main()
{
  vector p1[N], p2[N];
  vector v1[N], v2[N];
  int color1, color2;
 
  int i, j;

  for (i = 0; i<N; i++)
  {
    double phi = 2*PI*drand48();
    v1[i].x = sin(phi);   
    v1[i].y = cos(phi);   
    p1[i].x = drand48()*Width;
    p1[i].y = drand48()*Height;
    v2 = v1; 
    p2 = p1;
  }

  Window w = createWindow( 20, 20, Width, Height, "Animation" );

  setDrawMode( GXxor );

  setTimer(100000,100000);

  color1 = color2 = 0;

  for (j=0; j<T; j++)
  {
    waitForAlarm();
    drawAndCalculate( w, p1, v1, color1 );
  }

  while (1)
  {
    waitForAlarm();
    drawAndCalculate( w, p2, v2, color2 );
    drawAndCalculate( w, p1, v1, color1 );
  }
}

