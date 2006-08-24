%{

/*+

  sparse.l

  Flex input file for the "SParse" programming environment.

  author: Josh Shinavier
  last edited: 12/28/04

*/



#include <stdio.h>
#include <stdlib.h>
#include "sparse.tab.h"

//	extern YYSTYPE yylval;
	
/* Note: there is no call for comments in SParse, as you shouldn't really be working off of a SParse "input file" */

char *str_dup(char *s)
{
  int len = strlen(s);
  char *s2 = (char *) malloc(len+1);
  return strcpy(s2,s);
}

%}


/* An ID may not contain whitespace nor: "#$()./:?@[\] */
id_startchar [-!%-'*+;-<>?^-`{-~A-Za-z0-9]
id_char [-!%-'*+;-<>?^-`{-~A-Za-z0-9]

/*id_startchar [A-Za-z0-9]
id_char [A-Za-z0-9]*/

%%

"\\"	return BACKSLASH;
"."	return PERIOD;
","	return COMMA;
":"	return COLON;
"="	return EQUALS;
"("	return OPEN_PAREN;
")"	return CLOSE_PAREN;
"["	return OPEN_BRACKET;
"]"	return CLOSE_BRACKET;
"$"	return LAST_REFERENCE;
"@"	return SELF_REF_TOKEN;

['][ -&(-~][']	{
	yylval.charval = *(yytext+1);
	return CHAR;
}

['][\]['][']	{
	/* Note: you need to expand this to include more escape characters. */
	yylval.charval = '\'';
	return CHAR;
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
	yylval.string=str_dup(yytext+1);
	return LITERAL;
}

[0]|([1-9][0-9]*)	{
        yylval.intval=atoi(yytext);
	return PARAMETER;
}

[#]([0]|(([-][1-9][0-9]*)|([1-9][0-9]*)))	{
        yylval.intval = atoi(yytext+1);
	return INTEGER;
}

[#][b][0-1]	{
	yylval.intval = (*(yytext+2) - 48);
	return BOOLEAN;
}
		
[#][d]([0]|(([-][1-9][0-9]*)|([1-9][0-9]*)))[.]([0]|([1-9][0-9]*))	{
        yylval.doubleval = atof(yytext+2);
	return DOUBLE;
}

{id_startchar}({id_char}*)	{
	yylval.string = str_dup(yytext);
	return IDENTIFIER;
}

\n                      /* ignore end of line */;
\r			/* */
[ \t]+                  /* ignore whitespace */;
%%

/*- end of file --------------------------------------------------------------*/
