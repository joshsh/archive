/*+
 * fuzzy_output_family.c
 *
 * A fuzzy_output_family is just a list of set names, which are used as a
 * reference by the calculating functions of a FLE object.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * fuzzy_output_family::fuzzy_output_family(int id0)
 *
 *
 */
// "Deserializing" constructor
fuzzy_output_family::fuzzy_output_family(int id0)
       : item(id0),
         family_name(NULL),
         member_sets(NULL)
{
  #ifdef FUZZY_OUTPUT_FAMILY_DEBUG
    cout << "<> fuzzy_output_family::fuzzy_output_family(int)" << endl;
    cout.flush();
  #endif
}



/**
 * fuzzy_output_family::fuzzy_output_family(universe *u, char *name0)
 *
 *
 */
//"Run-time" constructor
fuzzy_output_family::fuzzy_output_family(universe *u, char *name0)
       : item(u, item_classe),
         family_name(new ascii(u, name0)),
         member_sets(new list(u))
{
  #ifdef FUZZY_OUTPUT_FAMILY_DEBUG
    cout << "<> fuzzy_output_family::fuzzy_output_family(universe *, char *)" << endl;
    cout.flush();
  #endif
}



/**
 * void fuzzy_output_family::serialize(ofstream &out)
 *
 *
 */
void fuzzy_output_family::serialize(ofstream &out)
{
  #ifdef FUZZY_OUTPUT_FAMILY_DEBUG
    cout << "<> void fuzzy_output_family::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t";
  ((item *) family_name)->write_id(out);
  out << "\t";
  ((item *) member_sets)->write_id(out);
  out << '\n';
}



/**
 * void fuzzy_output_family::deserialize(ifstream &in)
 *
 *
 */
void fuzzy_output_family::deserialize(ifstream &in)
{
  #ifdef FUZZY_OUTPUT_FAMILY_DEBUG
    cout << "<> void fuzzy_output_family::deserialize(ifstream &)" << endl;
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
 * int fuzzy_output_family::size()
 *
 * Return the number of member sets.
 */
int fuzzy_output_family::size()
{
  return member_sets->size();
}



/**
 * char *fuzzy_output_family::name()
 *
 * Return the family's name.
 */
char *fuzzy_output_family::name()
{
  return family_name->key();
}



/**
 * list *fuzzy_output_family::members()
 *
 * Return the list of member "sets".
 */
list *fuzzy_output_family::members()
{
  return member_sets;
}



/**
 * int fuzzy_output_family::find_index(char *key0)
 *
 * Find a member set's position in the list.
 */
int fuzzy_output_family::find_index(char *key0)
{
  node *n = member_sets->first();
  int i=0;
  while (n != NULL) {
    if (!strcmp(key0, ((ascii *) n->key())->key()))
      return i;
    n = n->next();
    i++;
  }
  return -1;
}



/**
 * void fuzzy_output_family::add(char *name)
 *
 * Add a new member set.
 */
void fuzzy_output_family::add(char *name)
{
  member_sets->enqueue((item *) new ascii(item_universe, name));
}



/**
 * void fuzzy_output_family::add(ascii *a)
 *
 * Add an existing "set" as a member.
 */
void fuzzy_output_family::add(ascii *a)
{
  member_sets->enqueue((item *) a);
}



/*- end of file --------------------------------------------------------------*/
