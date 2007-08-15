/*+
 * universe.c
 *
 * A class for creating, loading, manipulating and saving data sets either
 * inputted by a human user, or generated according to commands inputted by a
 * human user.
 *
 * author: Josh Shinavier
 * last edited: 7/6/04
 */

 
 
/**
 * void universe::construct(char *name0)
 *
 * A private function used by the constructors to give the universe a name
 * and create an array of "classe" objects.
 */
void universe::construct(char *name0)
{
  #ifdef UNIVERSE_DEBUG
    cout << "<> void universe::construct(char *): \"" << name0 << "\"" << endl;
    cout.flush();
  #endif

  item::item_universe = this;
  universe_name = ascii::copy(name0);
  
  classe_instances = new (linked_list *)[classes->size()];
  for (int i=0; i<classes->size(); i++)
    classe_instances[i] = new linked_list();
}



/**
 * universe::universe(int id0)
 *
 *
 */
universe::universe(int id0)
       : item(id0),
         universe_name(NULL)
{
  #ifdef UNIVERSE_DEBUG
    cout << "<> universe::universe(int)" << endl;
    cout.flush();
  #endif
}



/**
 * universe::universe(char *name0)
 *
 * Create a new (empty) universe.
 */
universe::universe(char *name0)
          : item(NULL, item_classe)
{
  #ifdef UNIVERSE_DEBUG
    cout << "<> universe::universe(char *): \"" << name0 << "\"" << endl;
    cout.flush();
  #endif

  construct(name0);
  //special_references = NULL;
  special_references = new lookup_table(this);
}



/**
 * universe::universe(ifstream &in)
 *
 * Create a universe by loading it from a file.
 */
universe::universe(ifstream &in)
          : item(NULL, item_classe)
{
  #ifdef UNIVERSE_DEBUG
    cout << "<> universe::universe(ifstream &)" << endl;
    cout.flush();
  #endif

  deserialize(in);
}



/**
 * universe::~universe()
 *
 * Apart from the universe's name, all data is bundled together by the classe
 * array.
 */
universe::~universe()
{
  #ifdef UNIVERSE_DEBUG
    cout << "<> universe::~universe(): \"" << universe_name << "\"" << endl;
    cout.flush();
  #endif

  //for (int i=0; i<TOTAL_CLASSES; i++)
  //  delete classes[i];
  for (int i=0; i<classes->size(); i++)
    delete classe_instances[i];
  delete classe_instances;
  
  //delete classes;
  delete universe_name;
}



/**
 * int universe::add(item *i, classe *item_classe0)
 *
 * This is how an item adds itself to a universe.
 */
int universe::add(item *i, classe *item_classe0)
{
  //return item_classe0->add(i);
  linked_list *instances = classe_instances[item_classe0->id()];
  instances->enqueue(i);
  return instances->size() - 1;
}



/**
 * item *universe::reference(char *s)
 *
 * Look up an item by its address
 *
 * Note: this function should be called ONLY during deserialization.  After
 * the helper array deserialization_array has been destroyed, this function
 * could return dangerous nonsense.
 */
item *universe::reference(char *s)
{
  int theclasse, theitem;
  char *s2 = s;
  while (*s2 != '.')
    s2++;
  *s2 = '\0';
  theclasse = atoi(s);
  *s2 = '.';
  s2++;
  theitem = atoi(s2);
  if (theitem >= 0)
    return deserialization_array[theclasse][theitem];
  else
    return NULL;
}



/**
 * item *universe::reference_local(int theitem, char *classname)
 *
 * Get an item of a known classe by its local address.
 * Note: O(n) time complexity, string matching... :-/
 */
item *universe::reference_local(int theitem, char *classname)
{
  if (theitem >= 0) {
    int i;
    for (i=0; i<classes->size(); i++)
      if (!strcmp(classname, ((classe *) classes->get(i))->name()))
        break;
    return deserialization_array[i][theitem];
  } else
    return NULL;
}



