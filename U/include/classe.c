/*+
 * classe.c
 *
 * A wrapper class to manage objects derived from "item".
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * classe::classe(int classe_id0,
               char *name0,
               item *(*constructor_wrapper0)(int),
               void (*set_classe)(classe *))
 *
 * Create a new classe, with an empty list of members.
 */
classe::classe(int classe_id0,
               char *name0,
               item *(*constructor_wrapper0)(int),
               void (*set_classe)(classe *))
               : classe_id(classe_id0),
                 classe_name(ascii::copy(name0)),
                 constructor_wrapper(constructor_wrapper0)//,
                 //classe_size(0)
{
  #ifdef CLASSE_DEBUG
    cout << "<> classe::classe(int, char *, item *(*)(int), void (*)(classe *)): \""
         << classe_name << "\"." << endl;
    cout.flush();
  #endif

  //instances = new linked_list();
  (set_classe)(this);
}



/**
 * classe::~classe()
 *
 * Destroy all items of this classe.
 */
classe::~classe()
{
  #ifdef CLASSE_DEBUG
    cout << "<> classe::~classe(): \"" << classe_name << "\"." << endl;
    cout.flush();
  #endif

  /*
  node *n = instances->first();
  while (n != NULL) {
    delete n->key();
    n = n->next();
  }
  delete instances;
  */
}



/**
 * int classe::id()
 *
 * Return the classe's id (this is the first portion of an item's full address).
 */
int classe::id()
{
  return classe_id;
}



/**
 * int classe::size()
 *
 * Return the number of items belonging to this classe.
 */
 /*
int classe::size()
{
  return classe_size;
}
*/


/**
 * char *classe::name()
 *
 * Return the name of the classe.
 */
char *classe::name()
{
  return classe_name;
}



/**
 * int classe::add(item *i)
 *
 * Add a new item to a classe list.
 */
 /*
int classe::add(item *i)
{
  instances->enqueue(i);
  classe_size++;
  return instances->size() - 1;
}
*/


/**
 * void classe::remove(item *i)
 *
 * Remove an item from a classe list.
 */
 /*
void classe::remove(item *i)
{
  //cout << classe_size << "[" << classe_id << ":" << i->get_classe()->id() << ":"
  //     << i->id() << "]" << endl;
  //if ((classe_id == i->get_classe()->id())&&(i->id() >= 0))

  if ((this == i->get_classe())&&(i->id() >= 0))
    classe_size--;

  //cout << "(" << i->id() << "," << classe_size << ")" << endl;
}
*/



/**
 * item *classe::create(int id0)
 *
 * Create a new item of the classe.
 */
item *classe::create(int id0)
{
  //item *i = constructor_wrapper(id0);
  //instances->enqueue(i);
  //classe_size++;
  //return i;
  return constructor_wrapper(id0);
}



/**
 * void classe::serialize(ofstream &out)
 *
 * Serialize all of a universe's items of this classe.
 */
 /*
void classe::serialize(ofstream &out)
{
  #ifdef CLASSE_DEBUG
    cout << "<> void classe::serialize(ofstream &): \"" << classe_name << "\"." << endl;
    cout.flush();
  #endif

  instances->refresh();

  ascii::write(out, classe_name);
  out << ' ' << instances->size() << " {\n";
  node *n = instances->first();
  while (n != NULL) {
    n->key()->serialize(out);
    n = n->next();
  }
  out << "}\n";
}
*/


/**
 * void classe::deserialize(ifstream &in)
 *
 * Deserialize all of a universe's items of this classe.
 */
 /*
void classe::deserialize(ifstream &in)
{
  #ifdef CLASSE_DEBUG
    cout << "<> void classe::deserialize(ifstream &): \"" << classe_name << "\"." << endl;
    cout.flush();
  #endif

  char *s = new char[MAX_STRLEN];
  in >> s >> s >> s >> s;
  node *n = instances->first();
//cout << "[" << instances->size() << "]" << endl; cout.flush();
//int i=0;
  while (n != NULL) {
//cout << "[" << i << ": " << n->key()->id() << "]"; cout.flush(); i++;
    n->key()->deserialize(in);
    n = n->next();
  }
//cout << endl;
  in >> s;
  delete s;
}
*/


/*- end of file --------------------------------------------------------------*/
