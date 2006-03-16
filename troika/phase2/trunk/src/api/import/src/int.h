#ifndef INT_H
#define INT_H

#include "include/cstring-def.h"
#include <p2.h>


/** $decoder */
int *int__decode( cstring *buffer );

/** $encoder */
void int__encode( int *p, cstring *buffer );


#endif  /* INT_H */
