/*
 ******************************************************************************
 * a simple C interface to X windows for Informatik I / Abt.IX /  ETHZ 
 * thomas raschle 11/94 			
 ******************************************************************************
 * to compile with gnu's gcc version 2.5.8
 ******************************************************************************
 ******************************************************************************
 */


#ifndef _INFO_I_GRAPHICS_
#define _INFO_I_GRAPHICS_

#include <X11/Xlib.h>

/*
 ******************************************************************************
 * create / destroy Drawables
 ******************************************************************************
 *
 * a Drawable is a Window or Pixmap
 *   Window --- a visible window on the physical screen 
 *   Pixmap --- a window in the memory
 * use copyArea to copy a Pixmap to a Window and vice versa
 */


Window createWindow( int x, int y, int width, int height, char* name );
/*  (x,h) is the upper left corner on the physical screen  */
void destroyWindow( Window window );
void clearWindow( Window window );


Pixmap createPixmap( Drawable d, int width, int height );
/*  d must be a created Window or Pixmap  */
void destroyPixmap( Pixmap p );


void synchronize( void );
/* stops until all previous calls are executed by the X-Server */


/*
 ******************************************************************************
 * subroutines for Drawable's, e.g. Window's and Pixmap's
 ******************************************************************************
 */


void drawPoint( Drawable d, int x, int y );
void drawLine( Drawable d, int x1, int y1, int x2, int y2 );
void drawString( Drawable d, int x, int y, char* string );

void drawRectangle( Drawable d, int x, int y, int w, int h );
void fillRectangle( Drawable d, int x, int y, int w, int h );
void drawCircle( Drawable d, int x, int y, int rad );
void fillCircle( Drawable d, int x, int y, int rad );
void drawEllipse( Drawable d, int x, int y, int w, int h );
void fillEllipse( Drawable d, int x, int y, int w, int h );

/*
 * copy a rectangular area of size (width, height) 
 *   from src at (src_x, src_y)
 *   to  dest at (dest_x, dest_y)
 */

void copyArea( Drawable src, Drawable dest, 
		int src_x, int src_y, int width, int height, 
		int dest_x, int dest_y);


/*
 ******************************************************************************
 * subroutines to get and set attributes  
 ******************************************************************************
 */


#define MAX_NUM_COLORS 256   	/* maximal number of colors possible      */
int numberOfColors( void );  	/* returns the number of colors available */

int white( void );		/* returns the number for white  	  */	
int black( void ); 		/* returns the number for black  	  */
int red( void );    		/* returns the number for red    	  */
int yellow( void ); 		/* returns the number for yellow 	  */
int green( void );  		/* returns the number for green  	  */
int blue( void );   		/* returns the number for blue   	  */
int pink( void );  		/* returns the number for pink   	  */

int getColor( void );   

void setColor( int color );     	/* default is black               */ 	
void setLineWidth( int w );     	/* default is 1                   */
void setDrawMode( int function ); 	/* function is the draw mode:     */
					/*   GXcopy -- paint (default)    */
 					/*   GXxor  -- xor                */
					/* cf. Xlib.h for more		  */

  
/*
 ******************************************************************************
 * event handling
 ******************************************************************************
 */
    
                                     
typedef struct 
{
  Window window;	/* the window associated with the event  */
  int type;		/* type of event, cf. below		 */
  int x, y;		/* depending on type of event, cf. below */
  int width, height;    /* depending on type of event, cf. below */
  int value;            /* depending on type of event, cf. below */
} Event;

/* the following event types can occur 
 *   Expose        --- (x,y,width,height) gives the exposed window 
 *   ButtonPress   --- value is the mouse button, (x,y) the mouse position
 *   ButtonRelease --- value is the mouse button, (x,y) the mouse position
 *   MotionNotify  --- (x,y) gives the mouse position
 *   KeyPress      --- values is the key value, (x,y) the mouse position
 *   KeyRelease    --- values is the key value, (x,y) the mouse position
 */

Event nextEvent( void );   /* get next event from event queue (blocking)  */
int checkEvent( void );    /* true if an event is available (nonblocking) */


/*
 ******************************************************************************
 * miscellaneous
 ******************************************************************************
 */


void setTimer( unsigned long value, unsigned long interval ); 
/* value    microseconds until the first alarm                           */
/* if interval == 0 --> no further alarms                                */
/* if interval != 0 --> interval microseconds between consecutive alarms */

void waitForAlarm( void );
/* wait for a timer generated alarm */



/*
 ******************************************************************************
 * variables used for Xlib calls, allows direct X access
 ******************************************************************************
 */

extern Display *display;
extern int screen;
extern GC gc;

#endif
