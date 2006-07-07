#ifndef INT_H
#define INT_H

#include <common.h>


/** $decoder */
extern int *
int__decode( cstring *buffer );

/** $encoder */
extern void
int__encode( int *p, cstring *buffer );


#endif  /* INT_H */
