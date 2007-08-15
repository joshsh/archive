/*+
 * integer_tensor.c
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



/**
 * CLASS_NAME::CLASS_NAME(int id0)
 *
 *
 */
CLASS_NAME::CLASS_NAME(int id0)
       : item(id0),
         tensor_rank(0)
{
  #ifdef INTEGER_TENSOR_DEBUG
    cout << "<> integer_tensor::integer_tensor(int)" << endl;
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
  #ifdef INTEGER_TENSOR_DEBUG
    cout << "<> integer_tensor::integer_tensor(universe *, int *, int)" << endl;
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
  #ifdef INTEGER_TENSOR_DEBUG
    cout << "<> integer_tensor::~integer_tensor()" << endl;
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
  #ifdef INTEGER_TENSOR_DEBUG
    cout << "<> void integer_tensor::serialize(ofstream &)" << endl;
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
 * void CLASS_NAME::deserialize(ifstream &in)
 *
 *
 */
void CLASS_NAME::deserialize(ifstream &in)
{
  #ifdef INTEGER_TENSOR_DEBUG
    cout << "<> void integer_tensor::deserialize(ifstream &)" << endl;
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
