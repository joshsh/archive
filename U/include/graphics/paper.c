/*+
 * paper.c
 *
 * Handles the interaction of the "grid" class (and eventually other types of
 * objects) with the SDL graphics library.  Includes functions for manipulating
 * "grids", drawing to them, and displaying them in various styles.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * paper::paper(universe *work_u0,
             SDL_Surface *screen0,
             color *default_c0,
             int scale0,
             int rows0,
             int cols0)
 *
 *
 */
paper::paper(universe *work_u0,
             SDL_Surface *screen0,
             color *default_c0,
             int scale0,
             int rows0,
             int cols0)
       : work_u(work_u0),
         pen_i(0),
         pen_j(0),
         screen(screen0),
         scale(scale0),
         screen_width(scale*cols0),
         screen_height(scale*rows0),
         stunt_double(NULL),
         g(new grid(work_u, default_c0, rows0, cols0)),
         pos(g->get_array())
{
  #ifdef PAPER_DEBUG
    cout << "<> paper::paper(universe *, SDL_Surface *, color *, int, int, int)" << endl;
    cout.flush();
  #endif
}



/**
 * paper::~paper()
 *
 *
 */
paper::~paper()
{
  #ifdef PAPER_DEBUG
    cout << "<> paper::~paper()" << endl;
    cout.flush();
  #endif

  // Note: array elements are not deleted individually, as they are supposed
  //       to be external.
  //delete g;
  //if (stunt_double != NULL)
  //  delete stunt_double;
  ((item *) g)->remove();
}



/**
 * void paper::embed_center(grid *g2)
 *
 *
 */
/* do we even want a separate function for this? */
void paper::embed_center(grid *g2)
{
  g->embed_center(g2);
  draw_color();
}



/**
 * void paper::pen_color(color *c)
 *
 *
 */
void paper::pen_color(color *c)
{
  *pos = c;
  poit(pen_i,pen_j);
}



/**
 * void paper::pen_move(int di, int dj)
 *
 *
 */
// {Atari}
void paper::pen_move(int di, int dj)
{
  int rows = g->rows();
  int cols = g->cols();
  pen_i = (pen_i+di+rows)%rows;
  pen_j = (pen_j+dj+cols)%cols;
  pos = g->get_array() + pen_i*g->cols() + pen_j;
}



/**
 * void paper::cls()
 *
 *
 */
void paper::cls()
{
  g->clear();
}



/**
 * void paper::ECT()
 *
 *
 */
void paper::ECT()
{
  pen_i = 0; pen_j = 0;
  pos = g->get_array();
  cls();
  draw_color();
}



/**
 * void paper::poit(int i, int j)
 *
 *
 */
void paper::poit(int i, int j)
{
  color *c = g->cell(i, j);
cout << c->encode() << endl;
  fill_rect(c, j*scale, i*scale, scale, scale);
  SDL_Flip(screen);
}



/**
 * void paper::draw_pixel(color *c, int x, int y)
 *
 *
 */
void paper::draw_pixel(color *c, int x, int y)
{
  Uint32 *pixel = (Uint32*)screen->pixels + (screen_width * y) + x;
  *pixel = SDL_MapRGB(screen->format, c->red(), c->green(), c->blue());
}



/**
 * void paper::draw_horizontal_line(color *c, int x, int y, int length)
 *
 *
 */
/** starting on the left and drawing right */
void paper::draw_horizontal_line(color *c, int x, int y, int length)
{
  Uint32 *pixel = (Uint32*)screen->pixels + (screen_width * y) + x;
  Uint32 color32 = SDL_MapRGB(screen->format, c->red(), c->green(), c->blue());
  for (int i=0; i<length; i++) {
    *pixel = color32;
    pixel++;
  }
}



/**
 * void paper::draw_vertical_line(color *c, int x, int y, int length)
 *
 *
 */
/** starting on the top and drawing down */
void paper::draw_vertical_line(color *c, int x, int y, int length)
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
 * void paper::draw_outline(color *c_outline)
 *
 *
 */
// {Atari}
// Function pointers, function pointers...
void paper::draw_outline(color *c_outline)
{
  color *default_c = g->default_color();
  bool n, e, s, w, ne, nw, se, sw;
  color **cp = g->get_array();
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
 * void paper::draw_color()
 *
 *
 */
void paper::draw_color()
{
  color **cp = g->get_array();
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
 * void paper::draw_bool(color *back_c, color *fore_c)
 *
 *
 */
void paper::draw_bool(color *back_c, color *fore_c)
{
  fill_rect(back_c, 0, 0, scale*g->cols(), scale*g->rows());
  color **cp = g->get_array();
  color *default_c = g->default_color();
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
 * void paper::fill_rect(color *c, int x, int y, int w, int h)
 *
 *
 */
// no refresh
void paper::fill_rect(color *c, int x, int y, int w, int h)
{
  SDL_Rect *rect = new SDL_Rect();
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  SDL_FillRect(screen, rect, SDL_MapRGB(screen->format, c->red(), c->green(), c->blue()));
}



/**
 * void paper::fill_rect(color *c, frame *f)
 *
 *
 */
void paper::fill_rect(color *c, frame *f)
{
  SDL_Rect *rect = new SDL_Rect();
  rect->x = f->x();
  rect->y = f->y();
  rect->w = f->width();
  rect->h = f->height();
  SDL_FillRect(screen, rect, SDL_MapRGB(screen->format, c->red(), c->green(), c->blue()));
}



/**
 * void paper::draw_silhouette(color *back_c, color *fore_c, color *line_c)
 *
 *
 */
void paper::draw_silhouette(color *back_c, color *fore_c, color *line_c)
{
  draw_bool(back_c, fore_c);
  draw_outline(line_c);
}



/**
 * void paper::save(list *l)
 * {crop}
 */
void paper::save(list *l)
{
  #ifdef PAPER_DEBUG
    cout << "<> void paper::save(list *)" << endl;
    cout.flush();
  #endif

  frame *f = g->crop();
  grid *g2 = new grid(work_u, g, f);
  l->enqueue((item *) g2);
  ((item *) f)->remove();
  delete f;
}



/*- end of file --------------------------------------------------------------*/
