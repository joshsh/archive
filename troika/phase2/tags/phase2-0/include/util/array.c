/*+
 * array.c
 *
 * An expanding array of void pointers, suitable for use as a stack or a queue.
 * You can even use it as a general ordered list, although there is memory
 * copying overhead associated with insertions and removals.
 *
 * author: Joshua Shinavier
 * last edited: 1/20/05
 */

#include "array.h"

// By default, array__expand() doubles the size of the array.
// Note: the expansion factor does not need to be an integer.
#define DEFAULT_EXPANSION_FACTOR 2

void array__expand(ARRAY *a);



ARRAY *array__new(int buffer_size, double expansion)
{
  ARRAY *a = (ARRAY *) malloc(sizeof(ARRAY));
  if (expansion <= 1)
    a->expansion = DEFAULT_EXPANSION_FACTOR;
  else
    a->expansion = expansion;
  if (buffer_size < 1)
    a->buffer_size = 1;
  else
    a->buffer_size = buffer_size;
  a->head = 0;
  a->size = 0;
  a->buffer = (void **) malloc(a->buffer_size * sizeof(void *));
  return a;
}



void array__delete(ARRAY *a)
{
  free(a->buffer);
  free(a);
}



/** Array resizing *//*========================================================*/



void array__expand(ARRAY *a)
{
  void **buffer0;
  int i, size0 = (int) (a->expansion * a->buffer_size);

  // If the the array's own exansion factor is too close to 1 to resize the
  // buffer, use DEFAULT_EXPANSION_FACTOR instead.
  if (size0 <= a->buffer_size)
    size0 = DEFAULT_EXPANSION_FACTOR * a->buffer_size;

  buffer0 = (void **) malloc(size0 * sizeof(void *));
  for (i=0; i<a->size; i++)
    buffer0[i] = a->buffer[(a->head+i)%a->buffer_size];
  free(a->buffer);
  a->buffer = buffer0;
  a->head = 0;
  a->buffer_size = size0;
}



void array__minimize(ARRAY *a)
{
  int i;
  void **buffer0;

  if (a->size < a->buffer_size) {
    if (!a->size) {
      buffer0 = (void **) malloc(sizeof(void *));
      a->buffer_size = 1;
    } else {
      buffer0 = (void **) malloc(a->size * sizeof(void *));
      for (i=0; i<a->size; i++)
        buffer0[i] = a->buffer[(a->head+i)%a->buffer_size];
      a->buffer_size = a->size;
    }
    free(a->buffer);
    a->buffer = buffer0;
    a->head = 0;
  }
}



/** Random access *//*=========================================================*/



void *array__get(ARRAY *a, int index)
{
  if ((index >=0)&&(index < a->size))
    return a->buffer[(index+a->head)%a->buffer_size];
  else
    return NULL;
}



void array__set(ARRAY *a, int index, void *p)
{
  if ((index >=0)&&(index < a->size))
    a->buffer[(index+a->head)%a->buffer_size] = p;
}



/** Stack and queue operations *//*============================================*/
/*
   push    : pre-decrement head (pre-resize if is_full)
   enqueue : post-increment tail_index (pre-resize if is_full)
   pop     : post-increment head (if array is non-empty)
   "dequeue" : pre-decrement tail_index (if array is non-empty)
*/



void array__push(ARRAY *a, void *p)
{
  if (a->size >= a->buffer_size)
    array__expand(a);  // Note: replaces a->buffer
  a->head = ((a->head-1)+a->buffer_size)%a->buffer_size;
  a->buffer[a->head] = p;
  a->size++;
}



void *array__pop(ARRAY *a)
{
  void *p;

  if (a->size) {
    p = a->buffer[a->head];
    a->head = (a->head+1)%a->buffer_size;
    a->size--;
    return p;
  } else
    return NULL;
}



void array__enqueue(ARRAY *a, void *p)
{
  if (a->size >= a->buffer_size)
    array__expand(a);  // Note: replaces a->buffer
  a->buffer[(a->head+a->size)%a->buffer_size] = p;
  a->size++;
}



void *array__dequeue(ARRAY *a)
{
  if (a->size) {
    a->size--;
    return a->buffer[(a->head+a->size)%a->buffer_size];
  } else
    return NULL;
}



/** Insertion and removal *//*=================================================*/



// Note: memory copying (and occasional array resizing) overhead
void array__insert_before(ARRAY *a, int index, void *p)
{
  int i;

  if ((index >= 0)&&(index < a->size)) {
    if (a->size >= a->buffer_size)
      array__expand(a);  // Note: replaces a->buffer
    for (i=a->size; i>index; i--)
      a->buffer[(a->head+i)%a->buffer_size]
        = a->buffer[(a->head+i-1)%a->buffer_size];
    a->buffer[(a->head+index)%a->buffer_size] = p;
    a->size++;
  }
}



// Note: memory copying (and occasional array resizing) overhead
void array__insert_after(ARRAY *a, int index, void *p)
{
  int i;

  if ((index >= 0)&&(index < a->size)) {
    if (a->size >= a->buffer_size)
      array__expand(a);  // Note: replaces a->buffer
    for (i=a->size; i>(index+1); i--)
      a->buffer[(a->head+i)%a->buffer_size]
        = a->buffer[(a->head+i-1)%a->buffer_size];
    a->buffer[(a->head+index+1)%a->buffer_size] = p;
    a->size++;
  }
}



// Note: O(n) memory copying overhead
void array__remove(ARRAY *a, int index)
{
  int i;

  if ((index >= 0)&&(index < a->size)) {
    for (i=index; i<(a->size-1); i++)
      a->buffer[(a->head+i)%a->buffer_size]
        = a->buffer[(a->head+i+1)%a->buffer_size];
    a->size--;
  }
}



/*
  Removes an array item and replaces it with the last item in the array,
  to avoid memory copying overhead ( O(n) --> O(c) ).
  DON'T USE if the order of your array elements is important.
*/
void array__simple_remove(ARRAY *a, int index)
{
  if ((index >= 0)&&(index < a->size)) {
    a->buffer[(a->head+index)%a->buffer_size]
      = a->buffer[(a->head+a->size-1)%a->buffer_size];
    a->size--;
  }
}



/** Miscellaneous functions *//*==============================================*/



void array__clear(ARRAY *a)
{
  a->size = 0;
  a->head = 0;
}



void array__forall(ARRAY *a, void (*func) (void *))
{
  int i;

  for (i=0; i<a->size; i++)
    func(a->buffer[(a->head+i)%a->buffer_size]);
}



/*- end of file --------------------------------------------------------------*/
