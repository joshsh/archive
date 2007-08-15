%{

/*+

  p2_parse.l

  Flex input file for the command line interface.

  author: Josh Shinavier
  last edited: 4/19/05

*///////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include "p2_parse.tab.h"
#include "p2_debug.h"



//	extern YYSTYPE yylval;
	
/* Note: there is no call for comments in Phase2, as you shouldn't really be
   working off of an "input file" */

char *strdup(char *s)
{
  int len = strlen(s);
  char *s2 = (char *) malloc(len+1);
  return strcpy(s2,s);
}

%}

%%


";"	{ PRINTF("Found SEMICOLON.\n"); return SEMICOLON; }
"="	{ PRINTF("Found EQUALS.\n"); return EQUALS; }
"("	{ PRINTF("Found OPEN_PAREN.\n"); return OPEN_PAREN; }
")"	{ PRINTF("Found CLOSE_PAREN.\n"); return CLOSE_PAREN; }
"["	{ PRINTF("Found OPEN_SQUARE_BRACKET.\n"); return OPEN_SQUARE_BRACKET; }
"]"	{ PRINTF("Found CLOSE_SQUARE_BRACKET.\n"); return CLOSE_SQUARE_BRACKET; }
"{"	{ PRINTF("Found OPEN_CURLY_BRACKET.\n"); return OPEN_CURLY_BRACKET; }
"}"	{ PRINTF("Found CLOSE_CURLY_BRACKET.\n"); return CLOSE_CURLY_BRACKET; }





([/][!#-'*-/:<>-@\^`|~][!#-'*-:<>-Z\^-z|~]*)|([!#-'*-.0-:<>-Z\^-z|~][!#-'*-:<>-Z\^-z|~]*)	{
        /* [/][not alnum][any]* | [not /][any]* */
	yylval.string = strdup(yytext);

	PRINTF("Found IDENTIFIER.\n");
	return IDENTIFIER;
}



[/][A-Za-z0-9_]+	{
	yylval.string = strdup(yytext+1);
	PRINTF("Found COMMAND.\n");
	return COMMAND;
}



["](([ \t!#-~])|([\\]["]))*["]	{
	/* No multi-line literals just yet. */
	char *s_low = yytext+1, *s_high = s_low;
	while (*s_high != '\0') {
	  if (*s_high == '\\') {
	    s_high++;
	    switch (*s_high) {
	      case '"':
	        *s_low = '"';
		break;
	      case '\\':
	        *s_low = '\\';
		break;
	      case 't':
	        *s_low = '\t';
		break;
	      case 'n':
	        *s_low = (char) 10;
		break;
	      //default:  // (e.g. '\0')
	        //...
	    }
	  } else
	    *s_low = *s_high;
	  s_low++;
	  s_high++;
	}
	s_low--;
	*s_low = '\0';
	yylval.string=strdup(yytext+1);
	return LITERAL;
	//TOKEN_MATCH(LITERAL);
}



\n                      /* ignore end of line */;
\r			/* ignore carriage return */;
[ \t]+                  /* ignore whitespace */;



%%

/*- end of file */
