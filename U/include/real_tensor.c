/*+
 * real_tensor.c
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



/**
 * CLASS_NAME::CLASS_NAME(int id0)
 *
 *
 */
CLASS_NAME::CLASS_NAME(int id0)
       : item(id0),
         tensor_rank(0)
{
  #ifdef REAL_TENSOR_DEBUG
    cout << "<> real_tensor::real_tensor(int)" << endl;
    cout.flush();
  #endif
}



/**
 * CLASS_NAME::CLASS_NAME(universe *u, int *dimensions0, int rank0)
 *
 *
 */
CLASS_NAME::CLASS_NAME(universe *u, int *dimensions0, int rank0)
       : item(u, item_classe),
         tensor_rank(rank0),
         tensor_dimensions(new int[rank0])
{
  #ifdef REAL_TENSOR_DEBUG
    cout << "<> real_tensor::real_tensor(universe *, int *, int)" << endl;
    cout.flush();
  #endif
for (int i=0; i<tensor_rank; i++)
  tensor_size = 1;
  int *dim0 = dimensions0, *dim = tensor_dimensions;
  for (int i=0; i<tensor_rank; i++) {
    *dim = *dim0;
    tensor_size *= (*dim0);
    dim++;
    dim0++;
  }
  tensor_array = new DATA_TYPE[tensor_size];

  //Just for the sake of neatness (in case we forget to assign values before
  //serializing):
  for (int i=0; i<tensor_size; i++)
    tensor_array[i] = 0;
}



/**
 * CLASS_NAME::~CLASS_NAME()
 *
 *
 */
CLASS_NAME::~CLASS_NAME()
{
  #ifdef REAL_TENSOR_DEBUG
    cout << "<> real_tensor::~real_tensor()" << endl;
    cout.flush();
  #endif

  if (tensor_rank != 0) {
    delete tensor_dimensions;
    delete tensor_array;
  }
}



/**
 * void CLASS_NAME::serialize(ofstream &out)
 *
 *
 */
void CLASS_NAME::serialize(ofstream &out)
{
  #ifdef REAL_TENSOR_DEBUG
    cout << "<> void real_tensor::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t" << tensor_rank << " { ";
  for (int i=0; i<tensor_rank; i++)
    out << tensor_dimensions[i] << " ";
  out << "} { ";
  for (int i=0; i<tensor_size; i++)
    out << tensor_array[i] << " ";
  out << "}\n";
}



/**
 * void CLASS_NAME::serialize(ofstream &out)
 *
 * For debugging purposes.
 */
void CLASS_NAME::serialize()
{
  cout << '\t' << id() << ")\t" << tensor_rank << " { ";
  for (int i=0; i<tensor_rank; i++)
    cout << tensor_dimensions[i] << " ";
  cout << "} { ";
  for (int i=0; i<tensor_size; i++)
    cout << tensor_array[i] << " ";
  cout << "}\n";
}



/**
 * void CLASS_NAME::deserialize(ifstream &in)
 *
 *
 */
void CLASS_NAME::deserialize(ifstream &in)
{
  #ifdef REAL_TENSOR_DEBUG
    cout << "<> void real_tensor::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[MAX_STRLEN];
  in >> s >> tensor_rank >> s;
  tensor_dimensions = new int[tensor_rank];
  tensor_size = 1;
  for (int i=0; i<tensor_rank; i++) {
    in >> tensor_dimensions[i];
    tensor_size *= tensor_dimensions[i];
  }
  tensor_array = new DATA_TYPE[tensor_size];
  in >> s >> s;
  for (int i=0; i<tensor_size; i++)
    in >> tensor_array[i];
  in >> s;
  delete s;
}



/**
 * int CLASS_NAME::rank()
 *
 *
 */
int CLASS_NAME::rank()
{
  return tensor_rank;
}



/**
 * int CLASS_NAME::dimension(int index)
 *
 *
 */
int CLASS_NAME::dimension(int index)
{
  return tensor_dimensions[index];
}



/**
 * int CLASS_NAME::size()
 *
 *
 */
int CLASS_NAME::size()
{
  return tensor_size;
}



/**
 * int CLASS_NAME::index_of(int *indices)
 *
 *
 */
/* Note: we're using a big-endian format for tensor indexing. */
int CLASS_NAME::index_of(int *indices)
{
  int *dim = tensor_dimensions, *partial_index = indices;
  int factor = tensor_size;
  int index = 0;
  for (int i=0; i<tensor_rank; i++) {
    factor /= *dim;
    index += factor*(*partial_index);
    dim++;
    partial_index++;
  }
  return index;
}



/**
 * DATA_TYPE CLASS_NAME::get(int index)
 *
 *
 */
DATA_TYPE CLASS_NAME::get(int index)
{
  return tensor_array[index];
}



/**
 * DATA_TYPE CLASS_NAME::get(int *indices)
 *
 *
 */
DATA_TYPE CLASS_NAME::get(int *indices)
{
  return tensor_array[index_of(indices)];
}



/**
 * void CLASS_NAME::set(int index, DATA_TYPE val)
 *
 *
 */
void CLASS_NAME::set(int index, DATA_TYPE val)
{
  tensor_array[index] = val;
}



/**
 * void CLASS_NAME::set(int *indices, DATA_TYPE val)
 *
 *
 */
void CLASS_NAME::set(int *indices, DATA_TYPE val)
{
  tensor_array[index_of(indices)] = val;
}



