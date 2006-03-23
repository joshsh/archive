/*+
 * linked_list.c
 *
 * For generic/multipurpose doubly-linked lists.  Includes stack and queue
 * operations.  If you don't need the flexibility of doubly-linked lists you'll
 * want to use a simpler (--> lower overhead) class.
 *
 * THIS linked list class is NOT derived from the item class.  A "linked_list"
 * is the sole property of the object which creates it, whereas a "list" is
 * part of a "universe", and may be directly accessible to the user.
 *
 * Note: all of a list's nodes are deleted when its destructor is called.  If
 *       nodes have been removed from the list and not put back, make sure they
 *       are deleted elsewhere, in order to avoid a memory leak.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

// ?{semaphore}



/**
 * void linked_list::invert()
 *
 * Reverse the order of nodes in a list.  Not really necessary, as these lists
 * are doubly-linked.
 */
// {slinky}
void linked_list::invert()
{
  node *cur, *next;
  if (head != NULL) {
    next = head->next();
    head->remove();
    cur = next;
    tail = head;
  }
  while (cur != NULL)
  {
    next = cur->next();
    cur->remove();
    cur->insert_before(head);
    head = cur;
    cur = next;
  }
}



/**
 * linked_list::linked_list()
 *
 * Create a new (empty) linked_list.
 */
linked_list::linked_list()
             : head(NULL),
               tail(NULL),
               list_size(0) {}



/**
 * linked_list::~linked_list()
 *
 * Delete the list and all its nodes.
 */
linked_list::~linked_list()
{
  node *cur = head, *next;
  while (cur != NULL) {
    next = cur->next();
    delete cur;
    cur = next;
  }
}



/**
 * node *linked_list::first()
 *
 * Get the list head.
 */
node *linked_list::first()
{
  return head;
}



/**
 * node *linked_list::last()
 *
 * Get the list tail.
 */
node *linked_list::last()
{
  return tail;
}



/**
 * node *linked_list::find(item *i)
 *
 * Find an item in a list by comparing it to the other list elements.
 * Return NULL if not found.
 */
node *linked_list::find(item *i)
{
  node *n = head;
  while (n != NULL) {
    if (!n->key()->compare_to(i))
      return n;
    n = n->next();
  }
  return NULL;
}



/**
 * int linked_list::find_index(item *i)
 *
 * Find an item in a list and return its position in the list, or -1 if the item
 * is not found.
 */
int linked_list::find_index(item *i)
{
  int k=0;
  node *n = head;
  while (n != NULL) {
    if (!n->key()->compare_to(i))
      return k;
    n = n->next();
    k++;
  }
  return -1;
}



/**
 * node *linked_list::find_by_index(int i)
 *
 * Get the node at a specified offset from the head of the list.
 */
node *linked_list::find_by_index(int i)
{
  if ((i<0)||(i>=list_size))
    return NULL;
  else {
    node *n = head;
    for (int j=0; j<i; j++)
      n = n->next();
    return n;
  }
}



/**
 * int linked_list::size()
 *
 * Return the number of nodes in the list.
 */
int linked_list::size()
{
  return list_size;
}



/*+ Stack operations *//*===================================================*/

/**
 * void linked_list::push(item *key)
 *
 * Create a new node with this key value and push it onto the "stack".
 */
void linked_list::push(item *key)
{
  list_size++;
  node *n = new node(key);
  if (head != NULL)
    n->insert_before(head);
  else
    tail = n;
  head = n;
}



/**
 * item *linked_list::pop()
 *
 * Pop an item from the "stack".
 */
item *linked_list::pop()
{
  node *n = head;
  if (n != NULL) {
    list_size--;
    item *key = n->key();
    head = n->next();
    n->remove();
    delete n;
    if (head == NULL)
      tail = NULL;
    return key;
  } else
    return NULL;
}

/*- stack operations .......................................................*/

/*+ Queue operations *//*===================================================*/

/**
 * void linked_list::enqueue(item *key)
 *
 * Enqueue an item to the list; that is, add it to the end of the list.
 */
void linked_list::enqueue(item *key)
{
  list_size++;
  node *n = new node(key);
  if (tail != NULL)
    n->insert_after(tail);
  else
    head = n;
  tail = n;
}



/**
 * item *linked_list::dequeue()
 *
 * Remove and return the item at the end of the list.
 */
item *linked_list::dequeue()
{
  node *n = tail;
  if (n != NULL) {
    list_size--;
    item *key = n->key();
    tail = n->prev();
    n->remove();
    delete n;
    if (tail == NULL)
      head = NULL;
    return key;
  } else
    return NULL;
}



/**
 * void linked_list::refresh()
 *
 * Update the local addresses of all nodes in the list, so that they are
 * numbered according to position.
 * This function should probably be in the "classe" class, not here.
 */
void linked_list::refresh()
{
  node *head2=NULL, *tail2=NULL;

  node *cur = head, *next;
  while (cur != NULL) {
    next = cur->next();
    cur->remove();
    if (cur->key()->id() >= 0) {
      if (head2 == NULL)
        head2 = cur;
      else
        cur->insert_after(tail2);
      tail2 = cur;
    } else {
      delete cur;
      list_size--;
    }
    cur = next;
  }

  head = head2;
  tail = tail2;
}



/*- queue operations .......................................................*/

/*- end of file --------------------------------------------------------------*/
