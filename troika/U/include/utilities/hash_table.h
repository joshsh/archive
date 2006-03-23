/*+
 * hash_table.h
 *
 * An expanding hash table in which both the indexing value and the
 * target value are void pointers.
 *
 * author: Josh Shinavier
 * last edited: 7/28/04
 */

#ifdef DEFAULT_EXPANSION_FACTOR
  #undef DEFAULT_EXPANSION_FACTOR
#endif
#ifdef DEFAULT_SPARSITY_FACTOR
  #undef DEFAULT_SPARSITY_FACTOR
#endif 
 
// By default, the hash table will wait until it is 1/3 full before expanding.
// Note: the sparsity factor does not need to be an integer.
#define DEFAULT_SPARSITY_FACTOR 3

// By default, expand() approximately doubles the size of the table.
// Note: the expansion factor does not need to be an integer.
#define DEFAULT_EXPANSION_FACTOR 2



class hash_table
{

  int table_size, filled, capacity;
  double sparsity_factor, expansion_factor;
  void **entries;  //Note: two (void *)'s per "entry":
                   //      first the hashing key, then the target value.
		   
  int find_next_prime(int);

  void resize(int);
  
  void expand();  
    
public:
  
  hash_table(int, double, double);
  
  ~hash_table();
  
  void *retrieve(void *);
  
  void add(void *, void *);
  
};



/**
 * int hash_table::find_next_prime(int i)
 *
 * Returns the least prime > 2 and >= i.
 */
int hash_table::find_next_prime(int i)
{
  if (i<3)
    i=3;
  else if (!(i%2))
    i++;
  
  while (true) { //Breaks out when next prime is found.
    register int j;
    for (j=3; j<i; j+=2)
      if (!(i%j))
        j=i+1;
    if (j>i)
      i+=2;
    else
      return i;
  }
}



void hash_table::resize(int size0)
{
  if (size0 > table_size) {
    int size_old = table_size;
    void **entries_old = entries;
    table_size = find_next_prime(size0);
    entries = new (void *)[2*table_size];
    capacity = (int) (((double) table_size)/sparsity_factor);
    for (register int i=0; i<2*table_size; i++)
      entries[i] = NULL;
    for (register int i=0; i<size_old; i++) {
      void **p = entries_old+(i*2);
      if (*p != NULL) {
        void **q = p+1;
	filled--; //Cancel out the incrementation for this re-hashing add()
        add(*p, *q);
      }
    }
    delete entries_old;
  }
}



/*
 * No good reason to have this separate from resize() any more...
 */
void hash_table::expand()
{
  resize((int) (table_size * expansion_factor));
}



/**
 *
 */
hash_table::hash_table(int size0, double sparsity0, double expansion0)
                 : table_size(find_next_prime(size0)),
		   sparsity_factor(sparsity0),
		   expansion_factor(expansion0),
		   entries(new (void *)[2*table_size]),
		   filled(0)
{
  for (register int i=0; i<table_size*2; i++)
    entries[i] = NULL;
    
  // sparsity_factor must be at least 1, otherwise the table will not resize
  // even when it is completely full.
  if (sparsity_factor < 1)
    sparsity_factor = DEFAULT_SPARSITY_FACTOR;

  // capacity is re-calculated whenever the table resizes
  capacity = (int) (((double) table_size)/sparsity_factor);
    
  // if expansion_factor is too small, the table might not actually gain in
  // integer size when it expand()'s, leading to a segmentation fault.
  if ((expansion_factor <= 1)
    ||(table_size == (int) (table_size*expansion_factor)))
    expansion_factor = DEFAULT_EXPANSION_FACTOR;
}



hash_table::~hash_table()
{
  delete entries;
}



/*
 * Caution: returns 0/NULL if an entry is not found.  Beware of
 * storing a 0/NULL as a target value, else you won't be able to tell it
 * apart from a failed retrieve().
 */
void *hash_table::retrieve(void *key)
{
  int int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  void **p = entries + (2*(int_key%table_size));
  while (*p != key) {
    if (*p == NULL)
      break;
    p += 2;
  }
  p++;
  return *p;  //Note: relies on the target value of empty entries being NULL.
}



void hash_table::add(void *key, void *target)
{
  int int_key = (int) key;
  if (int_key < 0)
    int_key *= -1;
  void **p = entries + (2*(int_key%table_size));
  while (*p != NULL) {
    if (*p == key) {
      filled--;
      break;  //No duplicate entries allowed.
    }
    p += 2;
  }
  *p = key;
  p++;
  *p = target;
  filled++;
  if (filled >= capacity)
    expand();
}



/*- end of file --------------------------------------------------------------*/
