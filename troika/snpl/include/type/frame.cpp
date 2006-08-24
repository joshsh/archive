/*+
 * frame.cpp
 *
 * Defines a region of a bitmap. Useful when "tokenizing" a bitmap, and whenever
 * it's important to isolate or keep track of different elements within a grid.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

#include "frame.h"
using namespace std;


/**
 * SNPL_frame::SNPL_frame(int x0, int y0, int width0, int height0)
 *
 *
 */
SNPL_frame::SNPL_frame(int x0, int y0, int width0, int height0)
       : frame_x(x0),
         frame_y(y0),
         frame_width(width0),
         frame_height(height0)
{
  #ifdef FRAME_DEBUG
    cout << "<> SNPL_frame::SNPL_frame(int, int, int, int)" << endl;
    cout.flush();
  #endif
}



/**
 * SNPL_frame::~SNPL_frame()
 *
 *
 */
SNPL_frame::~SNPL_frame()
{
  #ifdef FRAME_DEBUG
    cout << "<> SNPL_frame::~SNPL_frame()" << endl;
    cout.flush();
  #endif
}



/**
 * int SNPL_frame::x()
 *
 * Return the SNPL_frame's leftmost x coordinate.
 */
int SNPL_frame::x()
{
  return frame_x;
}



/**
 * int SNPL_frame::y()
 *
 * Return the SNPL_frame's uppermost y coordinate.
 */
int SNPL_frame::y()
{
  return frame_y;
}



/**
 * int SNPL_frame::width()
 *
 * Return the SNPL_frame's horizontal dimension.
 */
int SNPL_frame::width()
{
  return frame_width;
}



/**
 * int SNPL_frame::height()
 *
 * Return the SNPL_frame's vertical dimension.
 */
int SNPL_frame::height()
{
  return frame_height;
}



/*- end of file --------------------------------------------------------------*/
