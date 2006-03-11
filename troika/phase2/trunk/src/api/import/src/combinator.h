#ifndef COMBINATOR_H
#define COMBINATOR_H

#include "../../../sk/sk.h"


/** $encoder */
void combinator__encode( combinator *c, char *buffer );

/** $decoder */
combinator combinator__decode( char *buffer );


#endif  /* COMBINATOR_H */
