#ifndef INT_H
#define INT_H

#include <p2.h>


/** $decoder */
int *int__decode( cstring *buffer );

/** $encoder */
void int__encode( int *p, cstring *buffer );


#endif  /* INT_H */
