/*+
 * list.h
 *
 * For user-accessible doubly-linked lists
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class list : item, public linked_list
{
  static class classe *item_classe;

public:

  list(int);

  list(class universe *);

  virtual ~list();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static list* constructor_wrapper(int id0) { return new list(id0); }

};

classe *list::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
