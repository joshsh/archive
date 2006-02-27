/*
 ******************************************************************************
 * a simple C interface to X windows for Informatik I / Abt.IX /  ETHZ 
 * thomas raschle 11/94 			
 ******************************************************************************
 * compiler: gcc version 
 ******************************************************************************
 * updates:
 *    16.1.96 --- color allocation for low resolution graphic cards added
 ******************************************************************************
*/

#include <graphics.h>

#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include <string.h>

Display *display = 0;
int screen;
GC gc;


/*
 ******************************************************************************
 * initializer, connect to X server
 ******************************************************************************
 */


static int initialized = 0;
void initialize_colors( void );

int graphicsXIOErrorHandler( Display *display ){ }

void initGraphics( void )
{
  char* display_name = 0;

  if ( (display=XOpenDisplay(display_name)) == NULL )
  {
    fprintf(stderr,"graphics: cannot connect to X server %s\\n",   
            XDisplayName(display_name));
    exit(-1);
  }

  XSetIOErrorHandler( graphicsXIOErrorHandler );

  screen = DefaultScreen(display);
  gc = DefaultGC(display, screen );

  initialize_colors( );  

  setLineWidth( 1 );
  setColor( black() );
  setDrawMode( GXcopy );
}


/*
 ******************************************************************************
 * create / destroy Drawables
 ******************************************************************************
 */


#include "graph_icon"
 

static XEvent event;


Window createWindow( int x, int y, int width, int height, char* name )
{  
   Window window;
   XSizeHints size_hints;
   char **argv;
   int argc = 0;
   Pixmap icon_pixmap;
   char *win_name, *p;

   if (!initialized) 
   {
     initialized = 1;
     initGraphics();
   }

   p = win_name = (char*) malloc(128);
   while (*name!=0) *(p++)=*(name++);
   *p=0;

   window = XCreateSimpleWindow( display, RootWindow(display,screen), x, y,
 	    	   		 width, height, 4, BlackPixel(display,screen),
	    		         WhitePixel(display,screen));

   size_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
   size_hints.x = x;
   size_hints.y = y;
   size_hints.width = width;
   size_hints.height = height;
   size_hints.min_width = width;
   size_hints.min_height = height;
   size_hints.max_width = width;
   size_hints.max_height = height;

   icon_pixmap = XCreateBitmapFromData( display, window, graph_icon_bits, 
			                graph_icon_width, graph_icon_height );

   XSetStandardProperties( display, window, win_name, "JJS", 
           	           icon_pixmap, argv, argc, &size_hints);

   XSelectInput( display, window, KeyPressMask    | KeyReleaseMask |
			          PointerMotionMask  | 
                                  ButtonPressMask | ButtonReleaseMask  |
                                  ExposureMask ); 
   XMapWindow(display, window);

   while (!XCheckTypedWindowEvent(display,window,Expose,&event));
   XPutBackEvent(display,&event);

   return window; 
}


void destroyWindow( Window window )
{  XDestroyWindow( display, window ); }


void clearWindow( Window window )
{  XClearWindow( display, window); 
   XFlush(display);  }
 

Pixmap createPixmap( Drawable d, int width, int height )
{  Pixmap p;
   int c;
   p = XCreatePixmap( display, d, width, height, DefaultDepth(display, screen));
   c = getColor();
   setColor( white() );
   XFillRectangle( display, p, gc, 0, 0, width, height );
   setColor( c );
   return p; }


void destroyPixmap( Pixmap p )
{  XFreePixmap( display, p ); }
 

void synchronize( void )
{  XSync( display, 0 );  }


/*
 ******************************************************************************
 * subroutines for Drawable's, e.g. Window's and Pixmap's
 ******************************************************************************
 */


void drawPoint( Drawable d, int x, int y )
{  XDrawPoint( display, d, gc, x, y);
   XFlush(display);  }


void drawLine( Drawable d, int x1, int y1, int x2, int y2 )
{  XDrawLine( display, d, gc, x1, y1, x2, y2 );
   XFlush(display);  } 


void drawString( Drawable d, int x, int y, char* string )
{
  XDrawString(display, d, gc, x, y, string, strlen(string));
  XFlush(display);
}


void drawRectangle( Drawable d, int x, int y, int w, int h )
{  XDrawRectangle( display, d, gc, x, y, w, h );
   XFlush(display);  }


void fillRectangle( Drawable d, int x, int y, int w, int h )
{  XFillRectangle( display, d, gc, x, y, w, h);
   XFlush(display);  }


void drawCircle( Drawable d, int x, int y, int rad )
{  XDrawArc(display, d, gc, x-rad, y-rad, 2*rad+1, 2*rad+1, 0, 360*64);
   XFlush(display);  }


void fillCircle( Drawable d, int x, int y, int rad )
{  XFillArc(display, d, gc, x-rad, y-rad, 2*rad+1, 2*rad+1, 0, 360*64);
   XFlush(display);  }


