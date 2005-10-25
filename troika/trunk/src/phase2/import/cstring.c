#include <string.h>
#include <stdio.h>
#include <stdlib.h>



void cstring__encode(char *p, char *buffer)
{
    //- Reformatting for XML...

    sprintf(buffer, p);
}



char *cstring__decode(char *buffer)
{
    //- Reformatting from XML...

    return strdup(buffer);
}



void cstring__delete(char *p)
{
	free(p);
}
