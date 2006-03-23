/*+
 * fuzzy_set_family.c
 *
 * A fuzzy_set_family is bound to a particular (input) dimension and controls
 * a list of fuzzy_sets. Given a crisp input value, a fuzzy_set_family will
 * generate an array of degrees of membership appropriate to its member sets'
 * dom information.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * fuzzy_set_family::fuzzy_set_family(int id0)
 *
 *
 */
// "Deserializing" constructor
fuzzy_set_family::fuzzy_set_family(int id0)
       : item(id0),
         family_name(NULL),
         member_sets(NULL)
{
  #ifdef FUZZY_SET_FAMILY_DEBUG
    cout << "<> fuzzy_set_family::fuzzy_set_family(int)" << endl;
    cout.flush();
  #endif
}



/**
 * fuzzy_set_family::fuzzy_set_family(universe *u, char *name0)
 *
 *
 */
//"Run-time" constructor
fuzzy_set_family::fuzzy_set_family(universe *u, char *name0)
       : item(u, item_classe),
         family_name(new ascii(u, name0)),
         member_sets(new list(u))
{
  #ifdef FUZZY_SET_FAMILY_DEBUG
    cout << "<> fuzzy_set_family::fuzzy_set_family(universe *, char *)" << endl;
    cout.flush();
  #endif
}



/**
 * void fuzzy_set_family::serialize(ofstream &out)
 *
 *
 */
void fuzzy_set_family::serialize(ofstream &out)
{
  #ifdef FUZZY_SET_FAMILY_DEBUG
    cout << "<> void fuzzy_set_family::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t";
  ((item *) family_name)->write_id(out);
  out << "\t";
  ((item *) member_sets)->write_id(out);
  out << '\n';
}



/**
 * void fuzzy_set_family::deserialize(ifstream &in)
 *
 *
 */
void fuzzy_set_family::deserialize(ifstream &in)
{
  #ifdef FUZZY_SET_FAMILY_DEBUG
    cout << "<> void fuzzy_set_family::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[MAX_STRLEN];
  in >> s >> s;
  family_name = (ascii *) item_universe->reference(s);
  in >> s;
  member_sets = (list *) item_universe->reference(s);
  delete s;
}



/**
 * int fuzzy_set_family::size()
 *
 * Return the number of member sets.
 */
int fuzzy_set_family::size()
{
  return member_sets->size();
}



/**
 * char *fuzzy_set_family::name()
 *
 * Return the fuzzy_set_family's name.
 */
char *fuzzy_set_family::name()
{
  return family_name->key();
}



/**
 * list *fuzzy_set_family::members()
 *
 * Return the list of member sets.
 */
list *fuzzy_set_family::members()
{
  return member_sets;
}



/**
 * int fuzzy_set_family::find_index(char *key0)
 *
 * Find a member by its name.
 */
int fuzzy_set_family::find_index(char *key0)
{
  node *n = member_sets->first();
  int i=0;
  while (n != NULL) {
    if (!strcmp(key0, ((fuzzy_set *) n->key())->name()))
      return i;
    n = n->next();
    i++;
  }
  return -1;
}



/**
 * void fuzzy_set_family::add(char *name, double min0, double max0)
 *
 * Create a new fuzzy_set and add it as a member.
 */
void fuzzy_set_family::add(char *name, double min0, double max0)
{
  fuzzy_set *fs = new fuzzy_set(item_universe, name, min0, max0);
  member_sets->enqueue((item*) fs);
}



/**
 * void fuzzy_set_family::add(fuzzy_set *fs)
 *
 * Add an existing member set.
 */
void fuzzy_set_family::add(fuzzy_set *fs)
{
  member_sets->enqueue((item*) fs);
}



/**
 * void fuzzy_set_family::fuzzify(double crisp, double *dom_array)
 *
 * Fill an array with the member sets' degree-of-membership values for a
 * particular "crisp" input value.
 * Note: anything out-of-bounds is assumed to belong entirely to an end set.
 */
 // {trapezoid}
void fuzzy_set_family::fuzzify(double crisp, double *dom_array)
{
  double *runner = dom_array;
  node *cur = member_sets->first(), *prev=NULL, *next;
  double min, max;
  while (cur != NULL) {
    next = cur->next();
    min = ((fuzzy_set*) cur->key())->min();
    max = ((fuzzy_set*) cur->key())->max();
    if ((crisp >= min)&&(crisp <= max))
      *runner = 1;
    else if (crisp < min) {
      if (prev == NULL)
        *runner = 1;
      else {
        max = ((fuzzy_set*) prev->key())->max();
        if (crisp <= max)
          *runner = 0;
        else
          *runner = ((crisp-max)/(min-max));
      }
    } else {
      if (next == NULL)
        *runner = 1;
      else {
        min = ((fuzzy_set*) next->key())->min();
        if (crisp >= min)
          *runner = 0;
        else
          *runner = 1- ((crisp-max)/(min-max));
      }
    }
    prev = cur;
    cur = next;
    runner++;
  }
  #ifdef FUZZY_SET_FAMILY_DEBUG
    cout << "<> void fuzzy_set_family::fuzzify(double, double *):\t" << endl;
    for (int i=0; i<member_sets->size(); i++)
      cout <<dom_array[i] << " ";
    cout << endl;
    cout.flush();
  #endif
}

/*- end of file --------------------------------------------------------------*/
