/*+
 * lookup_table.c
 *
 * These use a binary search to pair a string identifier with a named "item".
 *
 * author: Josh Shinavier
 * last edited: 7/6/04
 */



/**
 * lookup_table::lookup_table(int id0)
 *
 *
 */
lookup_table::lookup_table(int id0)
       : item(id0),
         void_array()
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> lookup_table::lookup_table(int)" << endl;
    cout.flush();
  #endif
}



/**
 * lookup_table::lookup_table(universe *u)
 *
 *
 */
lookup_table::lookup_table(universe *u)
       : item(u, item_classe),
         void_array()
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> lookup_table::lookup_table(universe *)" << endl;
    cout.flush();
  #endif
}



/**
 * lookup_table::lookup_table(universe *u, int size0)
 *
 *
 */
lookup_table::lookup_table(universe *u, int size0)
       : item(u, item_classe),
         void_array(size0)
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> lookup_table::lookup_table(universe *, int)" << endl;
    cout.flush();
  #endif
}



/**
 * lookup_table::~lookup_table()
 *
 *
 */
lookup_table::~lookup_table()
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> lookup_table::~lookup_table()" << endl;
    cout.flush();
  #endif
  
  for (int i=0; i<array_nextindex; i++) {
    delete ((reference *) expanding_array[i]);
  }
}



//Note: multiple points of exit.
item *lookup_table::get(char *s)
{
/*for (int jj=array_nextindex-1; jj>=0; jj--) {
  cout << "[" << ((reference *) expanding_array[jj])->id << "]";
  cout.flush();
}
cout << endl; cout.flush();
cout << "Studebaker (?)" << endl; cout.flush();*/
  if (!array_nextindex)
    return NULL;
  int min=0, max=array_nextindex, mid = (min+max)/2;
//cout << "merengue (?)" << endl; cout.flush();
int ii=0;  
  while (true) { //Break out when item is found or binary search is exhausted.
//cout << "ii = " << ii++; cout.flush();   
    reference *ref = (reference *) expanding_array[mid];
//cout << ". (" << mid << ")"; cout.flush();
    int cmp = strcmp(s, ref->id);
//cout << "[" << s << " " << ref->id << "]*" << endl; cout.flush();
    if (!cmp)//{ cout << "ref = " << ref->id << endl; cout.flush();
      return ref->ref;//}
    else {
      int mid0 = mid;
      if (cmp > 0) {
        min = mid;
        mid = (mid+max)/2;
      } else {
        max = mid;
        mid = (min+mid)/2;
      }
      if (mid == mid0)//{ cout << "whassup" << endl; cout.flush();
        return NULL;//}
    }
  }
}



void lookup_table::add(class item *ref0, char *id0)
{
//cout << "inserting \"" << id0 << "\"" << endl; cout.flush();
  void *newref = (void *) new reference(ref0, id0);
  if (!array_nextindex) {
    append(newref);
    return;
  }

  int min=0, max=array_nextindex, mid = (min+max)/2;
  while (true) { //Break out when item is found or binary search is exhausted.
    reference *ref = (reference *) expanding_array[mid];
    int cmp = strcmp(id0, ref->id);
    if (!cmp)
      return;  //(this reference is already occupied)
    else {
      int mid0 = mid;
      if (cmp > 0) {// cout << ">: " << mid << " (" << ref->id << ")" << endl;
        min = mid;
        mid = (mid+max)/2;
      } else {// cout << "<: " << mid << " (" << ref->id << ")" << endl;
        max = mid;
        mid = (min+mid)/2;
      }
      if (mid == mid0) {
	if (cmp < 0)
	  insert_before(mid, newref);
	else
	  insert_after(mid, newref);
        return;
      }
    }
  }
}



/**
 * void lookup_table::serialize()
 *
 * For debugging purposes.
 */
void lookup_table::serialize()
{
  cout << '\t' << id() << ")\t" << array_nextindex << " {";
  for (int i=0; i<array_nextindex; i++) {
    cout << " " << ((reference *) expanding_array[i])->id;
    if (i < (array_nextindex-1))
      cout << " <";
  }
  cout << " }\n";
}
  


/**
 * void lookup_table::serialize(ofstream &out)
 *
 *
 */
void lookup_table::serialize(ofstream &out)
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> void lookup_table::serialize(ofstream &): " << array_nextindex << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t" << array_nextindex << " {";
  for (int i=0; i<array_nextindex; i++) {
    out << " ";
    ((reference *) expanding_array[i])->serialize(out);
    if (i < (array_nextindex-1))
      out << " <";
  }
  out << " }\n";
}



/**
 * void lookup_table::deserialize(ifstream &in)
 *
 *
 */
void lookup_table::deserialize(ifstream &in)
{
  char *s = new char[MAX_STRLEN];
  int size0;
  in >> s >> size0 >> s;
  if (size0 > 0)
    resize(size0);

  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> void lookup_table::deserialize(ifstream &): " << size0 << endl;
    cout.flush();
  #endif

  for (int i=0; i<size0; i++) {
//cout << "i = " << i << endl; cout.flush();
    in >> s;
    char *id = ascii::read(s);
    in >> s;
    item *ref = (item *) item_universe->reference(s);
    append((void *) new reference(ref, id));
/*char *ch = (char *) expanding_array;
for (int i=0; i<30; i++) {
  cout << *ch << ">";
  ch--;
}
cout << endl; cout.flush();
cout << id << ": "; cout.flush();
for (int jj=0; jj<array_nextindex; jj++) {
  cout << "[" << ((reference *) expanding_array[jj])->id << "]";
  cout.flush();
}
cout << ": " << array_size << endl; cout.flush();*/
    delete id;
    in >> s;
//cout << "finished: " << i << endl; cout.flush();
  }
  delete s;
//cout << "now hear this" << endl; cout.flush();
}



/*- end of file --------------------------------------------------------------*/
