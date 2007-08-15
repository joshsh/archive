#ifndef SDL_SURFACE_ITF_H
#define SDL_SURFACE_ITF_H

#include "../type/gui.h"
#include "../snpl.h"
#include "../SDLprims.h"
using namespace std;



void SDL_Surface_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SDL_Surface_deserializing_constructor(ifstream &);
void SDL_Surface_destructor(void *);



void SDL_Surface_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef SDL_SURFACE_DEBUG
    cout << "<> void SDL_Surface_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  SDL_Surface *screen = (SDL_Surface *) p;
  out << screen->w << " " << screen->h;
}



void *SDL_Surface_deserializing_constructor(ifstream &in)
{
  #ifdef SDL_SURFACE_DEBUG
    cout << "<> void *SDL_Surface_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif

  int w, h;
  in >> w >> h;
  return (void *) new_SDL_Surface(&w, &h);
}



void SDL_Surface_destructor(void *p)
{
  #ifdef SDL_SURFACE_DEBUG
    cout << "<> void SDL_Surface_destructor(void *)" << endl;
    cout.flush();
  #endif

  // (SDL_Surface is a structure)
  free((SDL_Surface *) p);
}

#endif
