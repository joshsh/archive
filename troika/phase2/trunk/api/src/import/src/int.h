#ifndef INT_H
#define INT_H

#include <common.h>


/** $decoder */
extern int *
int__decode( cstring *buffer );

/** $encoder */
extern void
int__encode( int *p, cstring *buffer );


extern int
int__inverse( int a );

extern int
int_int__add( int a, int b );

extern int
int_int__subtract( int a, int b );

extern int
int_int__multiply( int a, int b );

extern int
int_int__divide( int a, int b );

extern int
int_int__modulus( int a, int b );


#endif  /* INT_H */
