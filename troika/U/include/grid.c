/*+
 * grid.c
 *
 * An array of "color" elements, packed together with some useful or interesting
 * algorithms.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * void grid::construct()
 *
 *
 */
void grid::construct()
{
  grid_array = new (color *) [grid_rows*grid_cols];
}



/**
 * grid::grid(int id0)
 *
 *
 */
grid::grid(int id0)
      : item(id0),
        grid_array(NULL)
{
  #ifdef GRID_DEBUG
    cout << "<> grid::grid(int)" << endl;
    cout.flush();
  #endif
}



/**
 * grid::grid(class universe *u, color *default_c0, int rows0, int cols0)
 *
 *
 */
grid::grid(class universe *u, color *default_c0, int rows0, int cols0)
      : item(u, item_classe),
        default_c(default_c0),
        grid_rows(rows0),
        grid_cols(cols0)
{
  #ifdef GRID_DEBUG
    cout << "<> grid::grid(class universe *, color *, int, int)" << endl;
    cout.flush();
  #endif

  construct();
}



/**
 * grid::grid(universe *u, grid *ref, frame *f)
 *
 *
 */
grid::grid(universe *u, grid *ref, frame *f)
      : item(u, item_classe),
        default_c(ref->default_c),
        grid_rows(f->height()),
        grid_cols(f->width())
{
  #ifdef GRID_DEBUG
    cout << "<> grid::grid(universe *, grid *, frame *)" << endl;
    cout.flush();
  #endif

  construct();
  color **cp = ref->grid_array + ((f->y() * grid_cols) + f->x()), **cp2 = grid_array;
  for (int i=0; i<grid_rows; i++) {
    for (int j=0; j<grid_cols; j++) {
      *cp2 = *cp;
      cp++;
      cp2++;
    }
    cp += (ref->grid_cols - grid_cols);
  }
}



/**
 * grid::~grid()
 *
 *
 */
grid::~grid()
{
  #ifdef GRID_DEBUG
    cout << "<> grid::~grid()" << endl;
    cout.flush();
  #endif

  if (grid_array != NULL)
    delete grid_array;
}



/**
 * int grid::rows()
 *
 *
 */
int grid::rows()
{
  return grid_rows;
}



/**
 * int grid::cols()
 *
 *
 */
int grid::cols()
{
  return grid_cols;
}



/**
 * color **grid::get_array()
 *
 *
 */
color **grid::get_array()
{
  return grid_array;
}



/**
 * color *grid::default_color()
 *
 *
 */
color *grid::default_color()
{
  return default_c;
}



/**
 * void grid::serialize(ofstream &out)
 *
 *
 */
void grid::serialize(ofstream &out)
{
  #ifdef GRID_DEBUG
    cout << "<> void grid::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t" << grid_rows << ' ' << grid_cols << ' ';
  ((item*)default_c)->write_local_id(out);
  out << " {\n";
  color **cp = grid_array;

  for (int i=0; i<grid_rows; i++) {
//cout << "(" << i << "|"; cout.flush();
    out << "\t\t";
    for (int j=0; j<grid_cols; j++) {
//cout << " " << j; cout.flush();
      ((item *)(*cp))->write_local_id(out);
      if (j<grid_cols-1)
        out << '\t';
      else
        out << '\n';
      cp++;
    }
//cout << ")" << endl; cout.flush();
  }
  out << "\t}\n";
}



/**
 * void grid::deserialize(ifstream &in)
 *
 *
 */
void grid::deserialize(ifstream &in)
{
  #ifdef GRID_DEBUG
    cout << "<> void grid::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  int color_id;
  char *s = new char[10];
  in >> s >> grid_rows >> grid_cols >> color_id;
  default_c = (color *) item_universe->reference_local(color_id, "color");
  in >> s;

  construct();

  color **cp = grid_array;
  color *c;

cout << "[" << grid_rows << "," << grid_cols << "] ("; cout.flush();
  for (int i=0; i<grid_rows*grid_cols; i++) {
cout << " " << i; cout.flush();
    in >> color_id;
    *cp = (color *) item_universe->reference_local(color_id, "color");
    cp++;
  }
cout << ")" << endl; cout.flush();

  in >> s;
cout << "done." << endl; cout.flush();
}



