#ifndef P2Environment_h
#define P2Environment_h

////////////////////////////////////////////////////////////////////////////////

//#include "global.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2Environment : public QObject
{
    Q_OBJECT

public:

    enum ViewMode { layoutMode, commandLineMode };

    /** Initializes the P2Environment with default values. */
    P2Environment();

    /** Copy constructor. */
    P2Environment( const P2Environment &other );

    /** THESE DON'T BELONG HERE. */
    ViewMode getViewMode() const;
    void setViewMode( ViewMode viewMode );

    bool getIdleFrameVisibility() const;
    void setIdleFrameVisibility( bool v );

    bool getNameVisibility() const;
    void setNameVisibility( bool v );

    int getFrameSpacing() const;
    int getFramePadding() const;
    void setFrameSpacing( int spacing );
    void setFramePadding( int padding );

signals:

    void changed();

private:

    bool namesAreVisible;
    bool idleFramesAreVisible;

    ViewMode viewMode;

    int frameSpacing;
    int framePadding;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Environment_h
