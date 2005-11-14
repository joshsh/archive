#include <stdio.h>
#include <stdlib.h>



void int__encode(int *p, char *buffer)
{
    sprintf(buffer, "%d", *p);
}



int *int__decode(char *buffer)
{
    int *p = (int *) malloc(sizeof(int));
    *p = atoi(buffer);
    return p;
}



void int__delete(int *p)
{
	free(p);
}
