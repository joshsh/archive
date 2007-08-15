#include <P2Environment.h>


////////////////////////////////////////////////////////////////////////////////


P2Environment::P2Environment( bool quiet )
  : QObject( 0 ),
    clipboardObject( 0 )
{
    idleFramesAreVisible = false;
    namesAreVisible = true;

    frameContentsSpacing = FRAME__CONTENTS__SPACING;
    frameContentsPadding = FRAME__CONTENTS__PADDING;

    env = environment__new();
    compiler = interpreter__new( env, quiet ? TRUE : FALSE, FALSE );
}


P2Environment::P2Environment( const P2Environment &other )
    : QObject( 0 )
{
    idleFramesAreVisible = other.idleFramesAreVisible;
    namesAreVisible = other.namesAreVisible;

    env = environment__new();
}


P2Environment::~P2Environment()
{
    interpreter__free( compiler );
    environment__free( env );
}


////////////////////////////////////////////////////////////////////////////////


Environment *P2Environment::getEnv() const
{
    return env;
}


Interpreter *P2Environment::getInterpreter() const
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
