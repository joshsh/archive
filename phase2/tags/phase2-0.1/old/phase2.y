%{
/*+

  phase2.y

  Bison grammar file for the command line interface.
  The actual parser behavior is pretty minimal; it just matches user input
  against a simple "expression" and "command" syntax, puts it in a P2_LIST
  and sends it to the phase2.c module, which tells it what to print to the
  screen.  Then it waits for more input.

  author: Josh Shinavier
  last edited: 4/18/05
  
Note on Bison: don't forget to include curly braces after each grammar clause
(even if the action is empty), otherwise be prepared for errors of this ilk:
  j.y:54: type clash (`' `number') on default action  
  
*/



#include <stdio.h>
#include <string.h>

#include "util/list.h"
#include "debug.h"



/*
  Interface with the semantics module defined by phase2.c.
*/
extern char *P2_evaluate_expression(char *name, P2_LIST *expr);
extern char *P2_execute_command(char *name, P2_LIST *args);



void yyerror(const char *str)
{
	fprintf(stderr,"Parse error: %s\n",str);
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



%}

%token SEMICOLON
%token OPEN_PAREN CLOSE_PAREN
%token OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET
%token OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET

%union
{
	char *string;
	P2_LIST *list;
}

%token <string> LITERAL
%token <string> IDENTIFIER
%token <string> COMMAND

%type <string> token
%type <list> parameter_list
%type <list> term

%%

input_sequence:
	/* empty */ {
		//open_paren = jstring_from_char((char) OPEN_SUBTERM);
		//close_paren = jstring_from_char((char) CLOSE_SUBTERM);
		//printf("\t>> <<\n\n");
		
	}
	| input_sequence command {
		PRINTF("Found {input_sequence command}.\n");
	}
	| input_sequence expression {
		PRINTF("Found {input_sequence expression}.\n");
	}	
	;

command:
	COMMAND parameter_list SEMICOLON {
		PRINTF("Found {COMMAND parameter_list;}.\n");
		printf("%s\n", P2_execute_command($1, $2));
		//! garbage...	
	};

parameter_list:
	token {
		PRINTF("Found {token}.\n");
		$$ = P2_LIST_singleton((void *) $1);
		//! You also need to add the token to a collection where it can
		//  be garbage-collected afterwards.
	}
	| parameter_list token {
		PRINTF("Found {parameter_list token}.\n");	
		$$ = P2_LIST_concatenate($1, P2_LIST_singleton((void *) $2);
		//! garbage...
	};
	
expression:
	term SEMICOLON {
		PRINTF("Found {term;}.\n");
		printf("%s\n", P2_evaluate_expression(NULL, $1));
		//! garbage...
	}
	| term EQUALS token SEMICOLON {
		PRINTF("Found {term = token;}.\n");
		printf("%s\n", P2_evaluate_expression($3, $1));
		//! garbage...
	};

term:
	token {
		PRINTF("Found {token}.\n");
		$$ = P2_LIST_singleton((void *) $1);
		//!
	}
	| term TOKEN {
		PRINTF("Found {term token}.\n");
		$$ = P2_LIST_concatenate($1, P2_LIST_singleton((void *) $2);
	}
	| OPEN_PAREN term CLOSE_PAREN {
		PRINTF("Found {(term)}.\n");
	!!!!	$$ = P2_LIST_append($1, P2_LIST_singleton((void *) $2);
	};
	
token:
	IDENTIFIER {
		$$ = $1;
		PRINTF("Found {IDENTIFIER}.\n");
	}
	| LITERAL {
		$$ = $1;
		PRINTF("Found {LITERAL}.\n");
	};



/*- end of file --------------------------------------------------------------*/
