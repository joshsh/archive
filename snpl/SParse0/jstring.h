/*+

  jstring.h

  A crude string builder for sparse.y.
  
  author: Josh Shinavier
  last edited: 12/28/04

*/

#ifndef JSTRING_H
#define JSTRING_H



#include <stdlib.h>

/*
  Prototypes for constructs defined in this library.
*/
struct jstring;
void view_bytes(void *, int);
struct jstring *jstring_from_pointer(void *);
struct jstring *jstring_from_char(char);
struct jstring *jcat(struct jstring *, struct jstring *);
char *jstring_tostring(struct jstring *);
void jstring_view(struct jstring *);
void jstring_destructor(struct jstring *);


// Temporary
void view_bytes(void *start_address, int n)
{
  int i;
  char *s = (char *) start_address;
  printf("%d bytes beginning at %d:", n, (int) start_address);
  for (i=0; i<n; i++) {
    printf(" %d", (int) *s);
    s++;
  }
  printf("\n");
}



struct jstring
{
  char *s;
  int length;
};

void jstring_destructor(struct jstring *j)
{
  free(j->s);
  free(j);
}

struct jstring *jstring_from_pointer(void *p)
{
  int i;
  struct jstring *j = (struct jstring *) malloc(sizeof(struct jstring));
  char *s = (char *) malloc(5), *s2 = (char *) &p;
  j->s = s;
  *s = (char) REFERENCE;
  s++;
  for (i=0; i<4; i++)
  {
    *s = *s2;
    s++;
    s2++;
  }
  j->length = 5;
//printf("from pointer: %d\n", (int) p);
//printf("New jstring: length = %d\n", j->length);
//view_bytes((void *) j->s, j->length);
  return j;
}



struct jstring *jstring_from_char(char ch)
{
  struct jstring *j = (struct jstring *) malloc(sizeof(struct jstring));
  j->s = (char *) malloc(1);
  *(j->s) = ch;
  j->length = 1;
//printf("New jstring: length = %d\n", j->length);
  return j;
}



struct jstring *jcat(struct jstring *j1, struct jstring *j2)
{
  int i, len1 = j1->length, len2 = j2->length;
  char *s = (char *) malloc(len1+len2);
  struct jstring *j = (struct jstring *) malloc(sizeof(struct jstring));
  j->s = s;
  j->length = len1+len2;
  for (i=0; i<len1; i++)
    j->s[i] = j1->s[i];
  for (i=0; i<len2; i++)
    j->s[len1+i] = j2->s[i];
//printf("New jstring: length = %d\n", j->length);
  return j;
}



char *jstring_tostring(struct jstring *j)
{
  //printf("s0 = \n"); exit(0);
  char *s = (char *) malloc(j->length+1), *s2 = s;
  int i;
  for (i=0; i<j->length; i++) {
    *s2 = j->s[i];
    s2++;
  }
  *s2 = '\0';
  //printf("s = %s\n", s); exit(0);
  return s;
}



void jstring_view(struct jstring *j)
{
	printf("jstring of length %d: ", j->length);
	view_bytes(j->s, j->length);
}



#endif
