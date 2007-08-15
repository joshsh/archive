#ifndef CHAR_H
#define CHAR_H

#include <defs.h>


/** $decoder */
char *char__decode( cstring *buffer );

/** $encoder */
void char__encode( char *c, cstring *buffer );


#endif  /* CHAR_H */
