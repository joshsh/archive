#include <phase2/Set.h>

/** a - b */
extern Set *
set__exclusion( Set *a, Set *b );

/** Set-theoretic equivalent of AND. */
extern Set *
set__intersection( Set *a, Set *b );

/** Set-theoretic equivalent of XOR. */
extern Set *
set__symmetric_difference( Set *a, Set *b );

/** Set-theoretic equivalent of OR. */
extern Set *
set__union( Set *a, Set *b );
