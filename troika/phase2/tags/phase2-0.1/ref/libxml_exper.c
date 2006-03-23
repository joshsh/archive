/*+

  .....c

  Semantic module for the command line interface.

  author: Joshua Shinavier
  last edited: 4/23/05

*///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libxml/encoding.h>
//#include <libxml/xmlwriter.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


/** MAIN *//////////////////////////////////////////////////////////////////////



int main(int argv, char *args[])
{
    int i;
    //gJobPtr cur;

    /* COMPAT: Do not genrate nodes for formatting spaces */
    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    xmlDocPtr doc = xmlParseFile("test.xml");
    /*
    cur = parseGjobFile("test.xml");
	if ( cur )
	  handleGjob(cur);
	else
	  fprintf( stderr, "Error parsing file '%s'\n", argv[i]);
*/

    /* Clean up everything else before quitting. */
    xmlCleanupParser();

    return(0);    
    
}



/*- end of file */
