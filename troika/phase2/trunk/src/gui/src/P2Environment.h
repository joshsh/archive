#ifndef P2Environment_h
#define P2Environment_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Compiler.h>
}

#include <global.h>


////////////////////////////////////////////////////////////////////////////////

class P2Environment : public QObject
{
    Q_OBJECT

public:

    /** Initializes the P2Environment with default values. */
    P2Environment( bool quiet );

    /** Copy constructor. */
    P2Environment( const P2Environment &other );
    ~P2Environment();

    Environment *getEnv() const;
    Compiler *getCompiler() const;

    bool getIdleFrameVisibility() const;
    void setIdleFrameVisibility( bool v );

    bool getNameVisibility() const;
    void setNameVisibility( bool v );

    int getFrameContentsSpacing() const;
    int getFrameContentsPadding() const;
    void setFrameContentsSpacing( int spacing );
    void setFrameContentsPadding( int padding );

    const Object *getClipboardObject();
    void setClipboardObject( const Object *o );

signals:

    void changed();

    void hasClipboardObject( bool b );

private:

    bool namesAreVisible;
    bool idleFramesAreVisible;

    int frameContentsSpacing;
    int frameContentsPadding;

    Environment *env;
    Compiler *compiler;

    const Object *clipboardObject;
};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2Environment_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
