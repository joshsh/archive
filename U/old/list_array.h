/**
 * A helper class to cut down the time complexity of algorithms which would
 * otherwise have to cycle repetitively through a linked list to get at
 * individual items, e.g. the universe::deserialize routine.
 */
class list_array
{
  class item **array;
  int list_size;

public:

  list_array(class linked_list *);

  int size();

  //int index_of(class item *);
};

list_array::list_array(list *l)
{
  list_length = l->length();
  array = new (item*)[list_length];
  item **p = array;
  node *n = l->first();
  while (n != NULL) {
    *p = n->key();
    p++;
    n = n->next();
  }
}

/*
list_array::index_of(item *key)
{
  item **p = array;
  while (p < array+list_length) {
    if (*p == key)
      return (int) p-array;
    p++;
  }
  return -1;
}
*/
