#include <P2Environment.h>


////////////////////////////////////////////////////////////////////////////////


P2Environment::P2Environment( bool quiet )
  : QObject( 0 ),
    clipboardObject( 0 )
{
    #ifdef DEBUG
        cout << "P2Environment[" << (int) this << "]::P2Environment()" << endl;
    #endif

    idleFramesAreVisible = true;
    namesAreVisible = true;

    frameContentsSpacing = FRAME__CONTENTS__SPACING;
    frameContentsPadding = FRAME__CONTENTS__PADDING;

    env = environment__new();
    compiler = compiler__new( env, quiet ? 1 : 0 );
}


P2Environment::P2Environment( const P2Environment &other )
    : QObject( 0 )
{
    #ifdef DEBUG
        cout << "P2Environment[" << (int) this << "]::P2Environment("
             << (int) &other << ")" << endl;
    #endif

    idleFramesAreVisible = other.idleFramesAreVisible;
    namesAreVisible = other.namesAreVisible;

    env = environment__new();
}


P2Environment::~P2Environment()
{
    compiler__delete( compiler );
    environment__delete( env );
}


////////////////////////////////////////////////////////////////////////////////


Environment *P2Environment::getEnv() const
{
    return env;
}


Compiler *P2Environment::getCompiler() const
{
    return compiler;
}


////////////////////////////////////////////////////////////////////////////////


bool P2Environment::getIdleFrameVisibility() const
{
    return idleFramesAreVisible;
}


void P2Environment::setIdleFrameVisibility( bool v )
{
    idleFramesAreVisible = v;
    emit changed();
}


bool P2Environment::getNameVisibility() const
{
    return namesAreVisible;
}


void P2Environment::setNameVisibility( bool v )
{
    namesAreVisible = v;
    emit changed();
}


int P2Environment::getFrameContentsSpacing() const
{
    return frameContentsSpacing;
}


int P2Environment::getFrameContentsPadding() const
{
    return frameContentsPadding;
}


void P2Environment::setFrameContentsSpacing( int spacing )
{
    frameContentsSpacing = spacing;
    emit changed();
}


void P2Environment::setFrameContentsPadding( int padding )
{
    frameContentsPadding = padding;
    emit changed();
}


////////////////////////////////////////////////////////////////////////////////


const Object *P2Environment::getClipboardObject()
{
    return clipboardObject;
}


void P2Environment::setClipboardObject( const Object *o )
{
    clipboardObject = o;

    emit hasClipboardObject( ( o ) );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
