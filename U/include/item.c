/*+
 * item.c
 *
 * Serves as the base class for various front-end-accessible classes and allows
 * "universe" and "class" to handle them all in essentially the same way.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * item::item(int id0) : item_id(id0)
 *
 *
 */
item::item(int id0) : item_id(id0)
{
  #ifdef ITEM_DEBUG
    cout << "<> item::item(int)" << endl;
    cout.flush();
  #endif
}



/**
 * item::item(universe *u)
 *
 *
 */
item::item(universe *u)
{
  #ifdef ITEM_DEBUG
    cout << "<> item::item(universe *)" << endl;
    cout.flush();
  #endif

  if (u != NULL)
    item_id = u->add(this, item_classe);
  else
    item_id = -1;
}



/**
 * item::item(universe *u, classe *item_classe0)
 *
 *
 */
item::item(universe *u, classe *item_classe0)
{
  #ifdef ITEM_DEBUG
    cout << "<> item::item(universe *, classe *)" << endl;
    cout.flush();
  #endif

  if (u != NULL)
    item_id = u->add(this, item_classe0);
  else
    item_id = 0;
}



/**
 * item::~item()
 *
 *
 */
item::~item()
{
  #ifdef ITEM_DEBUG
    cout << "<> item::~item()" << endl;
    cout.flush();
  #endif
}




/**
 * int item::compare_to(item *i)
 *
 *
 */
int item::compare_to(item *i)
{
  return 0;
}



/**
 * void item::serialize(ofstream &out)
 *
 *
 */
void item::serialize(ofstream &out)
{
  #ifdef ITEM_DEBUG
    cout << "<> void item::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\n";
}



/**
 * void item::deserialize(ifstream &in)
 *
 *
 */
void item::deserialize(ifstream &in)
{
  #ifdef ITEM_DEBUG
    cout << "<> void item::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[10];
  in >> s;
cout << s << "!!!" << endl; cout.flush();
  delete s;
}




/**
 * int item::id()
 *
 * Return the item's local address.
 */
int item::id()
{
  return item_id;
}



/**
 * universe *item::get_universe()
 *
 * Return the universe to which the item belongs.
 */
universe *item::get_universe()
{
  return item_universe;
}



/**
 * void item::write_id(ofstream &out)
 *
 * Write an item's full address to a stream.
 */
void item::write_id(ofstream &out)
{
  out << get_classe()->id() << '.' << item_id;
}



/**
 * void item::write_local_id(ofstream &out)
 *
 * Write an item's local address to a stream.
 */
void item::write_local_id(ofstream &out)
{
  out << item_id;
}



/**
 * void item::remove()
 *
 * Remove an item from a universe.
 */
void item::remove()
{
/*
  (get_classe())->remove(this);
  item_id = -1;
*/
}



/*- end of file --------------------------------------------------------------*/
