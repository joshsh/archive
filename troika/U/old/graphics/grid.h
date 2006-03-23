#include <fstream>

/*+
 * class grid
 *
 *//*#########################################################################*/

class grid
{
private:

  int pen_i, pen_j;
  color **pos;

  void construct()
  {
    array = new (color *) [rows*cols];
    pen_i=0;
    pen_j=0;
    pos=array;
  }

public:

  color **array;
  color *default_c;
  int rows, cols;

  /** constructors, loading and saving *//*===================================*/

  grid(color *default_c0, int rows0, int cols0)
  {
    default_c = default_c0;
    rows = rows0;
    cols = cols0;
    construct();
  }

  grid(grid *g, frame *f)
  {
    default_c = g->default_c;
    rows = 1+f->imax-f->imin;
    cols = 1+f->jmax-f->jmin;
    construct();
    color **cp = g->array + ((f->imin * cols) + f->jmin), **cp2 = array;
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
        *cp2 = *cp;
        cp++;
        cp2++;
      }
      cp += (g->cols - cols);
    }
  }

  grid(char *filename)
  {
    ifstream in(filename);
    in >> rows;
    in >> cols;

    char *s = new char[7];
    s += 6;
    *s = '\0';
    s -= 6;
    in >> s;
    default_c = new color(s);

    construct();

    color **cp = array;
    color *c;
    for (int i=0; i<rows*cols; i++) {
      in >> s;
      c = new color(s);  // Note: wasty.  In future, re-use colors.
      if (c->equals(default_c)) {
        delete(c);
        *cp = default_c;
      } else
        *cp = c;
      cp++;
    }

    in.close();
  }

  ~grid()
  {
    delete array;
  }

  /**
   * write(ofstream out, int indent()
   *
   * Write an ASCII-encoded grid to a stream for storage.
   */
  write(ofstream out)
  {
	out << "\tgrid " << id() << " {\n";
    out << '\t';
    write_formatted_name(out);
    out << "\n";
    out << rows << ' ' << cols << ' ' << default_c->id() << '\n';
    color **cp = array;
    for (int i=0; i<rows; i++) {
	  out << "\t\t";
      for (int j=0; j<rows; j++) {
        out << (*c)->id();
        if (j<rows-1)
          out << '\t';
        else
          out << '\n';
        cp++;
      }
    }
    out << "\t}\n";
  }

  /** interactions between grids *//*=========================================*/

  /* Note: assumes same array dimensions */
  void copy(grid *ref)
  {
    color **cp = array, **cp2 = ref->array;
    for (int i=0; i<rows*cols; i++) {
      *cp = *cp2;
      cp++;
      cp2++;
    }
  }

  /**
   * void embed(grid *ref, int i_corner, int j_corner)
   *
   * Overwrite the contents of one grid with those of another
   * starting at the specified upper left corner.
   *
   * Note: current grid dimensions must be large enough to hold the target pattern.
   */
   void embed(grid *ref, int i_corner, int j_corner)
   {
     if ((ref->rows > rows)||(ref->cols > cols))
       return;
     else {
      color **cp = ref->array;
      for (int i=0; i<ref->rows; i++) {
        for (int j=0; j<ref->cols; j++) {
          array[((i_corner+i)*cols)+j_corner+j] = *cp;
          cp++;
        }
      }
     }
   }

  /**
   * void embed_center(grid *ref)
   *
   * Overwrite the center of one grid with another
   */
  void embed_center(grid *ref)
  {
    embed(ref, (rows-ref->rows)/2, (cols-ref->cols)/2);
  }

  /** transformations (the interesting stuff) *//*============================*/
  /* Note: all of these transformations assume that the reference grid has
     the same dimensions and default color as "this" grid */

  // relative vs. eigen-transformations

  void flatten(grid *ref, color *dest_c)
  {
    color **cp = array, **cp2 = ref->array;
    for (int i=0; i<rows*cols; i++) {
      if (!default_c->equals(*cp2))
        *cp = dest_c;
      cp++;
      cp2++;
    }
  }

  void flex(grid *ref, color *dest_c0, color *source_c0)
  {
    flatten(source_c0);
    dest_c = dest_c0;
    source_c = source_c0;
    color **cp = ref->array;
    int i, j;
    for (i=0; i<rows*cols; i++) {
      if (source_c->equals(*cp))
        break;
      cp++;
    }
    if (i<rows*cols) {
      j = i%cols;
      i = i/cols;
      imin = i; imax = i; jmin = j; jmax = j;
      recursive_set(i, j);
      cout << "imin = " << imin << endl;
      cout << "imax = " << imax << endl;
      cout << "jmin = " << jmin << endl;
      cout << "jmax = " << jmax << endl;
    }
  }

  /**
   * conway()
   *
   * A dead cell with exactly 3 live neighbours becomes alive (or is "born").
   * A live cell with 2 or 3 live neighbours stays alive
   *
   * {Atari}
   */
  void conway(grid *ref, color *fore_c)
  {
    color **cp = array, **cp2 = stunt_double;
    int neighbors;
    bool live;
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
        neighbors = 0;
        if (!default_c->equals(wrap(i-1,j-1))) //### ...
          neighbors++;
        if (!default_c->equals(wrap(i-1,j+1))) //### ...
          neighbors++;
        if (!default_c->equals(wrap(i+1,j-1))) //### ...
          neighbors++;
        if (!default_c->equals(wrap(i+1,j+1))) //### ...
          neighbors++;
        if (!default_c->equals(wrap(i-1,j))) //### ...
          neighbors++;
        if (!default_c->equals(wrap(i+1,j)))
          neighbors++;
        if (!default_c->equals(wrap(i,j-1)))
          neighbors++;
        if (!default_c->equals(wrap(i,j+1)))
          neighbors++;
        if (!default_c->equals(*cp)) {
          live = (neighbors > 1 && neighbors < 4);
          fore_c = *cp;
        } else
          live = (neighbors == 3);
        if (live)
          *cp2 = fore_c;
        else
          *cp2 = default_c;
        cp++;
        cp2++;
      }
    }
    color **temp = array;
    array = stunt_double;
    stunt_double = temp;
  }

  /** miscellaneous *//*======================================================*/

  /* find boundaries */
  frame *crop()
  {
    frame *f = new frame();

    register int i, j;
    int max_index = rows*cols-1;

    color **cp = array, **cp2;
    for (i=0; i<=max_index; i++) {
      if (default_c != (*cp))
        break;
      cp++;
    }
    f->imin = i/cols;

    cp = array + max_index;
    for (i=0; i<=max_index; i++) {
      if (default_c != (*cp))
        break;
      cp--;
    }
    f->imax = rows - (i/cols) - 1;

    cp = array;
    for (j=0; j<cols; j++) {
      cp2 = cp;
      for (i=0; i<rows; i++) {
        if (default_c != (*cp2)) {
          f->jmax = j;
          break;
        }
        cp2 += cols;
      }
      cp++;
    }

    cp = array + max_index;
    for (j=cols-1; j>=0; j--) {
      cp2 = cp;
      for (i=rows-1; i>=0; i--) {
        if (default_c != (*cp2)) {
          f->jmin = j;
          break;
        }
        cp2 -= cols;
      }
      cp--;
    }

    return f;
  }

}; /*- end class grid ........................................................*/
