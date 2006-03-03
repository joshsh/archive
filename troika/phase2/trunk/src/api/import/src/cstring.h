#ifndef CSTRING_H
#define CSTRING_H

#include "include/cstring-def.h"


/** $encoder */
void cstring__encode( cstring *p, char *buffer );

/** $decoder */
cstring *cstring__decode( char *buffer );


#endif  /* CSTRING_H */

