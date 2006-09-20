#include <common.h>
#include <stdlib.h>
#include "../../../../../settings.h"

char *
getenv_wrapper(const char* name)
{
    char *r = getenv( name );
    return r ? STRDUP( r ) : 0;
}