/**
 * color *grid::cell(int i, int j)
 *
 *
 */
color *grid::cell(int i, int j)
{
  i = (i+grid_rows)%grid_rows;
  j = (j+grid_cols)%grid_cols;
  color **cp = grid_array + ((i*grid_cols)+j);
  //return grid_array[i][j];
  return *cp;
}



/**
 * void grid::clear()
 *
 *
 */
void grid::clear()
{
  fill(default_c);
}



/**
 * void grid::fill(color *c)
 *
 *
 */
void grid::fill(color *c)
{
  color **cp = grid_array;
  for (int i=0; i<grid_rows*grid_cols; i++) {
    *cp = c;
    cp++;
  }
}



/*+ interactions between grids *//*=========================================*/

/**
 * void grid::copy(grid *ref)
 *
 *
 */
/* Note: assumes same array dimensions */
void grid::copy(grid *ref)
{
  color **cp = grid_array, **cp2 = ref->grid_array;
  for (int i=0; i<grid_rows*grid_cols; i++) {
    *cp = *cp2;
    cp++;
    cp2++;
  }
}



/**
 * void grid::embed(grid *ref, int i_corner, int j_corner)
 *
 * Overwrite the contents of one grid with those of another
 * starting at the specified upper left corner.
 *
 * Note: current grid dimensions must be large enough to hold the target pattern.
 */
void grid::embed(grid *ref, int i_corner, int j_corner)
{
  if ((ref->grid_rows > grid_rows)||(ref->grid_cols > grid_cols))
    return;
  else {
    color **cp = ref->grid_array;
    for (int i=0; i<ref->grid_rows; i++) {
      for (int j=0; j<ref->grid_cols; j++) {
        grid_array[((i_corner+i)*grid_cols)+j_corner+j] = *cp;
        cp++;
      }
    }
  }
}



/**
 * void grid::embed_center(grid *ref)
 *
 * Overwrite the center of one grid with another
 */
void grid::embed_center(grid *ref)
{
  embed(ref, (grid_rows-ref->grid_rows)/2, (grid_cols-ref->grid_cols)/2);
}


/*- interactions between grids ---------------------------------------------*/

/*+ transformations (the interesting stuff) *//*============================*/
/* Note: all of these transformations assume that the reference grid has
   the same dimensions and default color as "this" grid */

// relative vs. eigen-transformations

/**
 * void grid::flatten(grid *ref, color *dest_c)
 *
 *
 */
void grid::flatten(grid *ref, color *dest_c)
{
  color **cp = grid_array, **cp2 = ref->grid_array;
  for (int i=0; i<grid_rows*grid_cols; i++) {
    if (!ref->default_c->equals(*cp2))
      *cp = dest_c;
    cp++;
    cp2++;
  }
}



/**
 * void grid::set(int i, int j, color *c)
 *
 *
 */
void grid::set(int i, int j, color *c)
{
  i = (i+grid_rows)%grid_rows;
  j = (j+grid_cols)%grid_cols;
  color **cp = grid_array + ((grid_cols*i) + j);
  *cp = c;
}



/**
 * void grid::recursive_set(int i, int j, color *c)
 *
 * For use by flex().
 */
void grid::recursive_set(int i, int j, color *c)
{
  i = (i+grid_rows)%grid_rows;
  j = (j+grid_cols)%grid_cols;
  color **cp = grid_array + ((grid_cols*i) + j);
  if (!default_c->equals(*cp)) {
    *cp = c;
    recursive_set(i-1, j-1, c);
    recursive_set(i-1, j, c);
    recursive_set(i-1, j+1, c);
    recursive_set(i, j-1, c);
    recursive_set(i, j+1, c);
    recursive_set(i+1, j-1, c);
    recursive_set(i+1, j, c);
    recursive_set(i+1, j+1, c);
  }
}



/**
 * void grid::flex(grid *ref, color *dest_c, color *source_c)
 *
 * "Tokenize" a grid, highlighting the first token.
 */
