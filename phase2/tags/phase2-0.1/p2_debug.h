#ifndef _P2_DEBUG_H
#define _P2_DEBUG_H 1

//#define DEBUG

#ifdef DEBUG
  #define PRINTF printf
#else
  #define PRINTF //
#endif

#endif
