/*+
 * array.h
 *
 * An expanding array of void pointers, suitable for use as a stack or a queue.
 * You can even use it as a general ordered list, although there is memory
 * copying overhead associated with insertions and removals.
 *
 * author: Joshua Shinavier
 * last edited: 1/20/05
 */

#ifndef ARRAY_H
#define ARRAY_H



#include <stdlib.h>

struct JS_array {
  int head, size, buffer_size;
  double expansion;
  void **buffer;
};

#define ARRAY struct JS_array



// Constructor and destructor
ARRAY *array__new(int buffer_size, double expansion);
void array__delete(ARRAY *a);

// Array resizing
// void array__expand(ARRAY *a) is hidden
void array__minimize(ARRAY *a);

// Random access
void *array__get(ARRAY *a, int index);
void array__set(ARRAY *a, int index, void *p);

// Stack and queue operations (note: "dequeue" is nonstandard)
void array__push(ARRAY *a, void *p);
void *array__pop(ARRAY *a);
void array__enqueue(ARRAY *a, void *p);
void *array__dequeue(ARRAY *a);

// Random insertion and removal
// Note: O(n) memory copying overhead for all except simple_remove
void array__insert_before(ARRAY *a, int index, void *p);
void array__insert_after(ARRAY *a, int index, void *p);
void array__remove(ARRAY *a, int index);
void array__simple_remove(ARRAY *a, int index);

// Miscellaneous functions
void array__clear(ARRAY *a);
void array__forall(ARRAY *a, void (*func) (void *));



#endif

/*- end of file --------------------------------------------------------------*/
