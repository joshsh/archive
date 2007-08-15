/**

  gcc -c ../include/util/array.c
  gcc -c ../include/util/hash_table.c
  gcc -c SK.c
  gcc -o SK_test array.o hash_table.o SK.o SK_test.c

*/

#include <string.h>
#include <stdio.h>
#include "SK.h"
#include "../include/util/hash_table.h"

HASH_TABLE *names;
HASH_TABLE *ts_print;

#define HASHIT(term, name)	temp = strdup(name); \
				hash_table__add(names, (void *) term, temp); \
				array__push(names_array, (void *) temp);



void *integer_type = (void *) 1;

void integer_print(void *p)
{
  printf("%d", *((int *) p));
}

int *add(int *a, int *b)
{
  int *c = (int *) malloc(sizeof(int));
  *c = *a + *b;
  return c;
}



void print_SK_term(struct SK_term *term)
{
  int i;
  ARRAY *array;
  char *name = (char *) hash_table__lookup(names, (void *) term->value.array_value);
  void (*print)(void *);
  switch (term->type)
  {
    case atom_type:
      if (name == NULL) {
        print = (void (*)(void *)) hash_table__lookup(ts_print, (void *) term->value.atom_value->type);
        if (print == NULL)
	  printf("[atom]");
	else print((void *) term->value.atom_value->value);
      } else
        printf(name);
      break;
    case primitive_type:
      if (name == NULL)
        printf("[prim]");
      else
        printf(name);
      break;
    case S_type:
      printf("S");
      break;
    case K_type:
      printf("K");
      break;
    case expired_type:
      printf("[expired!]");
      break;
    case compound_type:
      printf("(");
      array = term->value.array_value;
      print_SK_term((struct SK_term *) array__peek(array));
      for (i=1; i<array->size; i++) {
        printf(" ");
	print_SK_term((struct SK_term *) array__get(array, i));
      }
      printf(")");
      break;
  }
}



void output_SK_term(struct SK_term *term)
{
    printf("\t>> ");
    print_SK_term(term);
    printf(" <<\n");
}



void apply_with_output(struct SK_term *left, struct SK_term *right)
{
  enum SK_evaluation_outcome outcome;

  print_SK_term(left);
  printf(" ");
  print_SK_term(right);
  printf("\n");

  outcome = SK_apply(left, right);
  if (outcome)
    printf("\t>> Error: %s <<\n", outcome_codes[outcome]);
  else
    output_SK_term(left);
}



struct SK_term *int_term(int i)
{
  int *i_p = (int *) malloc(sizeof(int));
  *i_p = i;
  return SK_atom_term((ATOM_REFERENCE) i_p, integer_type);
}



int main()
{
  char *temp;
  struct SK_term *term, *term2, *add_term;
  struct SK_primitive *add_prim;

  ARRAY *names_array = array__new(100, 2);
  names = hash_table__new(100, 2, 2, ADDRESS_DEFAULTS);

  ts_print = hash_table__new(10, 2, 2, ADDRESS_DEFAULTS);
  hash_table__add(ts_print, (void *) integer_type, (void *) integer_print);



  add_prim = (struct SK_primitive *) malloc(sizeof(struct SK_primitive));
  add_prim->value = (PRIM_REFERENCE) add;
  add_prim->return_type = (TYPE_REFERENCE) integer_type;
  add_prim->parameters = 2;
  add_prim->parameter_types = (TYPE_REFERENCE *) malloc(sizeof(TYPE_REFERENCE) * add_prim->parameters);
  add_prim->parameter_types[0] = (TYPE_REFERENCE) integer_type;
  add_prim->parameter_types[1] = (TYPE_REFERENCE) integer_type;
  add_term = SK_primitive_term(add_prim);
  HASHIT(add_prim, "+");



  /*
  term = SK_S_term();
  apply_with_output(term, SK_S_term());
  apply_with_output(term, SK_K_term());
  apply_with_output(term, SK_K_term());
  printf("\n\n");
  */

  /*
    S + (K -7) 31
    + 31 (K -7 31)
    + 31 -7
    24
    24 25
    --> Error: ATOM_APPLIED_AS_FUNCTION
  */
  term = SK_K_term();
  apply_with_output(term, int_term(-7));
  printf("\n");
  term2 = SK_S_term();
  apply_with_output(term2, add_term);
  apply_with_output(term2, term);
  apply_with_output(term2, int_term(31));

  // This one should result in an error.
  apply_with_output(term2, int_term(25));

  SK_destroy_term(term2);



  array__forall(names_array, free);
  array__delete(names_array);
  hash_table__delete(ts_print);
  hash_table__delete(names);
}
