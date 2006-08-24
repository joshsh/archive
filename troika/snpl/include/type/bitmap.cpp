/*+
 * bitmap.cpp
 *
 * An array of SNPL_color elements, bundled together with some useful or interesting
 * algorithms.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

#include "bitmap.h"
using namespace std;



/**
 * void SNPL_bitmap::construct()
 *
 *
 */
void SNPL_bitmap::construct()
{
  bitmap_array = new SNPL_color *[bitmap_rows*bitmap_cols];
}



/**
 * SNPL_bitmap::SNPL_bitmap(SNPL_color *default_c0, int rows0, int cols0)
 *
 *
 */
SNPL_bitmap::SNPL_bitmap(SNPL_color *default_c0, int rows0, int cols0)
      : default_c(default_c0),
        bitmap_rows(rows0),
        bitmap_cols(cols0)
{
  #ifdef BITMAP_DEBUG
    cout << "<> SNPL_bitmap::SNPL_bitmap(SNPL_color *, int, int)" << endl;
    cout.flush();
  #endif

  construct();
}



/**
 * SNPL_bitmap::SNPL_bitmap(SNPL_bitmap *ref, SNPL_frame *f)
 *
 *
 */
SNPL_bitmap::SNPL_bitmap(SNPL_bitmap *ref, SNPL_frame *f)
      : default_c(ref->default_c),
        bitmap_rows(f->height()),
        bitmap_cols(f->width())
{
  #ifdef BITMAP_DEBUG
    cout << "<> SNPL_bitmap::SNPL_bitmap(SNPL_bitmap *, SNPL_frame *)" << endl;
    cout.flush();
  #endif

  construct();
  SNPL_color **cp = ref->bitmap_array + ((f->y() * ref->bitmap_cols) + f->x());
  SNPL_color **cp2 = bitmap_array;
  for (int i=0; i<bitmap_rows; i++) {
    for (int j=0; j<bitmap_cols; j++) {
      *cp2 = *cp;
      cp++;
      cp2++;
    }
    cp += (ref->bitmap_cols - bitmap_cols);
  }
}



/**
 * SNPL_bitmap::~SNPL_bitmap()
 *
 *
 */
SNPL_bitmap::~SNPL_bitmap()
{
  #ifdef BITMAP_DEBUG
    cout << "<> SNPL_bitmap::~SNPL_bitmap()" << endl;
    cout.flush();
  #endif

  if (bitmap_array != NULL)
    delete bitmap_array;
}



/**
 * int SNPL_bitmap::rows()
 *
 *
 */
int SNPL_bitmap::rows()
{
  return bitmap_rows;
}



/**
 * int SNPL_bitmap::cols()
 *
 *
 */
int SNPL_bitmap::cols()
{
  return bitmap_cols;
}



/**
 * color **SNPL_bitmap::get_array()
 *
 *
 */
SNPL_color **SNPL_bitmap::get_array()
{
  return bitmap_array;
}



/**
 * SNPL_color *SNPL_bitmap::default_color()
 *
 *
 */
SNPL_color *SNPL_bitmap::default_color()
{
  return default_c;
}



/**
 * SNPL_color *SNPL_bitmap::cell(int i, int j)
 *
 *
 */
SNPL_color *SNPL_bitmap::cell(int i, int j)
{
  i = (i+bitmap_rows)%bitmap_rows;
  j = (j+bitmap_cols)%bitmap_cols;
  SNPL_color **cp = bitmap_array + ((i*bitmap_cols)+j);
  //return bitmap_array[i][j];
  return *cp;
}



/**
 * void SNPL_bitmap::clear()
 *
 *
 */
void SNPL_bitmap::clear()
{
  fill(default_c);
}



/**
 * void SNPL_bitmap::fill(SNPL_color *c)
 *
 *
 */
void SNPL_bitmap::fill(SNPL_color *c)
{
  SNPL_color **cp = bitmap_array;
  for (int i=0; i<bitmap_rows*bitmap_cols; i++) {
    *cp = c;
    cp++;
  }
}



/*+ interactions between SNPL_bitmaps *//*=========================================*/

/**
 * void SNPL_bitmap::copy(SNPL_bitmap *ref)
 *
 *
 */
/* Note: assumes same array dimensions */
void SNPL_bitmap::copy(SNPL_bitmap *ref)
{
  SNPL_color **cp = bitmap_array, **cp2 = ref->bitmap_array;
  for (int i=0; i<bitmap_rows*bitmap_cols; i++) {
    *cp = *cp2;
    cp++;
    cp2++;
  }
}



/**
 * void SNPL_bitmap::embed(SNPL_bitmap *ref, int i_corner, int j_corner)
 *
 * Overwrite the contents of one SNPL_bitmap with those of another
 * starting at the specified upper left corner.
 *
 * Note: current SNPL_bitmap dimensions must be large enough to hold the target pattern.
 */
void SNPL_bitmap::embed(SNPL_bitmap *ref, int i_corner, int j_corner)
{
  if ((ref->bitmap_rows > bitmap_rows)||(ref->bitmap_cols > bitmap_cols))
    return;
  else {
    SNPL_color **cp = ref->bitmap_array;
    for (int i=0; i<ref->bitmap_rows; i++) {
      for (int j=0; j<ref->bitmap_cols; j++) {
        bitmap_array[((i_corner+i)*bitmap_cols)+j_corner+j] = *cp;
        cp++;
      }
    }
  }
}



