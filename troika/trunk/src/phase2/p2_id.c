#include "p2_id.h"

#include <stdlib.h>  // malloc


p2_id *p2_id__new(char *dataset_id, char *local_id)
{
    p2_id *id = (p2_id *) malloc(sizeof(p2_id));
    id->dataset_id = dataset_id;
    id->local_id = local_id;
}



void p2_id__delete(p2_id *id)
{
    if (id->local_id)
        free(id->local_id);
    if (id->dataset_id)
        free(id->dataset_id);
    free(id);
}


