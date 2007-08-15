/*+
 * integer_tensor.h
 *
 * A storage class for general tensors in which the entries are handled as
 * int's. I'm using integer_tensors to store the inference rules of my FLEs.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

#ifdef CLASS_NAME
  #undef CLASS_NAME
#endif
#ifdef DATA_TYPE
  #undef DATA_TYPE
#endif

/* Change these when you define a tensor class for a new data type. */
#define CLASS_NAME integer_tensor
#define DATA_TYPE int

class CLASS_NAME : item
{
  static class classe *item_classe;

  int tensor_rank, tensor_size;
  int *tensor_dimensions;
  DATA_TYPE *tensor_array;

public:

  CLASS_NAME(int);

  CLASS_NAME(class universe *, int *, int);

  virtual ~CLASS_NAME();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  int rank();

  int dimension(int);

  int size();

  int index_of(int *);

  DATA_TYPE get(int);

  DATA_TYPE get(int *);

  void set(int, DATA_TYPE);

  void set(int *, DATA_TYPE);

  static int get_compound_index(int, int *, int *);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static CLASS_NAME* constructor_wrapper(int id0) { return new CLASS_NAME(id0); }

};

classe *CLASS_NAME::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
