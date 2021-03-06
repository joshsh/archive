#include <widgets/bindings/P2XPMWidget.h>


////////////////////////////////////////////////////////////////////////////////


static QImage
getImage( const Object *o )
{
    char buffer[ENCODING__BUFFER_SIZE];
    object__encode( o, buffer );

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


P2XPMWidget::P2XPMWidget( const Object *o )
    : P2Image( getImage( o ) )
{
    //...
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
