//#include "../XMLDOM.h"
#include <stdio.h>
#include <stdlib.h>


//DOM_Element *double__encode(void *p)
void double__encode(double *p, char *buffer)
{
    sprintf(buffer, "%g", *p);
    //DOM_Element *el = new_DOM_Element(document_, NULL, "xs:double", encode_buffer_);
}



//void *(*double__decode)(DOM_Element *root)
double *double__decode(char *buffer)
{
    double *p = (double *) malloc(sizeof(double));
    //*p = atof(DOM_text(root));
    *p = atof(buffer);
    return p;
}



void double__delete(double *p)
{
	free(p);
}
