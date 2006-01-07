#include "P2Environment.h"


////////////////////////////////////////////////////////////////////////////////


P2Environment::P2Environment()
{
    #ifdef DEBUG
        cout << "P2Environment[" << (int) this << "]::P2Environment()" << endl;
    #endif

    idleFramesAreVisible = false;
    viewMode = layoutMode;
}


P2Environment::ViewMode P2Environment::getViewMode()
{
    return viewMode;
}


void P2Environment::setViewMode( P2Environment::ViewMode viewMode )
{
    this->viewMode = viewMode;
}

