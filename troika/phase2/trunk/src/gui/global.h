#ifndef global_h
#define global_h


#include "compile-target.h"  // Macros specific to the target architecture.
//#include "P2Environment.h"
//#include <P2Binder.h>

class P2Environment;
class P2Binder;

////////////////////////////////////////////////////////////////////////////////

// Arbitrary out-of-range integer.
#define INT__INFINITY                       100000

#define ENCODING__BUFFER_SIZE               0x10000


////////////////////////////////////////////////////////////////////////////////

/** \note  From main.cpp. */
extern void
newMainWindow( P2Binder &b );


// Debugging ///////////////////////////////////////////////////////////////////

    #include <iostream.h>

//#define DEBUG

#ifdef DEBUG

    #define DEBUG__FRAME
    //#define DEBUG__LAYOUT

    #ifdef DEBUG__LAYOUT

        #define DEBUG__LAYOUT__FF_LAYOUT_TREE

    #endif

#endif   // DEBUG

#define DISABLE_FOCUS

/** \return  indentation whitespace for debugging output
    \note  Indentation level is post-incremented. */
const char *
indentPlus();

/** \return  indentation whitespace for debugging output */
const char *
indent();

/** \return  indentation whitespace for debugging output
    \note  Indentation level is pre-decremented. */
const char *
indentMinus();


// Display settings ////////////////////////////////////////////////////////////

#define SL5600__DISPLAY_WIDTH               240
#define SL5600__DISPLAY_HEIGHT              320

/** All child widgets inherit this background color, rather than defining their
    own. */
#define COLOR__BACKGROUND                   0xFF, 0xFF, 0xFF

// Full blue.
#define COLOR__FRAME__ACTIVE                0x00, 0x00, 0xFF, 0xFF
#define COLOR__TITLE__ACTIVE                0x00, 0x00, 0xFF, 0xFF

// Light blue.
#define COLOR__FRAME__INACTIVE              0xA0, 0xA0, 0xFF, 0xFF
#define COLOR__TITLE__INACTIVE              0x7F, 0x7F, 0xFF, 0xFF

// Full orange.
#define COLOR__FRAME__APPLY__ACTIVE         0xFF, 0x80, 0x00, 0xFF
#define COLOR__TITLE__APPLY__ACTIVE         0xFF, 0x80, 0x00, 0xFF

// Light orange.
#define COLOR__FRAME__APPLY__INACTIVE       0xFF, 0xB0, 0x60, 0xFF
#define COLOR__TITLE__APPLY__INACTIVE       0xFF, 0x90, 0x40, 0xFF


// Phase2 bindings /////////////////////////////////////////////////////////////

#define CHAR__COLOR                         0xBF, 0xBF, 0xFF, 0xFF
#define DOUBLE__COLOR                       0xBF, 0x00, 0xBF, 0xFF
#define COMBINATOR__COLOR                   0xFF, 0x00, 0x00, 0xFF
#define INT__COLOR                          0X00, 0xFF, 0x00, 0xFF
#define PRIM__COLOR                         0xFF, 0x7F, 0x00, 0xFF
#define STRING__COLOR                       0x3F, 0x3F, 0x3F, 0xFF
#define TERM__COLOR                         0xFF, 0x00, 0x00, 0xFF
#define TYPE__COLOR                         0x00, 0xFF, 0xFF, 0xFF
#define XPM__COLOR                          0xFF, 0x00, 0xFF, 0xFF

#define SYNTAX__COLOR                       0x60, 0x60, 0x60, 0xFF


// Layout //////////////////////////////////////////////////////////////////////

#define FRAME__BORDER_THICKNESS             1

/** Number of empty pixels on either side of a frame label. */
#define FRAME__LABEL__X_PADDING             3

/** Minimum x offset of a label from either side of its frame. */
#define FRAME__LABEL__X_MARGIN              7

/** Horizontal alignment of frame labels. */
#define FRAME__LABEL__ALIGNMENT             Qt::AlignLeft
//#define FRAME__LABEL__ALIGNMENT           Qt::AlignHCenter
//#define FRAME__LABEL__ALIGNMENT           Qt::AlignRight

/** Horizontal alignment of frame contents. */
#define FRAME__CONTENTS__ALIGNMENT          Qt::AlignHCenter

#define FRAME__CONTENTS__SPACING            3

/** 5 pixels is the minimum if a P2PlusMinus is 5 pixels wide */
#define FRAME__CONTENTS__PADDING            5

#define WINDOW__CONTENTS__PADDING           5

/** If defined, use a weighted average rather than a best fit approach to
    edge alignment. */
//#define LAYOUT__FF__WEIGHTED_ALIGNMENT


// Tool bar ////////////////////////////////////////////////////////////////////

#define TOOLBAR                             1

#define TOOLBAR__FILE                       0

#define TOOLBAR__FILE__NEW                  1
#define TOOLBAR__FILE__OPEN                 1
#define TOOLBAR__FILE__SAVE                 1
#define TOOLBAR__FILE__SAVE_AS              1
#define TOOLBAR__FILE__CLOSE                1
#define TOOLBAR__FILE__PRINT                1
#define TOOLBAR__FILE__QUIT                 0

#define TOOLBAR__EDIT                       1

#define TOOLBAR__EDIT__UNDO                 0
#define TOOLBAR__EDIT__REDO                 0
#define TOOLBAR__EDIT__CUT                  1
#define TOOLBAR__EDIT__COPY                 1
#define TOOLBAR__EDIT__PASTE                1
#define TOOLBAR__EDIT__DELETE               0
#define TOOLBAR__EDIT__RENAME               0
#define TOOLBAR__EDIT__SELECT_ALL           0
#define TOOLBAR__EDIT__FIND                 0

#define TOOLBAR__VIEW                       1

#define TOOLBAR__VIEW__COMMAND_LINE         1
#define TOOLBAR__VIEW__BACK                 1
#define TOOLBAR__VIEW__FORWARD              1
#define TOOLBAR__VIEW__NEW_WINDOW           0
#define TOOLBAR__VIEW__SPLIT_HORIZONTAL     0
#define TOOLBAR__VIEW__SPLIT_VERTICAL       0
#define TOOLBAR__VIEW__CLOSE_CURRENT_VIEW   0
#define TOOLBAR__VIEW__SHOW_FRAMES          0
#define TOOLBAR__VIEW__SHOW_NAMES           0

#define TOOLBAR__HELP                       0

#define TOOLBAR__HELP__MANUAL               1
#define TOOLBAR__HELP__ABOUT_PHASE2         1


// File paths //////////////////////////////////////////////////////////////////

/** Identifier for the Phase2 window icon. */
#define P2GUI_ICON                          ":/troika.png"
//#define P2GUI_ICON                        ":/p2gui.png"


#endif  // global_h


////////////////////////////////////////////////////////////////////////////////

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
