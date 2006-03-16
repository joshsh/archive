#ifndef CHAR_H
#define CHAR_H

#include "include/cstring-def.h"
#include <p2.h>


/** $decoder */
char *char__decode( cstring *buffer );

/** $encoder */
void char__encode( char *c, cstring *buffer );


#endif  /* CHAR_H */
