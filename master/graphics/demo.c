/* Demo shows how to use C++ graphicsWindows		*/
/* thomas raschle 10/94 				*/

#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

void draw_demo( Pixmap p );

int main()
{ 
  Event e;
  Window w = createWindow(20,20,600,550,"Info I Graphic Demo");
  Pixmap p = createPixmap(w,600,550);

  draw_demo( p );  // draw everything on pixmap

  do {
    e = nextEvent(); 
    if ( e.type == Expose)  // if expose then redraw
      copyArea( p, w, 0, 0, 600, 550, 0, 0); 
  } while ( e.type != ButtonPress );   

  for (int i=0; i<600; i=i+5) // do animation
  {
    copyArea( p, w, 0, 0, 600-i, 550, i, 0 );
    copyArea( p, w, 600-i, 0, i, 550, 0, 0 );
  };  

  synchronize();  // don't stop program unless all copy's are executed  

}


void draw_demo( Drawable p )
{
  int i,j;
  for (i=0; i<=180; i = i+6 )
  {
    drawLine( p,  10,  10, 190-i, 10+i );
    drawLine( p, 190, 190, 190-i, 10+i );
    drawLine( p,  10, 190,  10+i, 10+i );
    drawLine( p, 190,  10,  10+i, 10+i );
  }
  drawString( p, 65, 210, "drawLine" );

  double   x = 300;
  double   y = 100;
  double phi =   0;

  for (i=90; i>0; i = i-3)
  {
    drawCircle( p, (int)x, (int)y, i);
    x = x+3*sin(phi);
    y = y+3*cos(phi); 
    phi = phi + 0.12;
  }
  drawString( p, 265, 210, "drawCircle" );


  for (i=12; i<=90; i= i+6 )
  {
    drawEllipse( p, 500, 100, 90, i );
    drawEllipse( p, 500, 100, i, 90 );
  }
  drawString( p, 465, 210,"drawEllipse" );


  for (i=0; i<4000; i++)
  {
    drawPoint( p, 10+rand() % 181, 230+rand() % 181 );
  }
  drawString( p, 65, 430, "drawPoint" );

  for (i=0; i<50; i++ )
  {
    int w = rand() % 181;
    int h = rand() % 181;
    drawRectangle( p, 210 + rand() % (181-w), 230 + rand() % (181-h), w, h);
  }
  drawString( p, 265, 430, "drawRectangle" );

  for (i=0; i<32; i++)
    for (j=0; j<32; j++)
      if ((i & j) == 0)
      {
        fillCircle( p, 421+5*i, 241+5*j, 3 );
        fillCircle( p, 579-5*i, 399-5*j, 3 );
      } 
  drawString( p, 470, 430, "filledCircles" );

  for (i=0; i<2*numberOfColors(); i++)
  {
    setColor(i/2);
    drawLine( p, 51+i,450,51+i,490);
  }
  drawRectangle(p, 50, 450, 2*numberOfColors()+1, 40);
  drawString( p, 275, 510, "colors" );
  setColor( black() );
  drawString( p, 215, 530, "Press mouse button to quit!" );
}

 
