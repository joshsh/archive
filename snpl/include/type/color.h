#ifndef COLOR_H
#define COLOR_H

/*+
 * color.h
 *
 * A helper class (not user-accessible) to store and compare color values for
 * SDL graphics.
 *
 * author: Josh Shinavier
 * last edited: 9/17/04
 */

#include "../util/strutil.h"

class SNPL_color
{
protected:

  // ba ^ "unsigned char"
  unsigned char color_red, color_green, color_blue;

public:

  SNPL_color();

  SNPL_color(char *);

  SNPL_color(int, int, int);

  ~SNPL_color();

  int red(), green(), blue();

  bool equals(SNPL_color *);

  void decode(char *);

  char *encode();

};



// Interface methods (may need to move to color.cpp)

SNPL_color *new_color(char *);



#endif

/*- end of file --------------------------------------------------------------*/
