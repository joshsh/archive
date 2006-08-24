%{
/*+

  sparse.y

  Bison grammar file for the "SParse" programming environment.

  author: Josh Shinavier
  last edited: 12/28/04

*/



#include <stdio.h>
#include <string.h>

/* Expression string macros as defined in snpl.h */
#define REFERENCE_HOLE 0
#define OPEN_SUBTERM 1
#define CLOSE_SUBTERM 2
#define REFERENCE 3
#define SELF_REFERENCE 4
#define FIRST_PARAMETER 5

#include "debug.h"
#include "jstring.h"

/*
  Interface with the SNPL portion of the interpreter (via sparse.cpp).
*/
extern void *SParse_dictionary_lookup(char *);
extern void SParse_dictionary_add(void *, char *);
extern void *SParse_cstring_constructor(char *);
extern void *SParse_char_constructor(char);
extern void *SParse_bool_constructor(int);
extern void *SParse_int_constructor(int);
extern void *SParse_double_constructor(double);
extern void SParse_print(void *);
extern void *SParse_interpret_expression(char *, int, int);

/*
Note on Bison: don't forget to include curly braces after each grammar clause (even if the action is empty), otherwise be prepared for errors like this:
  j.y:54: type clash (`' `number') on default action
*/

void yyerror(const char *str)
{
	fprintf(stderr,"sparse.y error: %s\n",str);
}

int yywrap()
{
	return 1;
}

/*
main()
{
	yyparse();
}
*/

// Note: these are never garbage-collected.
struct jstring *open_paren, *close_paren;

struct jstring *parenthesize(struct jstring *j)
{
	return jcat(jcat(open_paren, j), close_paren);
}

/*
struct jstring *concatenate(struct jstring *j1, struct jstring *j2)
{
  struct jstring *j = jcat(j1, j2);
  return j;
}*/

void *last_item=NULL;
//int topvar;

struct expression {
  struct jstring *jval;
  int topvar;
};

void expression_destructor(struct expression *e)
{
  jstring_destructor(e->jval);
  free(e);
}

void *evaluate_term(struct expression *e)
{
  void *p;
  struct jstring *j = parenthesize(e->jval);

#ifdef DEBUG
jstring_view(j);
view_bytes(j->s, j->length);
#endif

  p = SParse_interpret_expression(
    j->s, j->length, e->topvar+1);
//printf("p = %d\n", (int) p);
  jstring_destructor(j);
  return p;
}

// Print the result of the reduction to stdout.
void output(void *p)
{
    printf("\n\t>> ");
    SParse_print(p);
    printf(" <<\n\n");
}



%}

%token PERIOD COMMA COLON EQUALS BACKSLASH
%token OPEN_PAREN CLOSE_PAREN
%token OPEN_BRACKET CLOSE_BRACKET
%token LAST_REFERENCE SELF_REF_TOKEN
%token EXPORT

%union
{
	int intval;
	char charval;
	double doubleval;
	char *string;
	struct expression *eval;
}

%token <intval> PARAMETER
%token <intval> BOOLEAN
%token <intval> INTEGER
%token <doubleval> DOUBLE
%token <charval> CHAR
%token <string> LITERAL
%token <string> IDENTIFIER

%type <eval> item
%type <eval> scalar
%type <eval> term

%%

commands: /* empty */ {
		open_paren = jstring_from_char((char) OPEN_SUBTERM);
		close_paren = jstring_from_char((char) CLOSE_SUBTERM);
		printf("\t>> <<\n\n");
	}
	| commands command COMMA {}
	| commands command PERIOD {
		output(last_item);
	}
	;

command:
	term {
		last_item = evaluate_term($1);
		expression_destructor($1);
	}
	| term EQUALS IDENTIFIER {
		last_item = evaluate_term($1);
		expression_destructor($1);

		PRINTF("Found IDENTIFIER as dictionary definition\n");
		if (last_item != NULL) {
		  PRINTF("adding %s to dictionary", $3);
		  SParse_dictionary_add(last_item, $3);
		}
		free($3);
	}
	;
	
term:
	item {
		$$ = $1;
	}
	| term item {
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jcat($1->jval, $2->jval);
		if ($1->topvar > $2->topvar)
		  $$->topvar = $1->topvar;
		else
		  $$->topvar = $2->topvar;
		expression_destructor($1);
		expression_destructor($2);
//printf("foo\n");
//view_bytes($$->jval->s, $$->jval->length);//exit(0);
	}
	;

item:
	PARAMETER {
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_char((char) ($1+FIRST_PARAMETER));
		//if ($1 > topvar)
		//  topvar = $1;
		$$->topvar = $1;
		PRINTF("Found PARAMETER: %d\n", $1);
	}
	| LITERAL {
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(
		  SParse_cstring_constructor($1));
		$$->topvar = -1;
		PRINTF("Found LITERAL\n");
	} /*!!! only in argument position !!!*/
	| scalar {
		$$ = $1;
	}  /*!!! only in argument position !!!*/
	| IDENTIFIER {
		void *p = SParse_dictionary_lookup($1);
		/* ! Redundant string: it's in both item and dictionary */
		if (p == NULL)
		{
		  p = SParse_cstring_constructor($1);
		  SParse_dictionary_add(p, $1);
		}
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(p);
		$$->topvar = -1;
		PRINTF("Found IDENTIFIER as item: %s, %d\n", $1, (int) p); //exit(0);
	}
	| LAST_REFERENCE {
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(last_item);
		$$->topvar = -1;
		PRINTF("Found LAST_REFERENCE\n");
	}
	| SELF_REF_TOKEN {
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_char((char) SELF_REFERENCE);
		$$->topvar = -1;
		PRINTF("Found SELF_REFERENCE\n");
	}
	| OPEN_PAREN term CLOSE_PAREN {
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = parenthesize($2->jval);
		$$->topvar = $2->topvar;
		expression_destructor($2);
		PRINTF("Found sub-term\n");
	}
	| OPEN_BRACKET term CLOSE_BRACKET {
		void *p = evaluate_term($2);
		expression_destructor($2);
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(p);
		$$->topvar = -1;
		PRINTF("Found bracketed sub-term\n");
	}
	;

scalar:
	BOOLEAN {
		void *p = SParse_bool_constructor($1);
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(p);
		$$->topvar = -1;
		PRINTF("Found BOOLEAN\n");
	}
	| INTEGER {
		void *p = SParse_int_constructor($1);
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(p);
		$$->topvar = -1;
		PRINTF("Found INTEGER\n");
	}
	| DOUBLE {
		void *p = SParse_double_constructor($1);
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(p);
		$$->topvar = -1;
		PRINTF("Found DOUBLE\n");
	}
	| CHAR {
		void *p = SParse_char_constructor($1);
		$$ = (struct expression *)
		  malloc(sizeof(struct expression));
		$$->jval = jstring_from_pointer(p);
		$$->topvar = -1;
		PRINTF("Found CHAR\n");
	}
	;

/*- end of file --------------------------------------------------------------*/
