#include <widgets/bindings/P2XPMWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QImage
getImage( Object *o )
{
    char buffer[ENCODING__BUFFER_SIZE];
    object__type( o )->encode( object__value( o ), buffer );

    QTemporaryFile file;
    if ( file.open() )
    {
        file.write( buffer, strlen( buffer ) );
        //file.close();

        return QImage( file.fileName() );
    }

    else
        return QImage();
}


P2XPMWidget::P2XPMWidget( Object *o )
    : P2Image( getImage( o ) )
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
