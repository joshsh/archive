/*+
 * item.h
 *
 * Serves as the base class for various front-end-accessible classes and allows
 * "universe" and "class" to handle them all in essentially the same way.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

#define MAX_STRLEN 100

class item
{
protected:

  int item_id;

  static class classe *item_classe;

public:

  static class universe *item_universe;

  item(int);

  item(class universe *);

  item(class universe *, class classe *);

  virtual ~item();

  virtual int compare_to(item *);

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  int id();

  class universe *get_universe();

  void write_id(ofstream &);

  void write_local_id(ofstream &);

  void remove();

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static item *constructor_wrapper(int id0) { return new item(id0); }

};

classe *item::item_classe = NULL;

universe *item::item_universe = NULL;

/*- end of file --------------------------------------------------------------*/
