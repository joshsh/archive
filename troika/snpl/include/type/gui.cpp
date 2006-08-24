/*+
 * GUI.cpp
 *
 * Handles the interaction of the SNPL_bitmap class (and eventually other types of
 * objects) with the SDL graphics library.  Includes functions for manipulating
 * SNPL_bitmap's, drawing to them, and displaying them in various styles.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

#include "gui.h"
#include <iostream> // for debugging
using namespace std;



void SNPL_GUI::flip()
{
  if (stunt_double == NULL) {
    stunt_double = new SNPL_bitmap(g->default_color(), g->rows(), g->cols());
    SNPL_register_item((void *) stunt_double, SNPL_reference_type("bitmap"));
  }
  SNPL_bitmap *temp = stunt_double;
  stunt_double = g;
  g = temp;
}



SNPL_GUI::SNPL_GUI(
             SDL_Surface *screen0,
             SNPL_color *default_c0,
             int scale0,
             int rows0,
             int cols0)
       /*: pen_i(0),
         pen_j(0),
         screen(screen0),
         scale(scale0),
         screen_width(scale*cols0),
         screen_height(scale*rows0),
         stunt_double(NULL),
         g(new SNPL_bitmap(default_c0, rows0, cols0)),
         pos(g->get_array())*/
{
  #ifdef GUI_DEBUG
    cout << "<> SNPL_GUI::SNPL_GUI(SDL_Surface *, SNPL_color *, int, int, int)" << endl;
    cout.flush();
  #endif

//cout << "NEW SNPL_GUI!!!!!!!!!!!!!!!!" << endl;
        pen_i =0;
echo("1");
         pen_j=0;
echo("2");         
	 screen=screen0;
echo("3");         
         scale=scale0;
echo("4");         
         screen_width=scale*cols0;
echo("5");         
         screen_height=scale*rows0;
echo("6");         
         stunt_double=NULL;
echo("7");         
         g=new SNPL_bitmap(default_c0, rows0, cols0);
    SNPL_register_item((void *) g, SNPL_reference_type("bitmap"));
echo("8");         
         pos=g->get_array();
echo("9");
	ECT();   
}



SNPL_GUI::SNPL_GUI(SDL_Surface *screen0, SNPL_bitmap *g0, int scale0)
         : pen_i(0),
         pen_j(0),
         screen(screen0),
         scale(scale0),
         screen_width(scale*g0->cols()),
         screen_height(scale*g0->rows()),
         stunt_double(NULL),
         g(g0),
         pos(g0->get_array())
{
  #ifdef GUI_DEBUG
    cout << "<> SNPL_GUI::SNPL_GUI(SDL_Surface *, SNPL_bitmap *, int)" << endl;
    cout.flush();
  #endif
  
  ECT();  
}



/**
 * SNPL_GUI::~SNPL_GUI()
 *
 *
 */
SNPL_GUI::~SNPL_GUI()
{
  #ifdef GUI_DEBUG
    cout << "<> SNPL_GUI::~SNPL_GUI()" << endl;
    cout.flush();
  #endif

  // Note: array elements are not deleted individually, as they are supposed
  //       to be external.  The two bitmaps are also first-class values, so they
  //       should not be deleted here, either.
  
}



/**
 * SNPL_bitmap *copy()
 * {crop}
 */
SNPL_bitmap *SNPL_GUI::copy()
{
  SNPL_frame *f = g->crop();
  SNPL_bitmap *g2 = new SNPL_bitmap(g, f);
  delete f;
  return g2;
}



/**
 * void SNPL_GUI::embed_center(SNPL_bitmap *g2)
 *
 *
 */
/* do we even want a separate function for this? */
void SNPL_GUI::embed_center(SNPL_bitmap *g2)
{
  g->embed_center(g2);
  draw_color();
}



/**
 * void SNPL_GUI::pen_color(SNPL_color *c)
 *
 *
 */
void SNPL_GUI::pen_color(SNPL_color *c)
{
//cout << "SNPL_GUI::pen_color" << endl; cout.flush();
  *pos = c;
//cout << "so far so good: (" << pen_i << "," << pen_j << ")" << endl; cout.flush();
  poit(pen_i,pen_j);
//cout << "done with ::" << endl; cout.flush();
}



