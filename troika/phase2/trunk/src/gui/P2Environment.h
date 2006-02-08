#ifndef P2Environment_h
#define P2Environment_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class P2Environment : public QObject
{
    Q_OBJECT

public:

    /** Initializes the P2Environment with default values. */
    P2Environment();

    /** Copy constructor. */
    P2Environment( const P2Environment &other );

    bool getIdleFrameVisibility() const;
    void setIdleFrameVisibility( bool v );

    bool getNameVisibility() const;
    void setNameVisibility( bool v );

    int getFrameContentsSpacing() const;
    int getFrameContentsPadding() const;
    void setFrameContentsSpacing( int spacing );
    void setFrameContentsPadding( int padding );

signals:

    void changed();

private:

    bool namesAreVisible;
    bool idleFramesAreVisible;

    int frameContentsSpacing;
    int frameContentsPadding;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Environment_h
