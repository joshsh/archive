#ifndef P2Environment_h
#define P2Environment_h

////////////////////////////////////////////////////////////////////////////////

//#include "global.h"

////////////////////////////////////////////////////////////////////////////////

struct P2Environment
{

public:

    enum ViewMode { layoutMode, commandLineMode };

    bool idleFramesAreVisible;

    /** Initializes the P2Environment with default values. */
    P2Environment();

    ViewMode getViewMode();
    void setViewMode( ViewMode viewMode );

private:

    ViewMode viewMode;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Environment_h