/**
 * void SNPL_bitmap::embed_center(SNPL_bitmap *ref)
 *
 * Overwrite the center of one SNPL_bitmap with another
 */
void SNPL_bitmap::embed_center(SNPL_bitmap *ref)
{
  embed(ref, (bitmap_rows-ref->bitmap_rows)/2, (bitmap_cols-ref->bitmap_cols)/2);
}


/*- interactions between SNPL_bitmaps ---------------------------------------------*/

/*+ transformations (the interesting stuff) *//*============================*/
/* Note: all of these transformations assume that the reference SNPL_bitmap has
   the same dimensions and default color as "this" SNPL_bitmap */

// relative vs. eigen-transformations

/**
 * void SNPL_bitmap::flatten(SNPL_bitmap *ref, SNPL_color *dest_c)
 *
 *
 */
void SNPL_bitmap::flatten(SNPL_bitmap *ref, SNPL_color *dest_c)
{
  SNPL_color **cp = bitmap_array, **cp2 = ref->bitmap_array;
  for (int i=0; i<bitmap_rows*bitmap_cols; i++) {
    if (!ref->default_c->equals(*cp2))
      *cp = dest_c;
    cp++;
    cp2++;
  }
}



/**
 * void SNPL_bitmap::set(int i, int j, SNPL_color *c)
 *
 *
 */
void SNPL_bitmap::set(int i, int j, SNPL_color *c)
{
  i = (i+bitmap_rows)%bitmap_rows;
  j = (j+bitmap_cols)%bitmap_cols;
  SNPL_color **cp = bitmap_array + ((bitmap_cols*i) + j);
  *cp = c;
}



/**
 * void SNPL_bitmap::recursive_set(int i, int j, SNPL_color *c)
 *
 * For use by flex().
 */
void SNPL_bitmap::recursive_set(int i, int j, SNPL_color *c)
{
  i = (i+bitmap_rows)%bitmap_rows;
  j = (j+bitmap_cols)%bitmap_cols;
  SNPL_color **cp = bitmap_array + ((bitmap_cols*i) + j);
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
 * void SNPL_bitmap::flex(SNPL_bitmap *ref, SNPL_color *dest_c, SNPL_color *source_c)
 *
 * "Tokenize" a SNPL_bitmap, highlighting the first token.
 */
void SNPL_bitmap::flex(SNPL_bitmap *ref, SNPL_color *dest_c, SNPL_color *source_c)
{
  flatten(ref, source_c);
  //dest_c = dest_c0;
  //source_c = source_c0;
  SNPL_color **cp = ref->bitmap_array;
  int i, j;
  for (i=0; i<bitmap_rows*bitmap_cols; i++) {
    if (source_c->equals(*cp))
      break;
    cp++;
  }
  if (i<bitmap_rows*bitmap_cols) {
    j = i%bitmap_cols;
    i = i/bitmap_cols;
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
 * void SNPL_bitmap::conway(SNPL_bitmap *ref, SNPL_color *fore_c)
 *
 * A dead cell with exactly 3 live neighbours becomes alive.
 * A live cell with 2 or 3 live neighbours stays alive.
 *
 * {Atari}
 */
void SNPL_bitmap::conway(SNPL_bitmap *ref, SNPL_color *fore_c)
{
  SNPL_color **cp = ref->bitmap_array, **cp2 = bitmap_array;
  int neighbors;
  bool live;
  for (int i=0; i<bitmap_rows; i++) {
    for (int j=0; j<bitmap_cols; j++) {
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
  //SNPL_color **temp = bitmap_array;
  //bitmap_array = stunt_double;
  //stunt_double = temp;
}



/*- transformations ----------------------------------------------------------*/

/*+ miscellaneous *//*======================================================*/

/**
 * SNPL_frame *SNPL_bitmap::crop()
 *
 * find boundaries
 */
SNPL_frame *SNPL_bitmap::crop()
{
  int imin, imax, jmin, jmax;

  register int i, j;
  int max_index = bitmap_rows*bitmap_cols-1;

  SNPL_color **cp = bitmap_array, **cp2;
  for (i=0; i<=max_index; i++) {
    if (default_c != (*cp))
      break;
    cp++;
  }
  imin = i/bitmap_cols;

  cp = bitmap_array + max_index;
  for (i=0; i<=max_index; i++) {
    if (default_c != (*cp))
      break;
    cp--;
  }
  imax = bitmap_rows - (i/bitmap_cols) - 1;

  cp = bitmap_array;
  for (j=0; j<bitmap_cols; j++) {
    cp2 = cp;
    for (i=0; i<bitmap_rows; i++) {
      if (default_c != (*cp2)) {
        jmax = j;
        break;
      }
      cp2 += bitmap_cols;
    }
    cp++;
  }

  cp = bitmap_array + max_index;
  for (j=bitmap_cols-1; j>=0; j--) {
    cp2 = cp;
    for (i=bitmap_rows-1; i>=0; i--) {
      if (default_c != (*cp2)) {
        jmin = j;
        break;
      }
      cp2 -= bitmap_cols;
    }
    cp--;
  }
  return new SNPL_frame(jmin, imin, (jmax-jmin)+1, (imax-imin)+1);
}

/*- miscellaneous ------------------------------------------------------------*/

/*- end of file --------------------------------------------------------------*/
