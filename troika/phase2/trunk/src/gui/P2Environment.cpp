#include "P2Environment.h"


////////////////////////////////////////////////////////////////////////////////


P2Environment::P2Environment()
    : QObject( 0 )
{
    #ifdef DEBUG
        cout << "P2Environment[" << (int) this << "]::P2Environment()" << endl;
    #endif

    idleFramesAreVisible = true;
    viewMode = layoutMode;
    namesAreVisible = true;
}


P2Environment::P2Environment( const P2Environment &other )
    : QObject( 0 )
{
    #ifdef DEBUG
        cout << "P2Environment[" << (int) this << "]::P2Environment("
             << (int) &other << ")" << endl;
    #endif

    idleFramesAreVisible = other.idleFramesAreVisible;
    viewMode = other.viewMode;
    namesAreVisible = other.namesAreVisible;
}


P2Environment::ViewMode P2Environment::getViewMode() const
{
    return viewMode;
}


void P2Environment::setViewMode( P2Environment::ViewMode viewMode )
{
    this->viewMode = viewMode;
    emit changed();
}


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


int P2Environment::getFrameSpacing() const
{
    return frameSpacing;
}


int P2Environment::getFramePadding() const
{
    return framePadding;
}


void P2Environment::setFrameSpacing( int spacing )
{
    frameSpacing = spacing;
    emit changed();
}


void P2Environment::setFramePadding( int padding )
{
    framePadding = padding;
    emit changed();
}