item *universe::reference_special(char *s)
{
  //if (special_references == NULL)
  //  special_references = new lookup_table(this);
  return special_references->get(s);
}



void universe::add_special_reference(class item *ref0, char *id0)
{
  //if (special_references == NULL)
  //  special_references = new lookup_table(this);
  special_references->add(ref0, id0);
}
  
  

void universe::display_special_references()
{
  special_references->serialize();
}



/**
 * void universe::serialize(ofstream &out)
 *
 * Save a universe to a file.
 */
void universe::serialize(ofstream &out)
{
  #ifdef UNIVERSE_DEBUG
    cout << "<> void universe::serialize(ofstream &): \"" << universe_name << "\"" << endl;
    cout.flush();
  #endif

  out << "universe ";
  ascii::write(out, universe_name);

  int nonempty_classes = 0, TOTAL_CLASSES = classes->size();
  for (int i=0; i < TOTAL_CLASSES; i++)
    if (classe_instances[i]->size())
      nonempty_classes++;
  
  out << "\n\nclasse " << nonempty_classes << " {\n";
  for (int i=0; i<TOTAL_CLASSES; i++) {
    classe *c = (classe *) classes->get(i);
    if (classe_instances[i]->size()) {
      out << '\t' << i << ")\t";
      ascii::write(out, c->name());
      out << ' ' << classe_instances[i]->size() << '\n';
    }
  }
  out << "}\n\n";

  for (int i=0; i<TOTAL_CLASSES; i++) {
    classe *c = (classe *) classes->get(i);
    linked_list *instances = classe_instances[i];
    if (instances->size() > 0) {
      instances->refresh();
      out << i << ")\t";
      ascii::write(out, c->name());
      out << ' ' << instances->size() << " {\n";
      node *n = instances->first();
      while (n != NULL) {
        n->key()->serialize(out);
        n = n->next();
      }
      out << "}\n\n";
    }
  }
}



/**
 * void universe::deserialize(ifstream &in)
 *
 * Read a universe from a file.  No error tolerance.
 */
void universe::deserialize(ifstream &in)
{
  #ifdef UNIVERSE_DEBUG
    cout << "<> void universe::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  int TOTAL_CLASSES = classes->size();
  deserialization_array = new (item **)[TOTAL_CLASSES];
  
  char *s = new char[MAX_STRLEN];
  in >> s >> s;
  char *s2 = ascii::read(s);
  construct(s2);
  delete s2;
    
  int nonempty_classes;
  in >> s >> nonempty_classes >> s;    
  for (int i=0; i<nonempty_classes; i++) {
    int instances;
    in >> s >> s >> instances;
    int k = identify_classe(s)->id();
    /*
    for (k=0; k<TOTAL_CLASSES; k++) {
      if (!strcmp(s, ((classe *) classes->get(i))->name()))
        break;
    }*/
    deserialization_array[k] = new (item *)[instances];//[classes[i]->size()];
    for (int j=0; j<instances; j++) {
      item *new_item = ((classe *) classes->get(k))->create(j);
      deserialization_array[k][j] = new_item;
      classe_instances[k]->enqueue(new_item);
    }
  }
  in >> s;

  //for (int i=0; i<TOTAL_CLASSES; i++)
  //  classes[i]->deserialize(in);
  //char *s = new char[MAX_STRLEN];  
  for (int i=0; i<TOTAL_CLASSES; i++) {
    linked_list *ci = classe_instances[i];
    if (ci->size()) {
      in >> s >> s >> s >> s;
      node *n = ci->first();
      while (n != NULL) {
        n->key()->deserialize(in);
        n = n->next();
      }
      in >> s;      
    }
  }
  //delete s;

  for (int i=0; i<TOTAL_CLASSES; i++)
    if (classe_instances[i]->size())
      delete deserialization_array[i];
  delete deserialization_array;
  delete s;

  special_references = (lookup_table *) classe_instances[identify_classe("lookup_table")->id()]->first()->key();
}

/*- end of file --------------------------------------------------------------*/
