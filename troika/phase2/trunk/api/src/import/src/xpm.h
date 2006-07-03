#ifndef XPM_H
#define XPM_H

#include <common.h>


/** $decoder */
extern xpm *
xpm__decode( cstring *buffer );

/** $encoder */
extern void
xpm__encode( xpm *p, cstring *buffer );

extern xpm *
cstring__to_xpm( cstring *s );


#endif  /* XPM_H */
