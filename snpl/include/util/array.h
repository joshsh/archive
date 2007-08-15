#ifndef ARRAY_H
#define ARRAY_H

/*+
 * array.h
 *
 * An expanding array of void pointers.  May also be used as a stack or a queue.
 * There are even insert_before(), insert_after() and remove() functions
 * available in case you want to treat the object as a general linked list, but
 * be aware that the actual data structure is still an array, so there is O(n)
 * memory copying overhead for each insertion or removal.
 *
 * Important: this class defines the dequeue() operation as removing an item
 * from the tail end of the "list", so that it's not identical to pop().
 * Stack operations are thus push() and pop(), and queue operations are
 * enqueue() and pop().  Use dequeue() to unput queue items.
 *
 * author: Josh Shinavier
 * last edited: 7/28/04
 */



class SNPL_array
{
private:

  void expand();

protected:

  int array_size, head_index, filled;
  double expansion_factor;
  void **expanding_array;
    
public:

  SNPL_array(int, double);

  ~SNPL_array();

  // Modularity issue (but needed in array_itf.h)
  void **get_array();
  
  void minimize();  
    
  int size();
  
  double expansion();
  
  void clear();
  
  void *get(int);
  
  void set(int, void *);

  void push(void *);
  
  void *pop();
  
  void enqueue(void *);
  
  void *dequeue();
    
  void insert_before(int, void *);
  
  void insert_after(int, void *);
  
  void remove(int);
  
  void simple_remove(int);
    
};



/*- end of file --------------------------------------------------------------*/

#endif
