#ifndef COMBINATOR_H
#define COMBINATOR_H

#include <sk/sk.h>
#include "include/cstring-def.h"


/** $encoder */
void combinator__encode( combinator *c, cstring *buffer );

/** $decoder */
combinator combinator__decode( cstring *buffer );


#endif  /* COMBINATOR_H */
