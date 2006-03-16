#ifndef COMBINATOR_H
#define COMBINATOR_H

#include <sk/sk.h>


/** $decoder */
combinator *combinator__decode( cstring *buffer );

/** $encoder */
void combinator__encode( combinator *c, cstring *buffer );


#endif  /* COMBINATOR_H */