/**
 * SNPL_color *SNPL_GUI::get_color()
 *
 *
 */
SNPL_color *SNPL_GUI::get_color()
{
  return *pos;
}



/**
 * void SNPL_GUI::pen_move(int di, int dj)
 *
 *
 */
// {Atari}
void SNPL_GUI::pen_move(int di, int dj)
{
  int rows = g->rows();
  int cols = g->cols();
  pen_i = (pen_i+di+rows)%rows;
  pen_j = (pen_j+dj+cols)%cols;
  pos = g->get_array() + pen_i*g->cols() + pen_j;
}



/**
 * void SNPL_GUI::cls()
 *
 *
 */
void SNPL_GUI::cls()
{
  g->clear();
}



/**
 * void SNPL_GUI::ECT()
 *
 *
 */
void SNPL_GUI::ECT()
{
  pen_i = 0; pen_j = 0;
  pos = g->get_array();
  cls();
  draw_color();
}



/**
 * void SNPL_GUI::poit(int i, int j)
 *
 *
 */
void SNPL_GUI::poit(int i, int j)
{
//cout << "scale = " << scale << ", screen = " << (int) screen << endl;
  SNPL_color *c = g->cell(i, j);
//cout << "poit(" << i << "," << j << "): " << c->encode() << endl; cout.flush();
  fill_rect(c, j*scale, i*scale, scale, scale);
  SDL_Flip(screen);
}



/**
 * void SNPL_GUI::draw_pixel(SNPL_color *c, int x, int y)
 *
 *
 */
void SNPL_GUI::draw_pixel(SNPL_color *c, int x, int y)
{
  Uint32 *pixel = (Uint32*)screen->pixels + (screen_width * y) + x;
  *pixel = SDL_MapRGB(screen->format, c->red(), c->green(), c->blue());
}



/**
 * void SNPL_GUI::draw_horizontal_line(SNPL_color *c, int x, int y, int length)
 *
 *
 */
/** starting on the left and drawing right */
void SNPL_GUI::draw_horizontal_line(SNPL_color *c, int x, int y, int length)
{
  Uint32 *pixel = (Uint32*)screen->pixels + (screen_width * y) + x;
  Uint32 color32 = SDL_MapRGB(screen->format, c->red(), c->green(), c->blue());
  for (int i=0; i<length; i++) {
    *pixel = color32;
    pixel++;
  }
}



/**
 * void SNPL_GUI::draw_vertical_line(SNPL_color *c, int x, int y, int length)
 *
 *
 */
/** starting on the top and drawing down */
void SNPL_GUI::draw_vertical_line(SNPL_color *c, int x, int y, int length)
{
  int yinc = scale*g->cols();
  Uint32 *pixel = (Uint32*)screen->pixels + (yinc * y) + x;
  Uint32 color32 = SDL_MapRGB(screen->format, c->red(), c->green(), c->blue());
  for (int i=0; i<length; i++) {
    *pixel = color32;
    pixel += yinc;
  }
}



/**
 * void SNPL_GUI::draw_outline(SNPL_color *c_outline)
 *
 *
 */
