#ifndef CSTRING_H
#define CSTRING_H

#include <phase2/common.h>


/** $decoder */
extern cstring *
cstring__decode( char *buffer );

/** $encoder
    \param buffer  $side-effect */
extern void
cstring__encode( cstring *p, char *buffer );


#endif  /* CSTRING_H */
