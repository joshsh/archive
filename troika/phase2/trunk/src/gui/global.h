#ifndef global_h
#define global_h

#include "compile-target.h"  // Macros specific to the target architecture.


// Debugging ///////////////////////////////////////////////////////////////////

#define DEBUG 1

#ifdef DEBUG

    #include <iostream.h>

#endif   // DEBUG


// Display settings ////////////////////////////////////////////////////////////

/** All child widgets inherit this background color, rather than defining their
    own. */
#define BACKGROUND_COLOR        0xFF, 0xFF, 0xFF


#endif  // global_h
