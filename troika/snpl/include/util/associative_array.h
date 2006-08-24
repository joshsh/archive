#ifndef ASSOCIATIVE_ARRAY_H
#define ASSOCIATIVE_ARRAY_H

/*+
 * associative_array.h
 *
 * An expanding hash table in which both the indexing value and the
 * target value are void pointers.
 *
 * author: Josh Shinavier
 * last edited: 11/18/04
 */



class SNPL_associative_array
{
  int table_size, filled, capacity;
  double sparsity_factor, expansion_factor;
  void **entries;
		   
  int find_next_prime(int);

  void resize(int);
  
  void expand();  
    
public:
  
  SNPL_associative_array(int, double, double);
  
  ~SNPL_associative_array();
  
  int size();
  
  double sparsity();
  
  double expansion();
    
  void *lookup(void *);
  
  void add(void *, void *);
  
  void remove(void *);

  void **condense();
   
};



/*- end of file --------------------------------------------------------------*/

#endif
