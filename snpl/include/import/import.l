%{
#include <stdlib.h>
#include <stdio.h>
#include "import.tab.h"

#ifndef DEBUG
  //#define DEBUG
#endif

void echo(char *s)
{
  #ifdef DEBUG
    printf(s);
  #endif
}

char *str_dup(char *s)
{
  int len = strlen(s);
  char *s2 = (char *) malloc(len+1);
  return strcpy(s2,s);
}

void exhaust_comment(int (*input_ptr)(void))
{
  char ch;
  while (((ch = input_ptr()) != '\n')
         &&(ch != '\r')
	 &&(ch != EOF)) {}
}

%}

%%

"#"	{
	echo("[comment]\n");
	exhaust_comment(&input);
}

[!$-,/-9;-~][\t !$-9;-~]*[:]	{
	echo("COLON_HEADER\n");
	return COLON_HEADER;
}

[\"][\t !#-~]*[\"]	{
	char *s = str_dup(yytext+1);
	s[strlen(s)-1] = '\0';
	yylval.string = s;
	echo("LITERAL\n");
	return LITERAL;
}

"."	{
	echo("PERIOD\n");
	return PERIOD;
}

[A-Za-z]+	{
	echo("WORD\n");
	return WORD;
}

\n                      /* ignore end of line */;
\r			/* */
[ \t]+                  /* ignore whitespace */;
%%
