#ifndef DEBUG_H
#define DEBUG_H

//#define DEBUG

#ifdef DEBUG
  #define PRINTF printf
#else
  #define PRINTF //
#endif

#endif
