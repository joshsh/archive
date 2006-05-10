#ifndef COMBINATOR_IMPL_H
#define COMBINATOR_IMPL_H

#include <sk/sk.h>


/** $decoder */
Combinator *combinator__decode( cstring *buffer );

/** $encoder */
void combinator__encode( Combinator *c, cstring *buffer );


#endif  /* COMBINATOR_IMPL_H */