// {Atari}
// Function pointers, function pointers...
void SNPL_GUI::draw_outline(SNPL_color *c_outline)
{
  SNPL_color *default_c = g->default_color();
  bool n, e, s, w, ne, nw, se, sw;
  SNPL_color **cp = g->get_array();
  for (int i=0; i<g->rows(); i++) {
    for (int j=0; j<g->cols(); j++) {
      if (!default_c->equals(*cp)) {  //### pointer comparison was better...
        n = (!default_c->equals(g->cell(i-1,j)));  //### ...
        s = (!default_c->equals(g->cell(i+1,j)));  //...
        e = (!default_c->equals(g->cell(i,j+1)));
        w = (!default_c->equals(g->cell(i,j-1)));
        ne = (!default_c->equals(g->cell(i-1,j+1)));
        nw = (!default_c->equals(g->cell(i-1,j-1)));
        se = (!default_c->equals(g->cell(i+1,j+1)));
        sw = (!default_c->equals(g->cell(i+1,j-1)));
        if (!n)
          draw_horizontal_line(c_outline, (j*scale), (i*scale), scale);
        if (!w)
          draw_vertical_line(c_outline, (j*scale), (i*scale), scale);
        if (!s)
          draw_horizontal_line(c_outline, (j*scale), ((i+1)*scale)-1, scale);
        if (!e)
          draw_vertical_line(c_outline, ((j+1)*scale)-1, (i*scale), scale);
        if (n && w && !nw)
          draw_pixel(c_outline, (j*scale), (i*scale));
        if (n && e && !ne)
          draw_pixel(c_outline, ((j+1)*scale)-1, (i*scale));
        if (s && w && !sw)
          draw_pixel(c_outline, (j*scale), ((i+1)*scale)-1);
        if (s && e && !se)
          draw_pixel(c_outline, ((j+1)*scale)-1, ((i+1)*scale)-1);
      }
      cp++;
    }
  }
  SDL_Flip(screen);
}



/**
 * void SNPL_GUI::draw_color()
 *
 *
 */
void SNPL_GUI::draw_color()
{
  SNPL_color **cp = g->get_array();
  int x=0, y=0;
  for (int i=0; i<g->rows(); i++) {
    for (int j=0; j<g->cols(); j++) {
      fill_rect(*cp, x, y, scale, scale);
      cp++;
      x += scale;
    }
    y += scale;
    x = 0;
  }
  SDL_Flip(screen);
}



/**
 * void SNPL_GUI::draw_bool(SNPL_color *back_c, SNPL_color *fore_c)
 *
 *
 */
void SNPL_GUI::draw_bool(SNPL_color *back_c, SNPL_color *fore_c)
{
  fill_rect(back_c, 0, 0, scale*g->cols(), scale*g->rows());
  SNPL_color **cp = g->get_array();
  SNPL_color *default_c = g->default_color();
  int x=0, y=0;
  for (int i=0; i<g->rows(); i++) {
    for (int j=0; j<g->cols(); j++) {
      if (!default_c->equals(*cp))  //### pointer comparison was better...
        fill_rect(fore_c, x, y, scale, scale);
      cp++;
      x += scale;
    }
    y += scale;
    x = 0;
  }
  SDL_Flip(screen);
}



/**
 * void SNPL_GUI::fill_rect(SNPL_color *c, int x, int y, int w, int h)
 *
 *
 */
// no refresh
void SNPL_GUI::fill_rect(SNPL_color *c, int x, int y, int w, int h)
{
  SDL_Rect *rect = new SDL_Rect();
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  SDL_FillRect(screen, rect, SDL_MapRGB(screen->format, c->red(), c->green(), c->blue()));
}



/**
 * void SNPL_GUI::fill_rect(SNPL_color *c, SNPL_frame *f)
 *
 *
 */
void SNPL_GUI::fill_rect(SNPL_color *c, SNPL_frame *f)
{
  SDL_Rect *rect = new SDL_Rect();
  rect->x = f->x();
  rect->y = f->y();
  rect->w = f->width();
  rect->h = f->height();
  SDL_FillRect(screen, rect, SDL_MapRGB(screen->format, c->red(), c->green(), c->blue()));
}



/**
 * void SNPL_GUI::draw_silhouette(SNPL_color *back_c, SNPL_color *fore_c, SNPL_color *line_c)
 *
 *
 */
void SNPL_GUI::draw_silhouette(SNPL_color *back_c, SNPL_color *fore_c, SNPL_color *line_c)
{
  draw_bool(back_c, fore_c);
  draw_outline(line_c);
}



void SNPL_GUI::flex(SNPL_color *dest_c, SNPL_color *source_c)
{
  flip();
  g->flex(stunt_double, dest_c, source_c);
}



void SNPL_GUI::conway(SNPL_color *fore_c)
{
  flip();
  g->conway(stunt_double, fore_c);
}



