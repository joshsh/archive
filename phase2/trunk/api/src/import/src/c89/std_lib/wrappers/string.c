#include <phase2/common.h>
#include <string.h>
#include "../../../../../settings.h"


char *
strcat_wrapper(char* s, const char* ct)
{
    char *r = ( char* ) malloc( strlen( s ) + strlen( ct ) + 1 );

    if ( r )
    {
        strcpy( r, s );
        strcat( r, ct );
    }

    return r;
}


/* Avoids collision with a strcmp macro. */
int
strcmp_wrapper(const char* cs, const char* ct)
{
    return strcmp( cs, ct );
}


int
strlen_wrapper(const char* cs)
{
    return ( int ) strlen( cs );
}
