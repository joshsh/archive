#ifndef CHAR_H
#define CHAR_H

#include <common.h>


/** $decoder */
extern char *
char__decode( cstring *buffer );

/** $encoder */
extern void
char__encode( char *c, cstring *buffer );


#endif  /* CHAR_H */
