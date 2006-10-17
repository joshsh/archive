#include <P2TermWidgetNew.h>
#include <P2Text.h>

extern "C"
{
#include <phase2/Term.h>
}


////////////////////////////////////////////////////////////////////////////////


P2TermWidgetNew::P2TermWidgetNew( const Object *o, P2Binder *eb )
    : P2BoxArray( QBoxLayout::LeftToRight )
{
    encode( o, eb );
}


static QString
getText( const Object *o )
{
    char buffer[1000];
    object__encode( o, buffer );
    return QString( buffer );
}


void
P2TermWidgetNew::appendObjectWidget( const Object *o, P2Binder *eb )
{
    QString name = eb->objectName( o );

    P2Widget *w = ( name.length() )
        ? new P2Text( name, eb->objectColor( o ) )
        : eb->objectWidget( o );

    add( w );
}


void
P2TermWidgetNew::appendSyntaxWidget( char *s )
{
    add( new P2Text( QString( s ), QColor( SYNTAX__COLOR ) ) );
}


static void
encode_static( Term *t, bool delimit, P2TermWidgetNew *tw, P2Binder *eb )
{
    /* If the sub-term represents a leaf node, execute the procedure. */
    if ( term__length( t ) == 1 )
        tw->appendObjectWidget( ( Object* ) term__head( t ), eb );
/*
    if ( ( unsigned int ) *cur == 2 )
    {
        cur++;

        tw->appendObjectWidget( ( Object* ) *cur, eb );
    }
*/

    /* If the sub-term contains further sub-terms, recurse through them. */
    else
    {
        if ( delimit )
            tw->appendSyntaxWidget( "(" );

        for ( unsigned int i = 0; i < term__length( t ); i++ )
        {
            Term *subt = term__subterm_at( t, i );
            encode_static( subt, true, tw, eb );
            term__free( subt );
        }
/*
        void **lim = cur + ( unsigned int ) *cur;
        cur++;
        while ( cur < lim )
        {
            encode_static( cur, true, tw, eb );

            cur += ( unsigned int ) *cur;

            //if ( cur < lim )
            //    tw->appendSyntaxWidget( " " );
        }
*/

        if ( delimit )
            tw->appendSyntaxWidget( ")" );
    }
}


void
P2TermWidgetNew::encode( const Object *o, P2Binder *eb )
{
    Term *t = ( Term* ) object__value( o );

    //if ( DEBUG__SAFE && ( !t || !buffer )
    //    abort();

    appendSyntaxWidget( "[" );

    encode_static( t, false, this, eb );

    appendSyntaxWidget( "]" );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
