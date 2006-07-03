#ifndef DOUBLE_H
#define DOUBLE_H

#include <common.h>


/** $decoder */
extern double *
double__decode( cstring *buffer );

/** $encoder */
extern void
double__encode( double *p, cstring *buffer );


extern double
double__inverse( double a );

extern double
double_double__add( double a, double b );

extern double
double_double__subtract( double a, double b );

extern double
double_double__multiply( double a, double b );

extern double
double_double__divide( double a, double b );


#endif  /* DOUBLE_H */
