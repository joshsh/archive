#ifndef BITMAP_H
#define BITMAP_H

/*+
 * bitmap.h
 *
 * An array of SNPL_color elements, bundled together with some useful or interesting
 * algorithms.
 *
 * author: Josh Shinavier
 * last edited: 9/17/04
 */

#include "frame.h" 
#include "color.h"
#include <cstdio>
 
class SNPL_bitmap
{

  class SNPL_color **bitmap_array;
  class SNPL_color *default_c;
  int bitmap_rows, bitmap_cols;

  void construct();

public:

  SNPL_bitmap(class SNPL_color *, int, int);

  SNPL_bitmap(SNPL_bitmap *, class SNPL_frame *);

  ~SNPL_bitmap();

  int rows();

  int cols();

  //! used by "paper", possible encapsulation issue
  class SNPL_color **get_array();

  class SNPL_color *default_color();

  class SNPL_color *cell(int, int);

  void set(int, int, class SNPL_color *);

  void recursive_set(int, int, class SNPL_color *);

  void copy(SNPL_bitmap *);

  void clear();

  void fill(class SNPL_color *);

  void embed(SNPL_bitmap *, int, int);

  void embed_center(SNPL_bitmap *);

  void flatten(SNPL_bitmap *, class SNPL_color *);

  void flex(SNPL_bitmap *, class SNPL_color *, class SNPL_color *);

  void conway(SNPL_bitmap *, class SNPL_color *);

  class SNPL_frame *crop();

};



#endif

/*- end of file --------------------------------------------------------------*/
