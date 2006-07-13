#ifndef COMBINATOR_IMPL_H
#define COMBINATOR_IMPL_H

#include <common.h>
#include <Combinator.h>


/** $decoder */
extern Combinator *
combinator__decode( cstring *buffer );

/** $encoder */
extern void
combinator__encode( Combinator *c, cstring *buffer );


#endif  /* COMBINATOR_IMPL_H */