/**
 * DATA_TYPE **CLASS_NAME::compose_by_dimension()
 *
 *
 */
/*
   Note: can probably be optimized somewhat by eliminating the call to "get".
   Also, you'll want to have the cycling code separate from the "working" code,
   which will be easy enough if you make "function" objects out of both
   components and make "indices" a member rather than a local variable.  Then
   you can re-use the cycling portion for arbitrary "one-per-cell" operations.
*/
DATA_TYPE **CLASS_NAME::compose_by_dimension()
{
  DATA_TYPE **composition = new (DATA_TYPE *)[tensor_rank];
  for (int k=0; k<tensor_rank; k++) {
    composition[k] = new DATA_TYPE[tensor_dimensions[k]];
    for (int l=0; l<tensor_dimensions[k]; l++)
      composition[k][l] = 0;
  }

  int *indices = new int[tensor_rank], *index = indices;
  for (int k=0; k<tensor_rank; k++)
    indices[k] = 0;

  int i = 0, *dim = tensor_dimensions;

  while (true) { // Returns "composition" array when finished.

    DATA_TYPE dt = get(indices);
    for (int j=0; j<tensor_rank; j++)
      composition[j][indices[j]] += dt;

    i = tensor_rank-1;
    dim = tensor_dimensions+i;
    index = indices+i;
    while (*index == (*dim)-1) {
      *index = 0;
      i--;
      if (i<0) {
        delete indices;
        #ifdef REAL_TENSOR_DEBUG
		  cout << "<> double **real_tensor::compose_by_dimension():" << endl;
		  for (int u=0; u<tensor_rank; u++) {
			cout << u << ":";
			for (int v=0; v<tensor_dimensions[u]; v++)
			  cout << " " << composition[u][v];
			cout << endl;
	      }
		  cout.flush();
        #endif
        return composition;
      }
      index--;
      dim--;
    }
    (*index)++;
  }
}



/**
 * void CLASS_NAME::apply_product_rule(DATA_TYPE **factor_array)
 *
 *
 */
void CLASS_NAME::apply_product_rule(DATA_TYPE **factor_array)
{
  int *indices = new int[tensor_rank], *index = indices;
  for (int k=0; k<tensor_rank; k++)
    indices[k] = 0;

  int i = 0, *dim = tensor_dimensions;

  while (true) { // Returns from within loop when finished.

    DATA_TYPE product = 1;
    for (int k=0; k<tensor_rank; k++)
      product *= factor_array[k][indices[k]];
    set(indices, product);

    i = tensor_rank-1;
    dim = tensor_dimensions+i;
    index = indices+i;
    while (*index == (*dim)-1) {
      *index = 0;
      i--;
      if (i<0) {
        delete indices;
        #ifdef REAL_TENSOR_DEBUG
		  cout << "<> void real_tensor::apply_product_rule(double **):" << endl;
		  serialize();
		  cout.flush();
        #endif
        return;
      }
      index--;
      dim--;
    }
    (*index)++;
  }
}



/**
 * void CLASS_NAME::clear()
 *
 *
 */
void CLASS_NAME::clear()
{
  int *indices = new int[tensor_rank], *index = indices;
  for (int k=0; k<tensor_rank; k++)
    indices[k] = 0;

  int i = 0, *dim = tensor_dimensions;

  while (true) { // Returns from within loop when finished.

    set(indices, 0);

    i = tensor_rank-1;
    dim = tensor_dimensions+i;
    index = indices+i;
    while (*index == (*dim)-1) {
      *index = 0;
      i--;
      if (i<0) {
        delete indices;
        #ifdef REAL_TENSOR_DEBUG
		  cout << "<> void real_tensor::clear():" << endl;
		  serialize();
		  cout.flush();
        #endif
        return;
      }
      index--;
      dim--;
    }
    (*index)++;
  }
}



/**
 * void CLASS_NAME::use_inference(integer_tensor *inference_rules, CLASS_NAME *output_tensor)
 *
 *
 */
void CLASS_NAME::use_inference(integer_tensor *inference_rules, CLASS_NAME *output_tensor)
{
  output_tensor->clear();

  int *indices = new int[tensor_rank], *index = indices;
  for (int k=0; k<tensor_rank; k++)
    indices[k] = 0;

  int i = 0, *dim = tensor_dimensions;

  while (true) { // Returns from within loop when finished.

    int i0 = inference_rules->get(indices);
    output_tensor->set(i0, get(indices) + output_tensor->get(i0));

    i = tensor_rank-1;
    dim = tensor_dimensions+i;
    index = indices+i;
    while (*index == (*dim)-1) {
      *index = 0;
      i--;
      if (i<0) {
        delete indices;
        #ifdef REAL_TENSOR_DEBUG
		  cout << "<> void real_tensor::use_inference(integer_tensor *, real_tensor *):" << endl;
		  output_tensor->serialize();
		  cout.flush();
        #endif
        return;
      }
      index--;
      dim--;
    }
    (*index)++;
  }
}



/**
 * int CLASS_NAME::get_compound_index(int rank0, int *dimensions, int *indices)
 *
 *
 */
int CLASS_NAME::get_compound_index(int rank0, int *dimensions, int *indices)
{
  int index = 0;
  int factor = 1;
  for (int i=0; i<rank0; i++)
    factor *= dimensions[i];
  for (int i=0; i<rank0; i++) {
    factor /= dimensions[i];
    index += factor*indices[i];
  }
  return index;
}

/*- end of file --------------------------------------------------------------*/
