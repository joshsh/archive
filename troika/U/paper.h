/*+
 * class paper
 *
 * Handles the interaction of the "grid" class (and eventually other types of
 * objects) with the SDL graphics library.  Includes functions for manipulating
 * "grids", drawing to them, and displaying them in various styles.
 *
 *//*#########################################################################*/

class paper
{
private:
  grid *stunt_double;

public:

  SDL_Surface *screen;
  int scale;
  grid *g;

  ~paper()
  {
    // Note: array elements are not deleted individually, as they are supposed
    //       to be external.
    delete g;
    if (stunt_double != NULL)
      delete stunt_double;
  }

  paper(SDL_Surface *screen0, color *default_c0, int scale0, int rows0, int cols0)
  {
    screen = screen0;
    scale = scale0;
    stunt_double = NULL;
    g = new grid(default_c0, rows0, cols0);
  }

  /* Not used... */
  paper(SDL_Surface *screen0, int scale0, char *filename)
  {
    screen = screen0;
    scale = scale0;
    stunt_double = NULL;
    g = new grid(filename);
  }

  /* do we even want a separate function for this? */
  void embed_center(grid *g2)
  {
    g->embed_center(g2);
    draw_color();
  }

  void pen_color(color *c)
  {
    *pos = c;
    poit(pen_i,pen_j);
  }

  // {Atari}
  void pen_move(int di, int dj)
  {
    pen_i = (pen_i+di+rows)%rows;
    pen_j = (pen_j+dj+cols)%cols;
    pos = array + pen_i*cols + pen_j;
  }

  void cls() {
    color **cp = array;
    int n = rows*cols;
    for (int i=0; i<n; i++) {
      *cp = default_c;
      cp++;
    }
  }

  void ECT() {
    pen_i = 0; pen_j = 0;
    pos = array;
    cls();
    draw_color();
  }

  void poit(int i, int j)
  {
    color *c = array[(i*cols)+j];
    fill_rect(c, j*scale, i*scale, scale, scale);
    SDL_Flip(screen);
  }

  void draw_pixel(color *c, int x, int y) {
    Uint32 *pixel = (Uint32*)screen->pixels + (scale * cols * y) + x;
    *pixel = SDL_MapRGB(screen->format, c->red, c->green, c->blue);
  }

  /** starting on the left and drawing right */
  void draw_horizontal_line(color *c, int x, int y, int length) {
    Uint32 *pixel = (Uint32*)screen->pixels + (scale * cols * y) + x;
    Uint32 color32 = SDL_MapRGB(screen->format, c->red, c->green, c->blue);
    for (int i=0; i<length; i++) {
      *pixel = color32;
      pixel++;
    }
  }

  /** starting on the top and drawing down */
  void draw_vertical_line(color *c, int x, int y, int length) {
    int yinc = scale*cols;
    Uint32 *pixel = (Uint32*)screen->pixels + (yinc * y) + x;
    Uint32 color32 = SDL_MapRGB(screen->format, c->red, c->green, c->blue);
    for (int i=0; i<length; i++) {
      *pixel = color32;
      pixel += yinc;
    }
  }

  color *wrap(int i, int j) {
    i = (i+rows)%rows;
    j = (j+cols)%cols;
    return array[(i*cols)+j];
  }

  //! Note: space-wasty.
  int imin, imax, jmin, jmax;
  color *dest_c, *source_c;

  // {Atari}
  void recursive_set(int i, int j)
  {
    i = (i+rows)%rows;
    j = (j+rows)%rows;
    color **cp = array + ((i*cols) + j);
    if (source_c->equals(*cp)) {
      *cp = dest_c;
      if (i<imin) imin = i;
      if (i>imax) imax = i;
      if (j<jmin) jmin = j;
      if (j>jmax) jmax = j;
      recursive_set(i,j-1);
      recursive_set(i,j+1);
      recursive_set(i-1,j);
      recursive_set(i+1,j);
      recursive_set(i-1,j-1);
      recursive_set(i-1,j+1);
      recursive_set(i+1,j-1);
      recursive_set(i+1,j+1);
    }
  }

  // {Atari}
  // Function pointers, function pointers...
  void draw_outline(color *c_outline)
  {
    bool n, e, s, w, ne, nw, se, sw;
    color **cp = array;
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
        if (!default_c->equals(*cp)) {  //### pointer comparison was better...
          n = (!default_c->equals(wrap(i-1,j)));  //### ...
          s = (!default_c->equals(wrap(i+1,j)));  //...
          e = (!default_c->equals(wrap(i,j+1)));
          w = (!default_c->equals(wrap(i,j-1)));
          ne = (!default_c->equals(wrap(i-1,j+1)));
          nw = (!default_c->equals(wrap(i-1,j-1)));
          se = (!default_c->equals(wrap(i+1,j+1)));
          sw = (!default_c->equals(wrap(i+1,j-1)));
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

  void draw_color()
  {
    color **cp = array;
    int x=0, y=0;
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
        fill_rect(*cp, x, y, scale, scale);
        cp++;
        x += scale;
      }
      y += scale;
      x = 0;
    }
    SDL_Flip(screen);
  }

  void draw_bool(color *back_c, color *fore_c)
  {
    fill_rect(back_c, 0, 0, scale*cols, scale*rows);
    color **cp = array;
    int x=0, y=0;
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
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

  // no refresh
  void fill_rect(color *c, int x, int y, int w, int h)
  {
    SDL_Rect *rect = new SDL_Rect();
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    SDL_FillRect(screen, rect, SDL_MapRGB(screen->format, c->red, c->green, c->blue));
  }

  void draw_silhouette(color *back_c, color *fore_c, color *line_c)
  {
    draw_bool(back_c, fore_c);
    draw_outline(line_c);
  }

  /**
   * void save(char *filename)
   * {crop}
   */
  void save(char *filename)
  {
    frame *f = g->crop();
    grid *g2 = new grid(g, f);
    g2->save(filename);
    delete f;
    delete g2;
  }

}; /*- end class paper */
