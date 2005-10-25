#ifndef P2_ID_H
#define P2_ID_H


typedef struct
{
    char *dataset_id;
    char *local_id;
} P2_id;

#define P2_ID_NAME       "P2_id"


P2_id *P2_id__new(char *dataset_id, char *local_id);
void P2_id__delete(P2_id *id);


#endif
