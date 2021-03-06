#ifndef COMBINATOR_IMPL_H
#define COMBINATOR_IMPL_H

#include <phase2/common.h>
#include <phase2/Combinator.h>


/** $decoder */
extern Combinator *
combinator__decode( cstring *buffer );

/** $encoder
    \param buffer  $side-effect */
extern void
combinator__encode( Combinator *c, cstring *buffer );


#endif  /* COMBINATOR_IMPL_H */
