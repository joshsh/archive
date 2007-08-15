// Reference-hole-UNFRIENDLY:
// Make sure that bitmaps are serialized after colors.

#ifndef BITMAP_ITF_H
#define BITMAP_ITF_H

#include "../type/bitmap.h"
#include "../snpl.h"  //~
using namespace std;



void SNPL_bitmap_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_bitmap_deserializing_constructor(ifstream &);
void SNPL_bitmap_destructor(void *);

void SNPL_bitmap_preserializer(void *, SNPL_array *);



void SNPL_bitmap_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef BITMAP_DEBUG
    cout << "<> void SNPL_bitmap_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif

  SNPL_bitmap *b = (SNPL_bitmap *) p;
  SNPL_color *c = b->default_color();
  #ifdef BREADTH_FIRST_SERIALIZATION
    data_stack->enqueue((void *) c);
  #else
    data_stack->push((void *) c);
  #endif
  for (int i=0; i<b->rows(); i++) {
    for (int j=0; j<b->cols(); j++) {
      c = b->cell(i,j);
      #ifdef BREADTH_FIRST_SERIALIZATION
        data_stack->enqueue((void *) c);
      #else
        data_stack->push((void *) c);
      #endif
    }
  }
}



void SNPL_bitmap_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef BITMAP_DEBUG
    cout << "<> void SNPL_bitmap_serializer(void *, SNPL_associative_array *, ofstream &)" << endl;
    cout.flush();
  #endif

  void *color_type = (void *) SNPL_reference_type("color");
  SNPL_bitmap *b = (SNPL_bitmap *) p;
  int rows = b->rows(), cols = b->cols();
  out << rows << ' ' << cols << ' '
      << (int) type_indices->lookup((void *) color_type) - 1
      << "."
      << (int) item_indices->lookup((void *) b->default_color()) - 1
      << " {\n";
  //SNPL_color **cp = b->get_array();
  SNPL_color *c;

  for (int i=0; i<rows; i++) {
    out << "\t\t";
    for (int j=0; j<cols; j++) {
      out << (int) item_indices->lookup((void *) b->cell(i,j)) - 1;
      if (j<cols-1)
        out << '\t';
      else
        out << '\n';
      //cp++;
    }
  }
  out << "\t}";
}



void *SNPL_bitmap_deserializing_constructor(ifstream &in)
{
  #ifdef BITMAP_DEBUG
    cout << "<> void *SNPL_bitmap_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  int rows, cols, color_id;
  char *s = new char[10];
  in >> rows >> cols >> s;
  SNPL_color *c = (SNPL_color *) SNPL_decode_item_reference(s);
  SNPL_bitmap *b = new SNPL_bitmap(c, rows, cols);  
  in >> s;  // Get past the opening bracket.
  SNPL_data_type *color_type
    = SNPL_reference_type("color");

  for (int i=0; i<rows; i++) {
    for (int j=0; j<cols; j++) {
      in >> color_id;
      b->set(i, j,
	(SNPL_color *) SNPL_decode_item_reference(color_type, color_id));
    }
  }

  in >> s;  // (ignore closing bracket)
  delete s;

  return (void *) b;
}



void SNPL_bitmap_destructor(void *p)
{
  //##### remember that some of the bitmap's colors may not
  //      appear elsewhere in the object graph... so how is
  //      the graph destructor going to find them?

  delete (SNPL_bitmap *) p;
}

#endif
