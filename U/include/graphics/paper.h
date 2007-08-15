/*+
 * paper.h
 *
 * Handles the interaction of the "grid" class (and eventually other types of
 * objects) with the SDL graphics library.  Includes functions for manipulating
 * "grids", drawing to them, and displaying them in various styles.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

//# Possibly give the paper class a static universe which is created along with
//  the first instance, and destroyed along with the last (note: instances could
//  go from >0 to =0 to >0 again, in which case you'd have repetitive creation and
//  deletion of universes).  That way you wouldn't have to clutter up your data
//  universe with temporary objects, nor would you have to worry about cleaning
//  up those objects before serialization.
//  Perhaps helper objects such as a "paper" object should have two references,
//  one to a data universe, and one to a "working" universe.

class paper
{
private:

  class universe *work_u;

  SDL_Surface *screen;
  int screen_width, screen_height;
  int scale;

  int pen_i, pen_j;
  class color **pos;

  class grid *g, *stunt_double;

public:

  paper(class universe *, SDL_Surface *, color *, int, int, int);

  ~paper();

  void embed_center(grid *);

  void pen_color(color *);

  void pen_move(int, int);

  void cls();

  void ECT();

  void poit(int, int);

  void draw_pixel(color *, int, int);

  void draw_horizontal_line(color *, int, int, int);

  void draw_vertical_line(color *, int, int, int);

  void draw_outline(color *);

  void draw_color();

  void draw_bool(color *, color *);

  void fill_rect(color *, int, int, int, int);

  void fill_rect(class color *, class frame *);

  void draw_silhouette(color *, color *, color *);

  void save(class list *);

};

/*- end of file --------------------------------------------------------------*/