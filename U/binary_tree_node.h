class binary_tree_node
{
  class item *node_key;
  binary_tree_node *node_left, *node_right, *node_parent;
  
  void remove_child(binary_tree_node *);
  
public:

  binary_tree_node(class item *);

  ~binary_tree_node();
  
  class item *key();
  
  int height();
  
  int depth();
  
  binary_tree_node *left();
  
  binary_tree_node *right();
  
  binary_tree_node *parent();
  
  binary_tree_node *leftmost_descendant();
  
  binary_tree_node *rightmost_descendant();
  
  binary_tree_node *predecessor();
  
  binary_tree_node *successor();
  
  void remove();
  
  void BST_insert(binary_tree_node *);
  
  void heap_insert(binary_tree_node *);

};



//------------------------------------------------------------------------------

binary_tree_node::binary_tree_node(item *key0)
{
  node_key = key0;
  node_left = NULL;
  node_right = NULL
  node_parent = NULL;
}



// Avoids a potentially large recursive stack by iteratively deleting a node's
// children.
binary_tree_node::~binary_tree_node()
{
  binary_tree_node *cur = this;
  while (true) { // Will return when all children have been exhausted.
    prev = cur;
    cur = cur->node_left;
    if (cur == NULL) {
      cur = prev->node_right;
      if (cur == NULL) {
        if (prev == this)
          return;
        else {
          cur = prev->node_parent;
          delete prev;
        }
      }
    }
  }
}



void binary_tree_node::remove_child(binary_tree_node *n)
{
  if (n == node_left)
    node_left = NULL;
  else if (n == node_right)
    node_right = NULL;
}



item *binary_tree_node::key()
{
  return node_key;
}



int binary_tree_node::height()
{
  int h, max=1;
  if ((node_left != NULL)&&((h = node_left->height()+1) > max))
    max = h;
  if ((node_right != NULL)&&((h = node_right->height()+1) > max))
    max = h;
  return max;
}



int binary_tree_node::depth()
{
  int d=0;
  node *n = this;
  while (n != NULL) {
    d++;
    n = n->node_parent;
  }
  return d;
}



binary_tree_node *binary_tree_node::left()
{
  return node_left;
}



binary_tree_node *binary_tree_node::right()
{
  return node_right;
}



binary_tree_node *binary_tree_node::parent()
{
  return node_parent;
}



binary_tree_node *binary_tree_node::leftmost_descendant()
{
  node *n = this, *prev;
  while (n != NULL) {
    prev = n;
    n = n->node_left;
  }
  return prev;
}



binary_tree_node *binary_tree_node::rightmost_descendant()
{
  node *n = this, *prev;
  while (n != NULL) {
    prev = n;
    n = n->node_right;
  }
  return prev;
}



// Note: assumes a proper BST structure.
binary_tree_node *binary_tree_node::predecessor()
{
  if (node_left == NULL)
    return this;
  else
    return node_left->rightmost_descendant();
}



// Note: assumes a proper BST structure.
binary_tree_node *binary_tree_node::successor()
{
  if (node_right == NULL)
    return this;
  else
    return node_right->leftmost_descendant();
}








void binary_tree_node::remove()
{
  if ((node_left == NULL)&&(node_right == NULL)) {
    if (node_parent != NULL)
      node_parent->remove_child(this);
  } else {
    
    node_left = NULL;
    node_right = NULL;
  }
  node_parent = NULL;
}

void binary_tree_node::BST_insert(binary_tree_node *n)
{
  n->remove();
  //... cur != n...
  binary_tree_node *cur = this, *prev;
  int ct;
  while () {
    prev = cur;
    ct = n->node_key->compare_to(cur->node_key);
    if (ct < 0) {
      cur = cur->node_left;
      if (cur == NULL)
        prev->node_left = n;
        n->parent = prev;
    } else if (ct > 0) {
    
    } else {
    
    }
  }
}



void binary_tree_node::heap_insert(binary_tree_node *n)
{

}














