#include "p2_id.h"



P2_id *P2_id__new(char *dataset_id, char *local_id)
{
    P2_id *id = (P2_id *) malloc(sizeof(P2_id));
    id->dataset_id = dataset_id;
    id->local_id = local_id;
}



void P2_id__delete(P2_id *id)
{
    if (id->local_id)
        free(id->local_id);
    if (id->dataset_id)
        free(id->dataset_id);
    free(id);
}
