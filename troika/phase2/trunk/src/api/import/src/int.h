#ifndef INT_H
#define INT_H

#include "include/cstring-def.h"


/** $decoder */
int int__decode( cstring *buffer );

/** $encoder */
void int__encode( int *p, cstring *buffer );


#endif  /* INT_H */
