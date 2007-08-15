#ifndef DOUBLE_H
#define DOUBLE_H

#include <phase2/common.h>


/** $decoder */
extern double *
double__decode( cstring *buffer );

/** $encoder
    \param buffer  $side-effect */
extern void
double__encode( double *p, cstring *buffer );


#endif  /* DOUBLE_H */
