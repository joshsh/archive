#ifndef TERNARY_ASSOCIATIVE_ARRAY_H
#define TERNARY_ASSOCIATIVE_ARRAY_H

/*+
 * ternary_associative_array.h
 *
 * An expanding hash table in which both the indexing value and the
 * target value are void pointers.
 *
 * author: Josh Shinavier
 * last edited: 11/18/04
 */



class SNPL_ternary_associative_array
{
  int table_size, filled, capacity;
  double sparsity_factor, expansion_factor;
  void **entries;
  
  int find_next_prime(int);

  void resize(int);
  
  void expand();  
    
public:
  
  SNPL_ternary_associative_array(int, double, double);
  
  ~SNPL_ternary_associative_array();
  
  int size();
  
  double sparsity();
  
  double expansion();
  
  //!!!!!! modularity issue.  But interface requires this...
  void **get_entries();
  
  void *lookup(void *, void *);
  
  void add(void *, void *, void *);

  void remove(void *, void *);

  void **condense();
  
#ifdef TERNARY_ASSOCIATIVE_ARRAY_DEBUG
  void display();
  
  void display(void (*)(void *));
#endif
  
};



/*- end of file --------------------------------------------------------------*/

#endif
