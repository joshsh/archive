/*+
 * caste.c
 *
 * For defining new internal object types.
 *
 * author: Josh Shinavier
 * last edited: 7/27/04
 */

/**
 * caste::caste(int id0)
 *
 *
 */
caste::caste(int id0)
       : item(id0)
{
  #ifdef CASTE_DEBUG
    cout << "<> caste::caste(int)" << endl;
    cout.flush();
  #endif
}



/**
 * caste::caste(universe *u, classe *c)
 *
 *
 */
caste::caste(universe *u, classe *c)
       : item(u, item_classe),
         wrapped_classe(c)
{
  #ifdef CASTE_DEBUG
    cout << "<> caste::caste(universe *, classe *)" << endl;
    cout.flush();
  #endif
}



/**
 * caste::caste(universe *u, classe *c)
 *
 *
 */
caste::caste(universe *u, caste **cc, int size0)
       : item(u, item_classe),
         wrapped_classe(NULL),
         array_size(size0)
{
  #ifdef CASTE_DEBUG
    cout << "<> caste::caste(universe *, classe *)" << endl;
    cout.flush();
  #endif

  caste_array = new (caste *)[array_size];
  for (int i=0; i<array_size; i++)
    caste_array[i] = cc[i];
}



/**
 * caste::~caste()
 *
 *
 */
caste::~caste()
{
  #ifdef CASTE_DEBUG
    cout << "<> caste::~caste()" << endl;
    cout.flush();
  #endif

  if (wrapped_classe == NULL)
    delete caste_array;
}



/**
 * void caste::display()
 *
 */
void caste::display()
{
  if (wrapped_classe != NULL)
    cout << wrapped_classe->name();
  else {
    for (int i=0; i<array_size; i++) {
      cout << "[";
      caste_array[i]->display();
      cout << "]";
    }
  }
}











/**
 * void caste::serialize(ofstream &out)
 *
 *
 */
void caste::serialize(ofstream &out)
{
  #ifdef CASTE_DEBUG
    cout << "<> void caste::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t";
  if (wrapped_classe != NULL)
    out << wrapped_classe->id() << "\n";
  else {
    out << array_size << " {";
    for (int i=0; i<array_size; i++) {
      out << " ";
      caste_array[i]->write_id(out);
    }
    out << " }\n";
  }
}



/**
 * void caste::deserialize(ifstream &in)
 *
 *
 */
void caste::deserialize(ifstream &in)
{
  #ifdef CASTE_DEBUG
    cout << "<> void caste::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif
  
  char *s = new char[10];
  int n;
  in >> s >> n >> s;
  if (*s != '{')
    wrapped_classe = (classe *) classes->get(n);
  else {
    array_size = n;
    for (int i=0; i<array_size; i++) {
      in >> s;
      caste_array[i] = (caste *) item_universe->reference(s);
    }
    in >> s;    
  }
  delete s;
}

/*- end of file --------------------------------------------------------------*/
