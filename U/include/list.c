/*+
 * list.c
 *
 * For user-accessible doubly-linked lists
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * list::list(int id0)
 *
 *
 */
list::list(int id0)
      : linked_list(),
        item(id0)
{
  #ifdef LIST_DEBUG
    cout << "<> list::list(int)" << endl;
    cout.flush();
  #endif
}



/**
 * list::list(universe *u)
 *
 *
 */
list::list(universe *u)
      : linked_list(),
        item(u, item_classe)
{
  #ifdef LIST_DEBUG
    cout << "<> list::list(universe *)" << endl;
    cout.flush();
  #endif
}



/**
 * list::~list()
 *
 *
 */
list::~list()
{
  #ifdef LIST_DEBUG
    cout << "<> list::~list()" << endl;
    cout.flush();
  #endif
}



/**
 * void list::serialize(ofstream &out)
 *
 *
 */
void list::serialize(ofstream &out)
{
  #ifdef LIST_DEBUG
    cout << "<> void list::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  refresh();

  out << '\t' << id() << ")\t" << list_size << " { ";
  node *cur = head;
  while (cur != NULL) {
    cur->key()->write_id(out);
    out << ' ';
    cur = cur->next();
  }
  out << "}\n";
}



/**
 * void list::deserialize(ifstream &in)
 *
 *
 */
void list::deserialize(ifstream &in)
{
  char *s = new char[10];
  int size0;
  in >> s >> size0 >> s;

  #ifdef LIST_DEBUG
    cout << "<> void list::deserialize(ifstream &): " << size0 << endl;
    cout.flush();
  #endif

  for (int i=0; i<size0; i++) {
    in >> s;
    enqueue(item_universe->reference(s));
  }
  in >> s;
  delete s;
}

/*- end of file --------------------------------------------------------------*/
