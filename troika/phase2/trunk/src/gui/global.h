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

    //#define DEBUG__LAYOUT__FF_LAYOUT_TREE

#endif   // DEBUG

#define DISABLE_FOCUS

/** \return  indentation whitespace for debugging output
    \note  Indentation level is post-incremented. */
const char *indentPlus();

/** \return  indentation whitespace for debugging output */
const char *indent();

/** \return  indentation whitespace for debugging output
    \note  Indentation level is pre-decremented. */
const char *indentMinus();


// Display settings ////////////////////////////////////////////////////////////

#define SL5600__DISPLAY_WIDTH  240
#define SL5600__DISPLAY_HEIGHT  320

/** All child widgets inherit this background color, rather than defining their
    own. */
#define COLOR__BACKGROUND       0xFF, 0xFF, 0xFF

// Full blue.
#define COLOR__FRAME__ACTIVE    0x00, 0x00, 0xFF, 0xFF
// Light blue.
#define COLOR__FRAME__INACTIVE  0xBF, 0xBF, 0xFF, 0xFF


// Layout //////////////////////////////////////////////////////////////////////

/** Number of empty pixels on either side of a frame label. */
#define FRAME__LABEL__X_PADDING     2

/** Minimum x offset of a label from either side of its frame. */
#define FRAME__LABEL__X_MARGIN      7

/** Horizontal alignment of frame labels. */
#define FRAME__LABEL__ALIGNMENT     Qt::AlignLeft
//#define FRAME__LABEL__ALIGNMENT     Qt::AlignHCenter
//#define FRAME__LABEL__ALIGNMENT     Qt::AlignRight

/** Horizontal alignment of frame contents. */
#define FRAME__CONTENTS__ALIGNMENT  Qt::AlignHCenter

#define FRAME__CONTENTS__SPACING    1
#define FRAME__CONTENTS__PADDING    1


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
    // #define TOOLBAR__VIEW__SHOW_NAMES
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

