#ifndef CHAR_H
#define CHAR_H

#include <phase2/common.h>


/** $decoder */
extern char *
char__decode( cstring *buffer );

/** $encoder
    \param buffer  $side-effect */
extern void
char__encode( char *c, cstring *buffer );


#endif  /* CHAR_H */
