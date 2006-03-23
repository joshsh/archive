#ifndef _SK_DEBUG_H
#define _SK_DEBUG_H	1



#define SK_DEBUG



#ifdef SK_DEBUG
  #define PRINTF printf
#else
  #define PRINTF //
#endif



#endif
