#include <stdio.h>
#include <stdlib.h>



void char__encode(char *p, char *buffer)
{
    //- Reformatting for XML...

    sprintf(buffer, "%c", *p);
}



char *char__decode(char *buffer)
{
    //- Reformatting from XML, escape characters...
    
    char *p = (char *) malloc(sizeof(char));
    *p = *buffer;
    return p;
}



// Note: eventually these nearly-trivial deallocators can be eliminated... just
// as soon as it's decided that they'll only ever be applied to truly simple types.
// Trivial types (e.g. PlaceHolder, S, K) need to be taken into account as well.
void char__delete(char *p)
{
	free(p);
}
