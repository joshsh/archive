/*+
 * array.h
 *
 * For user-accessible item arrays.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class array : item
{
  class item **array_head;
  int array_size;

  static class classe *item_classe;

public:

  array(int);

  array(class universe *, int);

  virtual ~array();

  class item *get(int);

  void set(int, class item *);

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static array* constructor_wrapper(int id0) { return new array(id0); }

};

classe *array::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
