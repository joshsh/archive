#include "P2Environment.h"


////////////////////////////////////////////////////////////////////////////////


P2Environment::P2Environment()
{
    #ifdef DEBUG
        cout << "P2Environment[" << (int) this << "]::P2Environment()" << endl;
        //cout << indentPlus() << "P2Environment[" << (int) this << "]::P2Environment()" << endl;
    #endif

    idleFramesAreVisible = true;
    viewMode = layoutMode;
    namesAreVisible = true;

    contentOffset = QSize( 0, 0 );
}


P2Environment::ViewMode P2Environment::getViewMode()
{
    return viewMode;
}


void P2Environment::setViewMode( P2Environment::ViewMode viewMode )
{
    this->viewMode = viewMode;
}


bool P2Environment::getIdleFrameVisibility()
{
    return idleFramesAreVisible;
}


void P2Environment::setIdleFrameVisibility( bool v )
{
    idleFramesAreVisible = v;
}


bool P2Environment::getNameVisibility()
{
    return namesAreVisible;
}


void P2Environment::setNameVisibility( bool v )
{
    namesAreVisible = v;
}

