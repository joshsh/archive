#ifndef P2Environment_h
#define P2Environment_h

////////////////////////////////////////////////////////////////////////////////

#include "global.h"

#include <QtGui>

extern "C"
{
#include <Compiler.h>
}


////////////////////////////////////////////////////////////////////////////////

class P2Environment : public QObject
{
    Q_OBJECT

public:

    /** Initializes the P2Environment with default values. */
    P2Environment();

    /** Copy constructor. */
    P2Environment( const P2Environment &other );

    ~P2Environment();

    bool getIdleFrameVisibility() const;
    void setIdleFrameVisibility( bool v );

    bool getNameVisibility() const;
    void setNameVisibility( bool v );

    int getFrameContentsSpacing() const;
    int getFrameContentsPadding() const;
    void setFrameContentsSpacing( int spacing );
    void setFrameContentsPadding( int padding );

    Environment *getEnv() const { return env; }
    Compiler *getCompiler() const { return compiler; }

signals:

    void changed();

private:

    bool namesAreVisible;
    bool idleFramesAreVisible;

    int frameContentsSpacing;
    int frameContentsPadding;

    Environment *env;
    Compiler *compiler;

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Environment_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
