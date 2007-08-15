/*+
 * node.h
 *
 * For a doubly-linked list.  "next" and "prev" references cannot be set
 * directly; instead use insert_before(), insert_after() and remove().
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class node
{
  class item *node_key;
  node *node_prev, *node_next;

public:

  node(class item *);

  item *key();

  node *prev();

  node *next();

  void insert_before(node *);

  void insert_after(node *);

  void remove();

};

/*- end of file --------------------------------------------------------------*/
