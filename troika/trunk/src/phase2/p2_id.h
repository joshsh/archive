#ifndef P2_ID_H
#define P2_ID_H


typedef struct
{
    char *dataset_id;
    char *local_id;
} p2_id;


p2_id *p2_id__new(char *dataset_id, char *local_id);
void p2_id__delete(p2_id *id);


#endif  // P2_ID_H

