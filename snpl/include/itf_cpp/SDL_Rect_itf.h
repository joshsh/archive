#ifndef SDL_RECT_ITF_H
#define SDL_RECT_ITF_H

#include <SDL/SDL.h>
#include "../snpl.h"  // For debugging macro.
using namespace std;



void SDL_Rect_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SDL_Rect_deserializing_constructor(ifstream &);
void SDL_Rect_destructor(void *);



void SDL_Rect_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef SDL_RECT_DEBUG
    cout << "<> void SDL_Rect_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  SDL_Rect *r = (SDL_Rect *) p;
  out << r->x << " " << r->y << " " << r->w << " " << r->h;
}



void *SDL_Rect_deserializing_constructor(ifstream &in)
{
  #ifdef SDL_RECT_DEBUG
    cout << "<> void *SDL_Rect_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  SDL_Rect *r = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  in >> r->x >> r->y >> r->w >> r->h;
  return (void *) r;
}



void SDL_Rect_destructor(void *p)
{
  #ifdef SDL_RECT_DEBUG
    cout << "<> void SDL_Rect_destructor(void *)" << endl;
    cout.flush();
  #endif

  free((SDL_Rect *) p);
}

#endif
