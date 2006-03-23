/*+
 * array.c
 *
 * For user-accessible item arrays.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * array::array(int id0)
 *
 *
 */
array::array(int id0)
       : item(id0),
         array_head(NULL)
{
  #ifdef ARRAY_DEBUG
    cout << "<> array::array(int)" << endl;
    cout.flush();
  #endif
}



/**
 * array::array(universe *u, int size0)
 *
 *
 */
array::array(universe *u, int size0)
       : item(u, item_classe),
         array_size(size0),
         array_head(new (item *)[size0])
{
  #ifdef ARRAY_DEBUG
    cout << "<> array::array(universe *, int)" << endl;
    cout.flush();
  #endif

  for (int i=0; i<array_size; i++)
    array_head[i] = NULL;
}



/**
 * array::~array()
 *
 *
 */
array::~array()
{
  #ifdef ARRAY_DEBUG
    cout << "<> array::~array()" << endl;
    cout.flush();
  #endif

  if (array_head != NULL)
    delete array_head;
}


/**
 * void array::serialize(ofstream &out)
 *
 *
 */
void array::serialize(ofstream &out)
{
  #ifdef ARRAY_DEBUG
    cout << "<> void array::serialize(ofstream &): " << array_size << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t" << array_size << " {";
  for (int i=0; i<array_size; i++) {
    out << " ";
    array_head[i]->write_id(out);
  }
  out << " }\n";
}



/**
 * void array::deserialize(ifstream &in)
 *
 *
 */
void array::deserialize(ifstream &in)
{
  char *s = new char[10];
  in >> s >> array_size >> s;

  #ifdef ARRAY_DEBUG
    cout << "<> void array::deserialize(ifstream &): " << array_size << endl;
    cout.flush();
  #endif

  for (int i=0; i<array_size; i++) {
    in >> s;
    array_head[i] = (item *) item_universe->reference(s);
  }
  in >> s;
  delete s;
}

/*- end of file --------------------------------------------------------------*/


