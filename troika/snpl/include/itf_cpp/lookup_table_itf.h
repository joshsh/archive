// Reference-hole-friendly; de-/serializes without restrictions.

#ifndef LOOKUP_TABLE_ITF_H
#define LOOKUP_TABLE_ITF_H

#include "../util/lookup_table.h"
#include "../snpl.h"  // ^ BREADTH_FIRST_SERIALIZATION
using namespace std;



//Temporary wrapper functions
void *SNPL_lookup_table_lookup(char *arg0, SNPL_lookup_table *arg1) {
  return ((SNPL_lookup_table *) arg1)->lookup(arg0); }
void SNPL_lookup_table_add(void *arg0, char *arg1, SNPL_lookup_table *arg2) {
  ((SNPL_lookup_table *) arg2)->add(arg0, arg1); }
void SNPL_lookup_table_remove(char *arg0, SNPL_lookup_table *arg1) {
  ((SNPL_lookup_table *) arg1)->remove(arg0); }



void SNPL_lookup_table_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_lookup_table_deserializing_constructor(ifstream &);
void SNPL_lookup_table_destructor(void *);
void SNPL_lookup_table_preserializer(void *, SNPL_array *);



void SNPL_lookup_table_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> void SNPL_lookup_table_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif
  
  SNPL_lookup_table *lt = (SNPL_lookup_table *) p;
  for (int i=0; i<lt->size(); i++) {
    void *ref = ((reference *) lt->get(i))->ref;
    #ifdef BREADTH_FIRST_SERIALIZATION
      data_stack->enqueue(ref);
    #else
      data_stack->push(ref);
    #endif 
  }
//cout << "done preserializing (lookup_table)" << endl; cout.flush();
}



void SNPL_lookup_table_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> void SNPL_lookup_table_serializer(void *, SNPL_associative_array *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_lookup_table *lt = (SNPL_lookup_table *) p;
  int filled = lt->size();
  out << filled << " "
      << lt->expansion() << " {\n";

  for (int i=0; i<filled; i++)
  {
    reference *ref = (reference *) lt->get(i);
    out << "\t\t"
        << (int) type_indices->lookup(
	   (void *) SNPL_find_type(ref->ref)) - 1
	<< "."
	<< (int) item_indices->lookup(ref->ref) - 1
	<< "\t";
    SNPL_write_cstring(out, ref->id);	
    out << "\n";
  }
  out << "\t}";
}



// This could be tricky...
void *SNPL_lookup_table_deserializing_constructor(ifstream &in)
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> void *SNPL_lookup_table_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[MAX_STRLEN], *s2 = new char[MAX_STRLEN];
  int size;
  double expansion;
  in >> size >> expansion >> s;
  SNPL_lookup_table *lt
    = new SNPL_lookup_table(size, expansion);

  for (int i=0; i<size; i++) {
    in >> s;
    void *ref = SNPL_decode_item_reference(s);
    in >> s2;
    char *id = SNPL_read_cstring(s2);
    if (ref != NULL)
      lt->add(ref, id);
    else {
      lt->add((void *) 1, id);  // A lookup_table won't take a NULL reference.
//cout << "id = " << id << ", s = " << s << endl; cout.flush();
      SNPL_add_reference_hole(s, (void **) lt->lookup_location(id));    
    }
    delete id;
  }
  in >> s;

  delete s, s2;

  return (void *) lt;
}



void SNPL_lookup_table_destructor(void *p)
{
  #ifdef LOOKUP_TABLE_DEBUG
    cout << "<> void SNPL_lookup_table_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  /*
    Don't worry about the actual data items in the lookup_table's references;
    they are assumed to be first-class values in the data set, and
    consequently will be deleted individually.
  */
  delete (SNPL_lookup_table *) p;
}



#endif
