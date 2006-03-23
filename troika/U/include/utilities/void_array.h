class void_array
{
protected:

  int array_size, array_nextindex;
  void **expanding_array;
  
public:

  void_array();

  void_array(int);

  ~void_array();
  
  int size();
  
  void resize(int);
  
  void expand();
  
  void append(void *);
  
  void *get(int);
  
  void set(int, void *);
  
  void insert_before(int, void *);
  
  void insert_after(int, void *);
  
  void minimize();
  
};



void_array::void_array()
            : array_nextindex(0),
	      array_size(0)
{}



void_array::void_array(int size0)
            : array_nextindex(0),
              array_size(size0)
{
  if (array_size <= 0)
    array_size = 1;
  expanding_array = new (void *)[array_size];
}



void_array::~void_array()
{
  if (array_size)
    delete expanding_array;
}



int void_array::size()
{
  return array_nextindex;
}



void void_array::resize(int size0)
{
  if (size0 > array_size) {
//cout << "resizing..." << endl; cout.flush();
    void **array0 = new (void *)[size0];
    if (array_size) {
//cout << "memcpy'ing..." << endl; cout.flush();
      memcpy(array0, expanding_array, array_size*sizeof(void *));
      delete expanding_array;
    }
    expanding_array = array0;
    array_size = size0;
  }
//cout << "array_size = " << array_size << endl; cout.flush();
}



void void_array::expand()
{
  if (array_size)
    resize(2*array_size);
  else
    resize(1);
}



void void_array::append(void *p)
{
  if (array_nextindex == array_size)
    expand();
  expanding_array[array_nextindex++] = p;
}



void *void_array::get(int index)
{
  if (index < 0 || index >= array_nextindex)
    return NULL;
  else
    return expanding_array[index];
}



void void_array::set(int index, void *p)
{
  if (index >= 0 || index < array_nextindex)
    expanding_array[index] = p;
}



//Note: memcpy'ing and array resizing overhead
void void_array::insert_before(int index, void *p)
{
//cout << "insert_before: " << index << endl; cout.flush();
  if (array_nextindex == array_size)
    expand();
  if ((index < 0)||(!array_nextindex)||(index >= array_nextindex))
    append(p);
  else {
    for (int i=array_nextindex; i>index; i--)
      expanding_array[i] = expanding_array[i-1];
    /*
    memcpy(expanding_array+index+1,
           expanding_array+index,
	   (array_nextindex-index) * sizeof(void *));
    */
    expanding_array[index] = p;
    array_nextindex++;
  }
}



//Note: memory copying and array resizing overhead
void void_array::insert_after(int index, void *p)
{
//cout << "insert_after: " << index << endl; cout.flush();
  if (array_nextindex == array_size)
    expand();
  if ((index < 0)||(!array_nextindex)||(index >= array_nextindex-1))
    append(p);
  else {
    for (int i=array_nextindex; i>(index+1); i--)
      expanding_array[i] = expanding_array[i-1];
    /*memcpy(expanding_array+index+2,
           expanding_array+index+1,
	   (array_nextindex-index-1) * sizeof(void *));
    */
    expanding_array[index+1] = p;
    array_nextindex++;
  }
}



void void_array::minimize()
{
  if (array_nextindex)
    resize(array_nextindex);
}
