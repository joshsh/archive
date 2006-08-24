#ifndef SDLPRIMS_H
#define SDLPRIMS_H



#include <cstdlib>
#include <SDL/SDL.h>

SDL_Rect *new_SDL_Rect(int *x, int *y, int *w, int *h);
SDL_Surface *new_SDL_Surface(int *hdim0, int *vdim0);



#endif
