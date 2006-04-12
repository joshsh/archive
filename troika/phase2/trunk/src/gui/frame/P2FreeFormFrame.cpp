#include <QtGui>

#include "P2FreeFormFrame.h"
#include "layout/P2FreeFormLayout.h"


////////////////////////////////////////////////////////////////////////////////


P2FreeFormFrame::P2FreeFormFrame( const P2Environment &env )
    : P2Frame( env )
{
    #ifdef DEBUG__FRAME
        cout << indentPlus()
             << "P2FreeFormFrame[" <<  (int) this << "]::P2FreeFormFrame()" << endl;
    #endif

    // Note: no need to call setLayout after using this constructor.
    P2FreeFormLayout *layout = new P2FreeFormLayout( this );

    connect( layout, SIGNAL( resized() ), this, SLOT( layoutResizedEvent() ) );

    focusChild = 0;

    environment = &env;

setAcceptDrops(true);

    #ifdef DEBUG__FRAME
        indentMinus();
    #endif
}


const QString P2FreeFormFrame::className()
{
    return QString( "P2FreeFormFrame" );
}


////////////////////////////////////////////////////////////////////////////////


void P2FreeFormFrame::addChild( P2Widget *widget, const QPoint &position )
{
    #ifdef DEBUG__FRAME
        cout << indent()
             << "P2FreeFormFrame[" << (int) this << "]::addChild( " << (int) widget << " )" << endl;
    #endif

    // "When you use a layout, you don't need to pass a parent when constructing
    // the child widgets. The layout will automatically reparent the widgets
    // (using QWidget::setParent()) so that they are children of the widget on
    // which the layout is installed.
    widget->setParent( this );

    widget->isDependent = true;

    (( P2FreeFormLayout* ) layout())->add( widget, position );

    //updateGeometry();  //~
}


// Event handling //////////////////////////////////////////////////////////////



bool P2FreeFormFrame::acceptDrops () const
{
    return true;
}


    void P2FreeFormFrame::dragEnterEvent( QDragEnterEvent *event )
    {
        //if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
    }


    void P2FreeFormFrame::dropEvent( QDropEvent *event )
    {
        cout << "Accepting a drop event: " << event->mimeData()->text().toStdString() << endl;

        //textBrowser->setPlainText(event->mimeData()->text());
        //mimeTypeCombo->clear();
        //mimeTypeCombo->addItems(event->mimeData()->formats());

        event->acceptProposedAction();
    }

    // S.a.  dragMoveEvent, dragLeaveEvent


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
