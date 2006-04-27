#ifndef XPM_H
#define XPM_H

#include <defs.h>


/** $decoder */
xpm *
xpm__decode( cstring *buffer );

/** $encoder */
void
xpm__encode( xpm *p, cstring *buffer );

xpm *
cstring__to_xpm( cstring *s );


#endif  /* XPM_H */
