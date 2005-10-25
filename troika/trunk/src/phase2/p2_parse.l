%{

/*+

  p2_parse.l

  Flex input file for the command line interface.

  last edited: 5/18/05

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

*///////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include "p2_parse.tab.h"
#include "p2_flags.h"



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


";"	{ DEBUG_PRINTF("Found SEMICOLON.\n"); return SEMICOLON; }
"="	{ DEBUG_PRINTF("Found EQUALS.\n"); return EQUALS; }
"("	{ DEBUG_PRINTF("Found OPEN_PAREN.\n"); return OPEN_PAREN; }
")"	{ DEBUG_PRINTF("Found CLOSE_PAREN.\n"); return CLOSE_PAREN; }
"["	{ DEBUG_PRINTF("Found OPEN_SQUARE_BRACKET.\n"); return OPEN_SQUARE_BRACKET; }
"]"	{ DEBUG_PRINTF("Found CLOSE_SQUARE_BRACKET.\n"); return CLOSE_SQUARE_BRACKET; }
"{"	{ DEBUG_PRINTF("Found OPEN_CURLY_BRACKET.\n"); return OPEN_CURLY_BRACKET; }
"}"	{ DEBUG_PRINTF("Found CLOSE_CURLY_BRACKET.\n"); return CLOSE_CURLY_BRACKET; }





([/][!#-'*-/:<>-@\^`|~][!#-'*-:<>-Z\^-z|~]*)|([!#-'*-.0-:<>-Z\^-z|~][!#-'*-:<>-Z\^-z|~]*)	{
        /* [/][not alnum][any]* | [not /][any]* */
	yylval.string = strdup(yytext);

	DEBUG_PRINTF("Found IDENTIFIER.\n");
	return IDENTIFIER;
}



[/][A-Za-z0-9_]+	{
	yylval.string = strdup(yytext+1);
	DEBUG_PRINTF("Found COMMAND.\n");
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
	/*
	s_low--;
	*s_low = '\0';
	yylval.string=strdup(yytext+1);
	*/
	yylval.string=strdup(yytext);
	return LITERAL;
	//TOKEN_MATCH(LITERAL);
}



\n                      /* ignore end of line */;
\r			/* ignore carriage return */;



%%

/*

[ \t]+                   ignore whitespace ;

*/

/*- end of file */
