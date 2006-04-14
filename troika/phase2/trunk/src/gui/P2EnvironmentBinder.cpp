#include <P2EnvironmentBinder.h>

#include <widgets/basic/P2Text.h>
#include <widgets/bindings/P2CharWidget.h>
#include <widgets/bindings/P2CombinatorWidget.h>
#include <widgets/bindings/P2DoubleWidget.h>
#include <widgets/bindings/P2IntWidget.h>
#include <widgets/bindings/P2NamespaceWidget.h>
#include <widgets/bindings/P2PrimitiveWidget.h>
#include <widgets/bindings/P2StringWidget.h>
#include <widgets/bindings/P2TypeWidget.h>


////////////////////////////////////////////////////////////////////////////////


static P2Widget *charWidget( Object *o, P2EnvironmentBinder *eb )
{
    eb = 0;
    return new P2CharWidget( o );
}


static P2Widget *combinatorWidget( Object *o, P2EnvironmentBinder *eb )
{
    eb = 0;
    return new P2CombinatorWidget( o );
}


static P2Widget *doubleWidget( Object *o, P2EnvironmentBinder *eb )
{
    eb = 0;
    return new P2DoubleWidget( o );
}


static P2Widget *intWidget( Object *o, P2EnvironmentBinder *eb )
{
   eb = 0;
   return new P2IntWidget( o );
}


static P2Widget *namespaceWidget( Object *o, P2EnvironmentBinder *eb )
{
    return new P2NamespaceWidget( o, eb );
}


static P2Widget *primitiveWidget( Object *o, P2EnvironmentBinder *eb )
{
    eb = 0;
    return new P2PrimitiveWidget( o );
}


static P2Widget *stringWidget( Object *o, P2EnvironmentBinder *eb )
{
    eb = 0;
    return new P2StringWidget( o );
}


static P2Widget *typeWidget( Object *o, P2EnvironmentBinder *eb )
{
    eb = 0;
    return new P2TypeWidget( o );
}


////////////////////////////////////////////////////////////////////////////////


static int getType( Environment *e, const char *name )
{
    Object *o = environment__resolve_type( e, name );
    return ( int ) object__value( o );
}


static int getType( Object *o )
{
    return ( int ) object__type( o );
}


P2EnvironmentBinder::P2EnvironmentBinder( const P2Environment &e )
{
    env = &e;
    environment = env->getEnv();

    constructors[ getType( environment, "char" ) ] = charWidget;
    constructors[ getType( environment, "Combinator" ) ] = combinatorWidget;
    constructors[ getType( environment, "double" ) ] = doubleWidget;
    constructors[ getType( environment, "int" ) ] = intWidget;
    constructors[ getType( environment, "Namespace" ) ] = namespaceWidget;
    constructors[ getType( environment, "Primitive" ) ] = primitiveWidget;
    constructors[ getType( environment, "cstring" ) ] = stringWidget;
    constructors[ getType( environment, "Type" ) ] = typeWidget;
}


const P2Environment *P2EnvironmentBinder::getEnv()
{
    return env;
}


static QString
getText( Object *o )
{
    char buffer[100];
    object__type( o )->encode( object__value( o ), buffer );
    return QString( buffer );
}


P2Widget *P2EnvironmentBinder::objectWidget( Object *o )
{
    objectWidgetConstructor c = constructors[ getType( o ) ];
    if ( !c )
        return new P2Text( getText( o ), new QColor( 0x00, 0x00, 0x00, 0xFF ) );

    else
        return c( o, this );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