void drawEllipse( Drawable d, int x, int y, int w, int h )
{  XDrawArc(display, d, gc, x-w, y-h, 2*w+1, 2*h+1, 0, 360*64);
   XFlush(display);  }


void fillEllipse( Drawable d, int x, int y, int w, int h )
{  XFillArc( display, d, gc, x-w, y-h, 2*w+1, 2*h+1, 0, 360*64);
   XFlush(display);  }


void copyArea( Drawable src, Drawable dest, 
		int src_x, int src_y, int width, int height, 
		int dest_x, int dest_y)
{  
   XCopyArea(display,src,dest,gc,src_x,src_y,width,height,dest_x,dest_y);
   XFlush(display);  }


/*
 ******************************************************************************
 * subroutines to get and set attributes  
 ******************************************************************************
 */


static unsigned long colors[MAX_NUM_COLORS+2];
static Colormap cmap;


static int NUM_COLORS=0;
static int WHITE=0, BLACK=0, RED=0, YELLOW=0;
static int GREEN=0, LIGHTGREEN=0, BLUE=0, PINK=0;
static int cur_color=0;


int numberOfColors( void ) {  return NUM_COLORS;  }


int white( void )  {  return WHITE;  }
int black( void )  {  return BLACK;  }
int red( void )    {  return RED;    }
int yellow( void ) {  return YELLOW; }
int green( void )  {  return GREEN;  }
int blue( void )   {  return BLUE;   }
int pink( void )   {  return PINK;   }


int getColor( void )
{  return cur_color;  }


void setColor( int color ) 
{  cur_color = color % (NUM_COLORS+2);
   XSetForeground(display, gc, colors[cur_color]);  }


void setLineWidth( int w )
{  XSetLineAttributes(display, gc, w, LineSolid, CapButt, JoinMiter);  }


void setDrawMode( int function )
{  XSetFunction( display, gc, function );  }


/*
 ******************************************************************************
 * event handling
 ******************************************************************************
 */
  

static KeySym keysym;
static XComposeStatus status;
static char ch;
Event nextEvent( void )
{
  Event e;
  XNextEvent( display, &event );
  e.window = event.xany.window;
  e.type = event.type;
  switch (event.type) {
    case Expose:   
      e.x = event.xexpose.x;
      e.y = event.xexpose.y;
      e.width = event.xexpose.width;
      e.height = event.xexpose.height;
      break;
    case ButtonPress:
      e.x = event.xbutton.x;
      e.y = event.xbutton.y;
      e.value = event.xbutton.button;
      break;
    case ButtonRelease: 
      e.x = event.xbutton.x;
      e.y = event.xbutton.y;
      e.value = event.xbutton.button;
      break;
    case MotionNotify: 
      e.x = event.xmotion.x;
      e.y = event.xmotion.y;
      break;
    case KeyPress: 
      ch = 0;
      XLookupString((XKeyEvent*)&event,&ch,1, &keysym, &status);
      e.x = event.xkey.x;
      e.y = event.xkey.y;
      e.value = ch;
      break;
    case KeyRelease: 
      ch = 0;
      XLookupString((XKeyEvent*)&event,&ch,1, &keysym, &status);
      e.x = event.xkey.x;
      e.y = event.xkey.y;
      e.value = ch;
      break;
    };
  return e;
 }

int checkEvent( void ) 
{ 
  if (XCheckMaskEvent( display, 
                       KeyPressMask    | KeyReleaseMask  | 
                       PointerMotionMask |
                       ButtonPressMask | ButtonReleaseMask  |
                       ExposureMask , &event) ) 
  {
    XPutBackEvent(display,&event);
    return 1;
  }
  else 
    return 0;
}



/*
 ******************************************************************************
 * miscellaneous
 ******************************************************************************
 */

int initTimer = 0;

void timerHandler( int ) { signal( SIGALRM, timerHandler ); }  

void setTimer( unsigned long value, unsigned long interval ) 
{
 
  if (!initTimer)
  {
    initTimer = 1;
    signal( SIGALRM, timerHandler );
  }

  itimerval v;

  v.it_interval.tv_sec  = interval / 1000000;
  v.it_interval.tv_usec = interval % 1000000;

  v.it_value.tv_sec  = value / 1000000;
  v.it_value.tv_usec = value % 1000000;

  setitimer( ITIMER_REAL, &v, 0); 
}

void waitForAlarm( void ) { pause(); }


/*
 ******************************************************************************
 * initialize colors, called by initGraphics()
 ******************************************************************************
 */