// temporary
void SNPL_GUI::input_string(char *buf)
{
  char *cur = buf;
  bool loop = true;
  
  while(loop)  // break out on ESC or ENTER
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      char ch = '\0';
      
      /* quit on SDL_QUIT */ 
      if ( event.type == SDL_QUIT )  
      { 
        //pl->save_to_file("data/patterns2");
        loop = false; 
      }

      if ( event.type == SDL_KEYDOWN )
      {
        ch = (char) event.key.keysym.sym;
//cout << "(" << (int) ch << ")" << endl;
//cout.flush();
	if (ch == 13) {
	  loop = false;
	  //cout << "DONE" << endl;
	  cout << endl;
	  cout.flush();
	  ch = '\0';
	}
	// Note: no tab characters
        //else if (((int) ch < 32)||((int) ch > 127))
	//  ch = '\0';
      }
      if (ch) {
        *cur = ch;
	cout << ch;// << "|";
	cout.flush();
	cur++;
      }
    }
  }
  *cur = '\0';
}



// Interface functions ##########################################################

SNPL_GUI *new_SNPL_GUI(SDL_Surface *a0, SNPL_color *a1, int *a2, int *a3, int *a4)
{
//cout << "new_SNPL_GUI" << endl; cout.flush();
  return new SNPL_GUI(a0, a1, *a2, *a3, *a4);
}
  
SNPL_bitmap *SNPL_GUI_copy(SNPL_GUI *a0)
{
  return a0->copy();
}
  
void SNPL_GUI_embed_center(SNPL_bitmap *a0, SNPL_GUI *a1)
{
  a1->embed_center(a0);
}

void SNPL_GUI_pen_color(SNPL_color *a0, SNPL_GUI *a1)
{
//cout << "SNPL_GUI_pen_color: " << (int) a0 << ", " << (int) a1 << endl; cout.flush();
//cout << a0->encode() << endl; cout.flush();
  a1->pen_color(a0);
//cout << "done." << endl; cout.flush();
}

SNPL_color *SNPL_GUI_get_color(SNPL_GUI *a0)
{
  return a0->get_color();
}

void SNPL_GUI_pen_move(int *a0, int *a1, SNPL_GUI *a2)
{
  a2->pen_move(*a0, *a1);
}

void SNPL_GUI_cls(SNPL_GUI *a0)
{
  a0->cls();
}

void SNPL_GUI_ECT(SNPL_GUI *a0)
{
  a0->ECT();
}

void SNPL_GUI_poit(int *a0, int *a1, SNPL_GUI *a2)
{
  a2->poit(*a0, *a1);
}

  //void SNPL_GUI_draw_pixel(SNPL_color *, int, int);

  //void SNPL_GUI_draw_horizontal_line(SNPL_color *, int, int, int);

  //void SNPL_GUI_draw_vertical_line(SNPL_color *, int, int, int);

void SNPL_GUI_draw_outline(SNPL_color *a0, SNPL_GUI *a1)
{
  a1->draw_outline(a0);
}

void SNPL_GUI_draw_color(SNPL_GUI *a0)
{
  a0->draw_color();
}

void SNPL_GUI_draw_bool(SNPL_color *a0, SNPL_color *a1, SNPL_GUI *a2)
{
  a2->draw_bool(a0, a1);
}

//void SNPL_GUI_fill_rect(SNPL_color *, int, int, int, int);

//void SNPL_GUI_fill_rect(SNPL_color *, class SNPL_frame *);

void SNPL_GUI_draw_silhouette(SNPL_color *a0, SNPL_color *a1, SNPL_color *a2, SNPL_GUI *a3)
{
  a3->draw_silhouette(a0, a1, a2);
}

void SNPL_GUI_flex(class SNPL_color *a0, class SNPL_color *a1, SNPL_GUI *a2)
{
  a2->flex(a0, a1);
}

void SNPL_GUI_conway(class SNPL_color *a0, SNPL_GUI *a1)
{
  a1->conway(a0);
}  
  
//void save(class list *);

//void SNPL_GUI_input_string(char *);



/*- end of file --------------------------------------------------------------*/
