#ifndef GUI_H
#define GUI_H

/*+
 * GUI.h
 *
 * Handles the interaction of the SNPL_bitmap class (and eventually other types of
 * objects) with the SDL graphics library.  Includes functions for manipulating
 * SNPL_bitmap's, drawing to them, and displaying them in various styles.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

#include <SDL/SDL.h> 
#include "bitmap.h"
#include "frame.h"
#include "color.h"
#include "../util/strutil.h"
#include "../snpl.h"

class SNPL_GUI
{
  class SDL_Surface *screen;
  int screen_width, screen_height;
  int scale;

  int pen_i, pen_j;
  class SNPL_color **pos;

  class SNPL_bitmap *g, *stunt_double;

  void flip();
  
public:

  SNPL_GUI(class SDL_Surface *, class SNPL_color *, int, int, int);
  
  SNPL_GUI(class SDL_Surface *, class SNPL_bitmap *, int);
  
  ~SNPL_GUI();

  SNPL_bitmap *copy();
  
  //!### If you keep these, you'll have to put them in
  //     gui.cpp at some point.
  SNPL_bitmap *get_bitmap() { return g; }
  //void set_bitmap(SNPL_bitmap *bp) { g = bp; } 
  class SDL_Surface *get_screen() { return screen; }
  int get_scale() { return scale; }
  
  void embed_center(SNPL_bitmap *);

  void pen_color(SNPL_color *);
  
  class SNPL_color *get_color();

  void pen_move(int, int);

  void cls();

  void ECT();

  void poit(int, int);

  void draw_pixel(SNPL_color *, int, int);

  void draw_horizontal_line(SNPL_color *, int, int, int);

  void draw_vertical_line(SNPL_color *, int, int, int);

  void draw_outline(SNPL_color *);

  void draw_color();

  void draw_bool(SNPL_color *, SNPL_color *);

  void fill_rect(SNPL_color *, int, int, int, int);

  void fill_rect(SNPL_color *, class SNPL_frame *);

  void draw_silhouette(SNPL_color *, SNPL_color *, SNPL_color *);

  void flex(class SNPL_color *, class SNPL_color *);

  void conway(class SNPL_color *);  
  
  //void save(class list *);

  void input_string(char *);
  
};



// Interface functions ##########################################################

SNPL_GUI *new_SNPL_GUI(SDL_Surface *a0, SNPL_color *a1, int *a2, int *a3, int *a4);  
SNPL_bitmap *SNPL_GUI_copy(SNPL_GUI *a0);
void SNPL_GUI_embed_center(SNPL_bitmap *a0, SNPL_GUI *a1);
void SNPL_GUI_pen_color(SNPL_color *a0, SNPL_GUI *a1);
SNPL_color *SNPL_GUI_get_color(SNPL_GUI *a0);
void SNPL_GUI_pen_move(int *a0, int *a1, SNPL_GUI *a2);
void SNPL_GUI_cls(SNPL_GUI *a0);
void SNPL_GUI_ECT(SNPL_GUI *a0);
void SNPL_GUI_poit(int *a0, int *a1, SNPL_GUI *a2);
void SNPL_GUI_draw_outline(SNPL_color *a0, SNPL_GUI *a1);
void SNPL_GUI_draw_color(SNPL_GUI *a0);
void SNPL_GUI_draw_bool(SNPL_color *a0, SNPL_color *a1, SNPL_GUI *a2);
void SNPL_GUI_draw_silhouette(SNPL_color *a0, SNPL_color *a1, SNPL_color *a2, SNPL_GUI *a3);
void SNPL_GUI_flex(class SNPL_color *a0, class SNPL_color *a1, SNPL_GUI *a2);
void SNPL_GUI_conway(class SNPL_color *a0, SNPL_GUI *a1);






#endif

/*- end of file --------------------------------------------------------------*/
