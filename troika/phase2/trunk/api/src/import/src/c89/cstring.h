#ifndef CSTRING_H
#define CSTRING_H

#include <common.h>


/** $decoder */
extern cstring *
cstring__decode( char *buffer );

/** $encoder */
extern void
cstring__encode( cstring *p, char *buffer );


#endif  /* CSTRING_H */
