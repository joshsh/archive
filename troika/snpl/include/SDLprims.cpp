#include "SDLprims.h"



SDL_Rect *new_SDL_Rect(int *x, int *y, int *w, int *h)
{
  SDL_Rect *r = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  r->x = *x;
  r->y = *y;
  r->w = *w;
  r->h = *h;
  return r;
}



SDL_Surface *new_SDL_Surface(int *hdim0, int *vdim0)
{
  int hdim = *hdim0, vdim = *vdim0;

  SDL_Surface *screen;
  if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
  {
    printf("SDL Oops: %s\n", SDL_GetError());
    return NULL;
  }

  //Note: the 32 represents "32 bits per pixel"
  screen = SDL_SetVideoMode(hdim,vdim,32,SDL_SWSURFACE|SDL_HWPALETTE) ;
  /* do this now so we can use the surface info to set transparency */
  if ( screen == NULL )
  {
    printf("Unable to set %dx%d video: %s\n", hdim, vdim, SDL_GetError());
    return NULL;
  }
//cout << "screen = " << (int) screen << endl;
  return screen;
}
