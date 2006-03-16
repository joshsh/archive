#ifndef DOUBLE_H
#define DOUBLE_H

#include <p2.h>


/** $decoder */
double *double__decode( cstring *buffer );

/** $encoder */
void double__encode( double *p, cstring *buffer );


#endif  /* DOUBLE_H */
