#ifndef INT_H
#define INT_H

#include <phase2/common.h>


/** $decoder */
extern int *
int__decode( cstring *buffer );

/** $encoder
    \param buffer  $side-effect */
extern void
int__encode( int *p, cstring *buffer );


#endif  /* INT_H */
