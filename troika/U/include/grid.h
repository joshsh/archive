/*+
 * grid.h
 *
 * An array of "color" elements, packed together with some useful or interesting
 * algorithms.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class grid : item
{
  static class classe *item_classe;

  class color **grid_array;
  class color *default_c;
  int grid_rows, grid_cols;

  void construct();

public:

  grid(int);

  grid(class universe *, class color *, int, int);

  grid(class universe *, grid *, class frame *);

  virtual ~grid();

  int rows();

  int cols();

  //! used by "paper", possible encapsulation issue
  class color **get_array();

  class color *default_color();

  class color *cell(int, int);

  void set(int, int, color *);

  void recursive_set(int, int, color *);

  void copy(grid *);

  void clear();

  void fill(class color *);

  void embed(grid *, int, int);

  void embed_center(grid *);

  void flatten(grid *, class color *);

  void flex(grid *, class color *, class color *);

  void conway(grid *, class color *);

  class frame *crop();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static grid* constructor_wrapper(int id0) { return new grid(id0); }

};

classe *grid::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
