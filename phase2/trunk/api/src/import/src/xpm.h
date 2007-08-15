#ifndef XPM_H
#define XPM_H

#include <phase2/common.h>


/** $decoder */
extern xpm *
xpm__decode( cstring *buffer );

/** $encoder
    \param buffer  $side-effect */
extern void
xpm__encode( xpm *p, cstring *buffer );

extern xpm *
cstring__to_xpm( cstring *s );


#endif  /* XPM_H */
