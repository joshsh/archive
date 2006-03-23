/*+
 * frame.h
 *
 * Defines a region of a grid. Useful when "tokenizing" a bitmap, and whenever
 * it's important to isolate or keep track of different elements within a grid.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class frame : item
{
  static class classe *item_classe;

  int frame_x, frame_y, frame_width, frame_height;

public:

  frame(int);

  frame(class universe *, int, int, int, int);

  virtual ~frame();

  int x();

  int y();

  int width();

  int height();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static frame* constructor_wrapper(int id0) { return new frame(id0); }

};

classe *frame::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
