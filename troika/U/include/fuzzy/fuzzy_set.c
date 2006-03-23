/*+
 * fuzzy_set.c
 *
 * These are specifically fuzzy input sets as far as our project is concerned
 * (there is no feedback involved, but there could be). They each contain a
 * name identifier and two values which, in combination with its neighboring
 * sets in the family, determine the set's degree-of-membership function.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * fuzzy_set::fuzzy_set(int id0)
 *
 *
 */
fuzzy_set::fuzzy_set(int id0)
       : item(id0),
         fuzzy_set_name(NULL)
{
  #ifdef FUZZY_SET_DEBUG
    cout << "<> fuzzy_set::fuzzy_set(int)" << endl;
    cout.flush();
  #endif
}



/**
 * fuzzy_set::fuzzy_set(universe *u, char *name0, double dom_min0, double dom_max0)
 *
 *
 */
fuzzy_set::fuzzy_set(universe *u, char *name0, double dom_min0, double dom_max0)
       : item(u, item_classe),
         fuzzy_set_name(new ascii(u, name0)),
         dom_min(dom_min0),
         dom_max(dom_max0)
{
  #ifdef FUZZY_SET_DEBUG
    cout << "<> fuzzy_set::fuzzy_set(universe *, char *, int, int): \""
         << fuzzy_set_name->key() << "\"" << endl;
    cout.flush();
  #endif
}



/**
 * void fuzzy_set::serialize(ofstream &out)
 *
 *
 */
void fuzzy_set::serialize(ofstream &out)
{
  #ifdef FUZZY_SET_DEBUG
    cout << "<> void fuzzy_set::serialize(ofstream &): \""
         << fuzzy_set_name->key() << "\"" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t";
  ((item *) fuzzy_set_name)->write_id(out);
  out << "\t" << dom_min << "\t" << dom_max << "\n";
}



/**
 * void fuzzy_set::deserialize(ifstream &in)
 *
 *
 */
void fuzzy_set::deserialize(ifstream &in)
{
  #ifdef FUZZY_SET_DEBUG
    cout << "<> void fuzzy_set::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[MAX_STRLEN];
  in >> s >> s >> dom_min >> dom_max;
  fuzzy_set_name = (ascii *) item_universe->reference(s);
  delete s;
}



/**
 * double fuzzy_set::min()
 *
 * Return the fuzzy_set's min value.
 */
double fuzzy_set::min()
{
  return dom_min;
}



/**
 * double fuzzy_set::max()
 *
 * Return the fuzzy_set's max value.
 */
double fuzzy_set::max()
{
  return dom_max;
}



/**
 * char *fuzzy_set::name()
 *
 * Return the fuzzy_set's name.
 */
char *fuzzy_set::name()
{
  return fuzzy_set_name->key();
}



/*- end of file --------------------------------------------------------------*/
