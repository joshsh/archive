/*+
 * node.c
 *
 * For a doubly-linked list.  "next" and "prev" references cannot be set
 * directly; instead use insert_before(), insert_after() and remove().
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * node::node(item *key0)
 *
 * Create a new node.
 */
node::node(item *key0)
      : node_key(key0),
        node_prev(NULL),
        node_next(NULL) {}



/**
 * item *node::key()
 *
 * Return the node's key value.
 */
item *node::key()
{
  return node_key;
}



/**
 * node *node::prev()
 *
 * Get the previous node in the list.
 */
node *node::prev()
{
  return node_prev;
}



/**
 * node *node::next()
 *
 * Get the next node in the list.
 */
node *node::next()
{
  return node_next;
}



/**
 * void node::insert_before(node *n)
 *
 * Insert a node into a list before the specified node.
 */
void node::insert_before(node *n)
{
  if ((n == NULL)||(n == this))
    return;
  else {
    remove();
    node_prev = n->node_prev;
    node_next = n;
    if (n->node_prev != NULL)
      n->node_prev->node_next = this;
    n->node_prev = this;
  }
}



/**
 * void node::insert_after(node *n)
 *
 * Insert a node into a list after the specified node.
 */
void node::insert_after(node *n)
{
  if ((n == NULL)||(n == this))
    return;
  else {
    remove();
    node_prev = n;
    node_next = n->node_next;
    if (n->node_next != NULL)
      n->node_next->node_prev = this;
    n->node_next = this;
  }
}



/**
 * void node::remove()
 *
 * Remove a node from a list.
 */
void node::remove()
{
  /* remove the node from the list */
  if (node_prev != NULL)
    node_prev->node_next = node_next;
  if (node_next != NULL)
    node_next->node_prev = node_prev;

  /* NULL the node's own references (in case we still want to use it as a
     one-node list) */
  node_prev = NULL;
  node_next = NULL;
}



/*- end of file --------------------------------------------------------------*/
