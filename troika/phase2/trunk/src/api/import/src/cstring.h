#ifndef CSTRING_H
#define CSTRING_H

#include <p2.h>


/** $decoder */
cstring *cstring__decode( char *buffer );

/** $encoder */
void cstring__encode( cstring *p, char *buffer );


#endif  /* CSTRING_H */
