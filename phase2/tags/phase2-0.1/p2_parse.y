%{
/*+

  p2_parse.y

  Bison grammar file for the command line interface.
  The actual parser behavior is pretty minimal; it just matches user input
  against a simple "expression" and "command" syntax, encodes it as XML and
  sends it to the p2_itf.c module, which tells it what to print to the
  console.

  author: Josh Shinavier
  last edited: 4/20/05

Note on Bison: don't forget to include curly braces after each grammar clause
(even if the action is empty), otherwise be prepared for errors of this ilk:
  j.y:54: type clash (`' `number') on default action

*///////////////////////////////////////////////////////////////////////////////



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p2_debug.h"



/*
  Semantic functions from p2_itf.c:
*/
extern char *P2_evaluate_expression(char *name, char *expr);
extern char *P2_execute_command(char *name, char *args);



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



#define TOK_	"<xs:string>"
#define _TOK	"</xs:string>\n"

//! Not safe as-is.  E.g. a string literal with a '"' would be a
//  problem.  Eventually token_tag will need to transform XML-unfriendly strings.
char *token_tag(char *contents)
{
  const int len = 1 + strlen(TOK_) + strlen(_TOK);
  char *s = (char *) malloc(sizeof(char)*(strlen(contents)+len));
  sprintf(s, "%s%s%s", TOK_, contents, _TOK);
  return s;
}



#define SEQ_	"<P2:Sequence sequenceType=\"immediate\">\n"
#define _SEQ	"</P2:Sequence>\n"

char *sequence_tag(char *contents)
{
  const int len = 1 + strlen(SEQ_) + strlen(_SEQ);
  char *s = (char *) malloc(sizeof(char)*(strlen(contents)+len));
  sprintf(s, "%s%s%s", SEQ_, contents, _SEQ);
  return s;
}



#define SEQD_	"<P2:Sequence sequenceType=\"delayed\">\n"
#define _SEQD	"</P2:Sequence>\n"

char *sequence_tag_delayed(char *contents)
{
  const int len = 1 + strlen(SEQD_) + strlen(_SEQD);
  char *s = (char *) malloc(sizeof(char)*(strlen(contents)+len));
  sprintf(s, "%s%s%s", SEQD_, contents, _SEQD);
  return s;
}


%}

%token SEMICOLON EQUALS
%token OPEN_PAREN CLOSE_PAREN
%token OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET
%token OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET

%union
{
	char *string;
}

%token <string> LITERAL
%token <string> IDENTIFIER
%token <string> COMMAND

%type <string> token term subterm expression parameter_list command

%%


parser_input:
	/* empty */ {
		// Global vars could be initialized here.

		//printf("\t>> <<\n\n");
		printf("\n");

	}
	| parser_input command {
		PRINTF("Found {parser_input command}.\n");
	}
	| parser_input expression {
		PRINTF("Found {parser_input expression}.\n");
	};



command:
	COMMAND SEMICOLON {
		char *s = sequence_tag("");
		PRINTF("Found {COMMAND parameter_list;}.\n");
		printf("\n%s\n", P2_execute_command($1, s));
		free(s);
	}
	| COMMAND parameter_list SEMICOLON {
		char *s = sequence_tag($2);
		free($2);
		PRINTF("Found {COMMAND parameter_list;}.\n");
		printf("\n%s\n", P2_execute_command($1, s));
		free(s);
	};



parameter_list:
	token {
		PRINTF("Found {token}.\n");
	        $$ = token_tag($1);
	        free($1);
	}
	| parameter_list token {
		char *t = token_tag($2);
		free($2);
		PRINTF("Found {parameter_list token}.\n");
		$$ = (char *) malloc(sizeof(char)*(1+strlen($1)+strlen(t)));
		sprintf($$, "%s%s", $1, t);
		free($1); free(t);
	};



expression:
	term SEMICOLON {
		char *s = sequence_tag($1), *s2;
		free($1);
		s2 = P2_evaluate_expression(NULL, s);
		free(s);
		PRINTF("Found {term;}.\n");
		printf("\n%s\n", s2);
		free(s2);
	}
	| term EQUALS token SEMICOLON {
		char *s = sequence_tag($1), *s2;
		free($1);
		s2 = P2_evaluate_expression($3, s);
		PRINTF("Found {term;}.\n");
		printf("\n%s\n", s2);
		free($3); free(s); free(s2);
	};



term:
	subterm {
		$$ = $1;
	}
	| term subterm {
		PRINTF("Found {term subterm}.\n");
		$$ = (char *) malloc(sizeof(char)*(1+strlen($1)+strlen($2)));
		sprintf($$, "%s%s", $1, $2);
		free($1); free($2);
	};



subterm:
	token {
	        $$ = token_tag($1);
	        free($1);
		PRINTF("Found {token}.\n");
	}
	| OPEN_PAREN term CLOSE_PAREN {
		PRINTF("Found {(term)}.\n");
		$$ = sequence_tag($2);
		free($2);
	}
	| OPEN_SQUARE_BRACKET term CLOSE_SQUARE_BRACKET {
		PRINTF("Found {(term)}.\n");
		$$ = sequence_tag_delayed($2);
		free($2);
	};



token:
	IDENTIFIER {
		$$ = $1;
	}
	| LITERAL {
		$$ = $1;
	};



/*- end of file */
