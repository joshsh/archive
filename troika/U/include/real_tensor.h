/*+
 * real_tensor.h
 *
 * Like integer_tensor, but with floating-point (double) entries. Includes the
 * functions compose_by_dimension(), apply_product_rule() and use_inference()
 * for the benefit of the FLE class. These aren't really basic enough to belong
 * here, and will be left to the user to define when I've finished the procedure
 * class, as will much of the actual fuzzy logic which for the moment is
 * hard-coded into FLE.
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
#define CLASS_NAME real_tensor
#define DATA_TYPE double

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

  void serialize();  //~

  int rank();

  int dimension(int);

  int size();

  int index_of(int *);

  DATA_TYPE get(int);

  DATA_TYPE get(int *);

  void set(int, DATA_TYPE);

  void set(int *, DATA_TYPE);

  DATA_TYPE **compose_by_dimension();

  void apply_product_rule(DATA_TYPE **);

  void clear();

  //~
  void use_inference(integer_tensor *, CLASS_NAME *);

  static int get_compound_index(int, int *, int *);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static CLASS_NAME* constructor_wrapper(int id0) { return new CLASS_NAME(id0); }

};

classe *CLASS_NAME::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
