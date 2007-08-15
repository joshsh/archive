#include <QtGui>

extern "C"
{
#include <phase2/Set.h>
#include <phase2/Array.h>
}

#include <widgets/bindings/P2SetWidget.h>
#include <widgets/P2ObjectFrame.h>


////////////////////////////////////////////////////////////////////////////////


/*
static Array *
set__to_array( Set *s )
{
    Array *a = array__new( 0, 0 );
    void *helper( void **refp )
    {
        array__enqueue( a, *refp );
        return 0;
    }

    set__walk( s, ( Dist_f ) helper );
    return a;
}
*/


/*
{
    class Helper
    {
        Array *a;

    public:

        Helper()
            : a( array__new( 0, 0 ) ) {}

        void *dist( void **refp )
        {
            array__enqueue( a, *refp );
            return 0;
        }

        Array *as_array( Set *s )
        {
            set__walk( s, ( Dist_f ) &dist );
            return a;
        }
    };

    Helper helper();
    return helper.as_array( s );
}
*/


P2SetWidget::P2SetWidget( const Object *o, P2Binder *eb )
    : P2BoxArray( QBoxLayout::LeftToRight )
{
    Set *s = ( Set* ) object__value( o );

    Array *a = set__to_array( s );
    int size = array__size( a );

    for ( int i = 0; i < size; i++ )
    {
        Object *child = ( Object* ) array__get( a, i );
        P2ObjectFrame *f = new P2ObjectFrame( child, 0, *eb );
        add( f );
    }

    array__free( a );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
