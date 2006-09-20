#include <common.h>
#include <stdio.h>
#include "../../../../../settings.h"

int
putchar_wrapper(int c)
{
    return putchar( c );
}
