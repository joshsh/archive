/*+
 * linked_list.h
 *
 * THIS linked list class is NOT derived from the item class.  A "linked_list"
 * is the sole property of the object which creates it, whereas a "list" is
 * part of a "universe", and may be directly accessible to the user.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class linked_list
{
protected:

  int list_size;

  class node *head, *tail;

  void invert();

public:

  linked_list();

  virtual ~linked_list();

  class node *first();

  class node *last();

  class node *find(item *);

  int find_index(item *);

  class node *find_by_index(int);

  int size();

  void push(class item *);

  item *pop();

  void enqueue(class item *);

  item *dequeue();

  void refresh();

};

/*- end of file --------------------------------------------------------------*/
