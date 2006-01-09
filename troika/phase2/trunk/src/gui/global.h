#ifndef global_h
#define global_h


#include "compile-target.h"  // Macros specific to the target architecture.


////////////////////////////////////////////////////////////////////////////////

// Arbitrary out-of-range integer.
#define INT__INFINITY  100000


////////////////////////////////////////////////////////////////////////////////

#include "P2Environment.h"

/** \return  a global P2Environment variable */
extern P2Environment *environment();


// Debugging ///////////////////////////////////////////////////////////////////

#define DEBUG 1

#ifdef DEBUG

    #include <iostream.h>

#endif   // DEBUG

#define DISABLE_FOCUS


// Display settings ////////////////////////////////////////////////////////////

/** All child widgets inherit this background color, rather than defining their
    own. */
#define BACKGROUND_COLOR    0xFF, 0xFF, 0xFF

#define SL5600__DISPLAY_WIDTH  240
#define SL5600__DISPLAY_HEIGHT  320


// Tool bar ////////////////////////////////////////////////////////////////////

// #define TOOLBAR__FILE_ACTIONS
#define TOOLBAR__EDIT_ACTIONS
#define TOOLBAR__VIEW_ACTIONS
// #define TOOLBAR__HELP_ACTIONS

#ifdef TOOLBAR__FILE_ACTIONS
    #define TOOLBAR__FILE__NEW
    #define TOOLBAR__FILE__OPEN
    #define TOOLBAR__FILE__SAVE
    #define TOOLBAR__FILE__SAVE_AS
    #define TOOLBAR__FILE__CLOSE
    #define TOOLBAR__FILE__PRINT
    // #define TOOLBAR__FILE__QUIT
#endif

#ifdef TOOLBAR__EDIT_ACTIONS
    // #define TOOLBAR__EDIT__UNDO
    // #define TOOLBAR__EDIT__REDO
    #define TOOLBAR__EDIT__CUT
    #define TOOLBAR__EDIT__COPY
    #define TOOLBAR__EDIT__PASTE
    // #define TOOLBAR__EDIT__DELETE
    // #define TOOLBAR__EDIT__SELECT_ALL
    // #define TOOLBAR__EDIT__FIND
#endif

#ifdef TOOLBAR__VIEW_ACTIONS
    #define TOOLBAR__VIEW__COMMAND_LINE
    #define TOOLBAR__VIEW__BACK
    #define TOOLBAR__VIEW__FORWARD
    // #define TOOLBAR__VIEW__SHOW_FRAMES
#endif

#ifdef TOOLBAR__HELP_ACTIONS
    #define TOOLBAR__HELP__MANUAL
    #define TOOLBAR__HELP__ABOUT_PHASE2
#endif


// File paths //////////////////////////////////////////////////////////////////

/** Identifier for the Phase2 window icon. */
#define P2GUI_ICON    ":/troika.png"
//#define P2GUI_ICON    ":/p2gui.png"


#endif  // global_h

