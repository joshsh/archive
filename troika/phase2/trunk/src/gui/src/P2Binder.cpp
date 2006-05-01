#include <P2Binder.h>

#include <widgets/basic/P2Text.h>

#include <widgets/bindings/P2BagWidget.h>
#include <widgets/bindings/P2CharWidget.h>
#include <widgets/bindings/P2CombinatorWidget.h>
#include <widgets/bindings/P2DoubleWidget.h>
#include <widgets/bindings/P2IntWidget.h>
#include <widgets/bindings/P2NamespaceWidget.h>
#include <widgets/bindings/P2PrimitiveWidget.h>
#include <widgets/bindings/P2SetWidget.h>
#include <widgets/bindings/P2StringWidget.h>
#include <widgets/bindings/P2TermWidgetNew.h>
//#include <widgets/bindings/P2TermWidget.h>
#include <widgets/bindings/P2TypeWidget.h>
#include <widgets/bindings/P2XPMWidget.h>


////////////////////////////////////////////////////////////////////////////////


static P2Widget *
bagWidget( const Object *o, P2Binder *eb )
{
    return new P2BagWidget( o, eb );
}


static P2Widget *
charWidget( const Object *o, P2Binder *eb )
{
    eb = 0;
    return new P2CharWidget( o );
}


static P2Widget *
combinatorWidget( const Object *o, P2Binder *eb )
{
    eb = 0;
    return new P2CombinatorWidget( o );
}


static P2Widget *
doubleWidget( const Object *o, P2Binder *eb )
{
    eb = 0;
    return new P2DoubleWidget( o );
}


static P2Widget *
intWidget( const Object *o, P2Binder *eb )
{
   eb = 0;
   return new P2IntWidget( o );
}


static P2Widget *
namespaceWidget( const Object *o, P2Binder *eb )
{
    return new P2NamespaceWidget( o, eb );
}


static P2Widget *
primitiveWidget( const Object *o, P2Binder *eb )
{
    eb = 0;
    return new P2PrimitiveWidget( o );
}


static P2Widget *
setWidget( const Object *o, P2Binder *eb )
{
    return new P2SetWidget( o, eb );
}


static P2Widget *
stringWidget( const Object *o, P2Binder *eb )
{
    eb = 0;
    return new P2StringWidget( o );
}


static P2Widget *
termWidget( const Object *o, P2Binder *eb )
{
    return new P2TermWidgetNew( o, eb );
    //eb = 0;
    //return new P2TermWidget( o );
}


static P2Widget *
typeWidget( const Object *o, P2Binder *eb )
{
    eb = 0;
    return new P2TypeWidget( o );
}


static P2Widget *
xpmWidget( const Object *o, P2Binder *eb )
{
    eb = 0;
    return new P2XPMWidget( o );
}


////////////////////////////////////////////////////////////////////////////////


static int
getType( Environment *e, const char *name )
{
    Object *o = environment__resolve_type( e, name );
    return ( int ) object__value( o );
}


static int
getType( const Object *o )
{
    return ( int ) object__type( o );
}


P2Binder::
P2Binder( P2Environment *e )
  : QObject(),
    focusWidget( 0 )
{
    env = e;
    environment = env->getEnv();

    constructors[ getType( environment, "Bag" ) ] = bagWidget;
    constructors[ getType( environment, "char" ) ] = charWidget;
    constructors[ getType( environment, "Combinator" ) ] = combinatorWidget;
    constructors[ getType( environment, "double" ) ] = doubleWidget;
    constructors[ getType( environment, "int" ) ] = intWidget;
    constructors[ getType( environment, "Namespace" ) ] = namespaceWidget;
    constructors[ getType( environment, "Primitive" ) ] = primitiveWidget;
    constructors[ getType( environment, "cstring" ) ] = stringWidget;
    constructors[ getType( environment, "Set" ) ] = setWidget;
    constructors[ getType( environment, "Term" ) ] = termWidget;
    constructors[ getType( environment, "Type" ) ] = typeWidget;
    constructors[ getType( environment, "xpm" ) ] = xpmWidget;

    colors[ getType( environment, "Bag" ) ] = QColor( BAG__COLOR );
    colors[ getType( environment, "char" ) ] = QColor( CHAR__COLOR );
    colors[ getType( environment, "Combinator" ) ] = QColor( COMBINATOR__COLOR );
    colors[ getType( environment, "double" ) ] = QColor( DOUBLE__COLOR );
    colors[ getType( environment, "int" ) ] = QColor( INT__COLOR );
    colors[ getType( environment, "Namespace" ) ] = QColor( NAMESPACE__COLOR );
    colors[ getType( environment, "Primitive" ) ] = QColor( PRIM__COLOR );
    colors[ getType( environment, "cstring" ) ] = QColor( STRING__COLOR );
    colors[ getType( environment, "Set" ) ] = QColor( SET__COLOR );
    colors[ getType( environment, "Term" ) ] = QColor( TERM__COLOR );
    colors[ getType( environment, "Type" ) ] = QColor( TYPE__COLOR );
    colors[ getType( environment, "xpm" ) ] = QColor( XPM__COLOR );
}


P2Environment *P2Binder::
getEnv()
{
    return env;
}


static QString
getText( const Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2Widget *P2Binder::
objectWidget( const Object *o )
{
    objectWidgetConstructor c = constructors[ getType( o ) ];
    if ( !c )
        return new P2Text( getText( o ), objectColor( o ) );
        //return new P2Text( getText( o ), QColor( 0x00, 0x00, 0x00, 0xFF ) );

    else
        return c( o, this );
}


QColor P2Binder::
objectColor( const Object *o )
{
    return colors[ getType( o ) ];
}


////////////////////////////////////////////////////////////////////////////////


P2Widget *P2Binder::
getFocusWidget() const
{
    return focusWidget;
}


void P2Binder::
setFocusWidget( P2Widget *w )
{
    focusWidget = w;
    emit hasFocusWidget( true );
}


void P2Binder::
clearFocusWidget( P2Widget *w )
{
    // Clear the focus only if it has not already been reset to another widget.
    if ( focusWidget == w )
    {
        focusWidget = 0;
        emit hasFocusWidget( false );
    }
}


void P2Binder::
requestObjectView( const Object *o )
{
    emit objectViewRequest( ( const Object* ) o );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
