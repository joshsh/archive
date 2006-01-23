#ifndef P2Environment_h
#define P2Environment_h

////////////////////////////////////////////////////////////////////////////////

//#include "global.h"

////////////////////////////////////////////////////////////////////////////////

struct P2Environment
{

public:

    enum ViewMode { layoutMode, commandLineMode };

    /** Initializes the P2Environment with default values. */
    P2Environment();

    ViewMode getViewMode();
    void setViewMode( ViewMode viewMode );

    bool getIdleFrameVisibility();
    void setIdleFrameVisibility( bool v );

    bool getNameVisibility();
    void setNameVisibility( bool v );

private:

    bool namesAreVisible;
    bool idleFramesAreVisible;

    ViewMode viewMode;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Environment_h
