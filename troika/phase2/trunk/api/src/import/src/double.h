#ifndef DOUBLE_H
#define DOUBLE_H

#include <common.h>


/** $decoder */
extern double *
double__decode( cstring *buffer );

/** $encoder */
extern void
double__encode( double *p, cstring *buffer );


#endif  /* DOUBLE_H */