void interpol( int li, int hi,
               int lr, int hr, 
               int lg, int hg, 
               int lb, int hb )
{
  int i;
  XColor xcolor;

  for ( i = li; i < hi ; i++ )
  {
    xcolor.pixel = colors[i];
   
    XQueryColor(display, cmap, &xcolor);
    xcolor.red   = (int) ((double)(hr-lr)/(double)(hi-li) * (i-li) + lr);
    xcolor.green = (int) ((double)(hg-lg)/(double)(hi-li) * (i-li) + lg);
    xcolor.blue  = (int) ((double)(hb-lb)/(double)(hi-li) * (i-li) + lb);
    XStoreColor ( display, cmap, &xcolor );
  }
}


int poor_interpol( int li, int hi,
                   int lr, int hr, 
                   int lg, int hg, 
                   int lb, int hb )
{
  int i, succ = 0, first = ( NUM_COLORS - 1 > 0 ) ? NUM_COLORS - 1 : 0 ;
  XColor xcolor;

  for ( i = li; i < hi ; i++ )
  {
    xcolor.red   = (int) ((double)(hr-lr)/(double)(hi-li) * (i-li) + lr);
    xcolor.green = (int) ((double)(hg-lg)/(double)(hi-li) * (i-li) + lg);
    xcolor.blue  = (int) ((double)(hb-lb)/(double)(hi-li) * (i-li) + lb);
    if ( XAllocColor( display, cmap, &xcolor) ) {
      if ( !succ )
        first = NUM_COLORS;
      colors[NUM_COLORS++] = xcolor.pixel;
    } 
  }
}

void initialize_colors( void )
{
  int MAX = 65535;
  int MIN = 0;
  unsigned long plane_masks[1]; 

  cmap = DefaultColormap(display, screen);

  for (NUM_COLORS = MAX_NUM_COLORS; NUM_COLORS > 0; NUM_COLORS--)
    if (XAllocColorCells (display, cmap, False, plane_masks, 
                          0, colors, NUM_COLORS))
    break; 

  if ( NUM_COLORS >= 8 ) {

    NUM_COLORS = NUM_COLORS - 2;
    RED = 0;
    YELLOW = (NUM_COLORS)/6;
    GREEN =  (2*NUM_COLORS)/6;
    LIGHTGREEN = (3*NUM_COLORS)/6;
    BLUE = (4*NUM_COLORS)/6;
    PINK = (5*NUM_COLORS)/6;
    WHITE = NUM_COLORS;
    BLACK = NUM_COLORS+1;

    interpol(RED,YELLOW,MAX,MAX,MIN,MAX,MIN,MIN);
    interpol(YELLOW,GREEN,MAX,MIN,MAX,MAX,MIN,MIN); 
    interpol(GREEN,LIGHTGREEN,MIN,MIN,MAX,MAX,MIN,MAX);
    interpol(LIGHTGREEN,BLUE,MIN,MIN,MAX,MIN,MAX,MAX);
    interpol(BLUE,PINK,MIN,MAX,MIN,MIN,MAX,MAX);
    interpol(PINK,NUM_COLORS,MAX,MAX,MIN,MIN,MAX,MIN);
    interpol(WHITE,BLACK,MAX,MAX,MAX,MAX,MAX,MAX);
    interpol(BLACK,NUM_COLORS+2,MIN,MIN,MIN,MIN,MIN,MIN);

    XFreeColors( display, cmap, colors, NUM_COLORS, 0); 
  }
  else {

    int T_N_C = NUM_COLORS = DisplayCells( display, screen ) - 2;
    

    RED = 0;
    YELLOW = (NUM_COLORS)/6;
    GREEN =  (2*NUM_COLORS)/6;
    LIGHTGREEN = (3*NUM_COLORS)/6;
    BLUE = (4*NUM_COLORS)/6;
    PINK = (5*NUM_COLORS)/6;
    WHITE = NUM_COLORS;
    BLACK = NUM_COLORS+1;

    NUM_COLORS = 0;

    RED = poor_interpol(RED,YELLOW,MAX,MAX,MIN,MAX,MIN,MIN);
    YELLOW = poor_interpol(YELLOW,GREEN,MAX,MIN,MAX,MAX,MIN,MIN); 
    GREEN = poor_interpol(GREEN,LIGHTGREEN,MIN,MIN,MAX,MAX,MIN,MAX);
    LIGHTGREEN = poor_interpol(LIGHTGREEN,BLUE,MIN,MIN,MAX,MIN,MAX,MAX);
    BLUE = poor_interpol(BLUE,PINK,MIN,MAX,MIN,MIN,MAX,MAX);
    PINK = poor_interpol(PINK,T_N_C,MAX,MAX,MIN,MIN,MAX,MIN);

    WHITE = NUM_COLORS;
    BLACK = NUM_COLORS+1;

    WHITE = poor_interpol(WHITE,BLACK,MAX,MAX,MAX,MAX,MAX,MAX);
    BLACK = poor_interpol(BLACK,NUM_COLORS+2,MIN,MIN,MIN,MIN,MIN,MIN);

  }
}
