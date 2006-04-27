#ifndef P2TermWidgetNew_h
#define P2TermWidgetNew_h

////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

extern "C"
{
#include <Object.h>
}

#include <P2BoxArray.h>
#include <P2Binder.h>

////////////////////////////////////////////////////////////////////////////////

class P2TermWidgetNew : public P2BoxArray
{
public:

    P2TermWidgetNew( Object *o, P2Binder *eb );

    void appendObjectWidget( Object *o, P2Binder *eb );
    void appendSyntaxWidget( char *s );

private:

    void encode( Object *o, P2Binder *eb );

};

////////////////////////////////////////////////////////////////////////////////

#endif  // P2TermWidgetNew_h

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