void grid::flex(grid *ref, color *dest_c, color *source_c)
{
  flatten(ref, source_c);
  //dest_c = dest_c0;
  //source_c = source_c0;
  color **cp = ref->grid_array;
  int i, j;
  for (i=0; i<grid_rows*grid_cols; i++) {
    if (source_c->equals(*cp))
      break;
    cp++;
  }
  if (i<grid_rows*grid_cols) {
    j = i%grid_cols;
    i = i/grid_cols;
    //imin = i; imax = i; jmin = j; jmax = j;
    recursive_set(i, j, dest_c);
    /*
    cout << "imin = " << imin << endl;
    cout << "imax = " << imax << endl;
    cout << "jmin = " << jmin << endl;
    cout << "jmax = " << jmax << endl;
    */
  }
}



/**
 * void grid::conway(grid *ref, color *fore_c)
 *
 * A dead cell with exactly 3 live neighbours becomes alive.
 * A live cell with 2 or 3 live neighbours stays alive.
 *
 * {Atari}
 */
void grid::conway(grid *ref, color *fore_c)
{
  color **cp = ref->grid_array, **cp2 = grid_array;
  int neighbors;
  bool live;
  for (int i=0; i<grid_rows; i++) {
    for (int j=0; j<grid_cols; j++) {
      neighbors = 0;
      if (!ref->default_c->equals(ref->cell(i-1,j-1))) //### ...
        neighbors++;
      if (!ref->default_c->equals(ref->cell(i-1,j+1))) //### ...
        neighbors++;
      if (!ref->default_c->equals(ref->cell(i+1,j-1))) //### ...
        neighbors++;
      if (!ref->default_c->equals(ref->cell(i+1,j+1))) //### ...
        neighbors++;
      if (!ref->default_c->equals(ref->cell(i-1,j))) //### ...
        neighbors++;
      if (!ref->default_c->equals(ref->cell(i+1,j)))
        neighbors++;
      if (!ref->default_c->equals(ref->cell(i,j-1)))
        neighbors++;
      if (!ref->default_c->equals(ref->cell(i,j+1)))
        neighbors++;
      if (!ref->default_c->equals(*cp)) {
        live = (neighbors > 1 && neighbors < 4);
        fore_c = *cp;
      } else
        live = (neighbors == 3);
      if (live)
        *cp2 = fore_c;
      else
        *cp2 = ref->default_c;
      cp++;
      cp2++;
    }
  }
  //color **temp = grid_array;
  //grid_array = stunt_double;
  //stunt_double = temp;
}



/*- transformations ----------------------------------------------------------*/

/*+ miscellaneous *//*======================================================*/

/**
 * frame *grid::crop()
 *
 * find boundaries
 */
frame *grid::crop()
{
  int imin, imax, jmin, jmax;

  register int i, j;
  int max_index = grid_rows*grid_cols-1;

  color **cp = grid_array, **cp2;
  for (i=0; i<=max_index; i++) {
    if (default_c != (*cp))
      break;
    cp++;
  }
  imin = i/grid_cols;

  cp = grid_array + max_index;
  for (i=0; i<=max_index; i++) {
    if (default_c != (*cp))
      break;
    cp--;
  }
  imax = grid_rows - (i/grid_cols) - 1;

  cp = grid_array;
  for (j=0; j<grid_cols; j++) {
    cp2 = cp;
    for (i=0; i<grid_rows; i++) {
      if (default_c != (*cp2)) {
        jmax = j;
        break;
      }
      cp2 += grid_cols;
    }
    cp++;
  }

  cp = grid_array + max_index;
  for (j=grid_cols-1; j>=0; j--) {
    cp2 = cp;
    for (i=grid_rows-1; i>=0; i--) {
      if (default_c != (*cp2)) {
        jmin = j;
        break;
      }
      cp2 -= grid_cols;
    }
    cp--;
  }

  return new frame(item_universe, jmin, imin, (jmax-jmin), (imax-imin));
}

/*- miscellaneous ------------------------------------------------------------*/

/*- end of file --------------------------------------------------------------*/
