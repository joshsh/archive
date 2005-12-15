#ifndef global_h
#define global_h


#include "compile-target.h"  // Macros specific to the target architecture.

#include "P2Environment.h"

/** \return  a global P2Environment variable */
extern P2Environment *environment();


// Debugging ///////////////////////////////////////////////////////////////////

#define DEBUG 1

#ifdef DEBUG

    #include <iostream.h>

#endif   // DEBUG


// Display settings ////////////////////////////////////////////////////////////

/** All child widgets inherit this background color, rather than defining their
    own. */
#define BACKGROUND_COLOR    0xFF, 0xFF, 0xFF


// File paths //////////////////////////////////////////////////////////////////

/** Identifier for the Phase2 window icon. */
#define P2GUI_ICON    ":/troika.png"
//#define P2GUI_ICON    ":/p2gui.png"


#endif  // global_h

