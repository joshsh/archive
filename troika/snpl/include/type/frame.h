#ifndef FRAME_H
#define FRAME_H

/*+
 * frame.h
 *
 * Defines a region of a bitmap. Useful when "tokenizing" a bitmap, and whenever
 * it's important to isolate or keep track of different elements within a grid.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

class SNPL_frame
{
  int frame_x, frame_y, frame_width, frame_height;

public:

  SNPL_frame(int);

  SNPL_frame(int, int, int, int);

  ~SNPL_frame();

  int x();

  int y();

  int width();

  int height();

};



#endif

/*- end of file --------------------------------------------------------------*/
