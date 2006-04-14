#include <global.h>


#define MAX_INDENT 16
#define CHARS_PER_INDENT_LEVEL 2
#define BUFFER_SIZE 33


// Note: the size of the buffer should be exactly
//     MAX_INDENT * CHARS_PER_INDENT_LEVEL + 1.
static char *indent_buffer = "                                \0";

static int indent_level = 0;


const char *indentPlus()
{
    char *c = indent_buffer
        + ( ( MAX_INDENT - indent_level ) * CHARS_PER_INDENT_LEVEL );

    if ( ++indent_level > MAX_INDENT )
        indent_level = MAX_INDENT;
    return c;
}


const char *indent()
{
    char *c = indent_buffer
        + ( ( MAX_INDENT - indent_level ) * CHARS_PER_INDENT_LEVEL );

    return c;
}


const char *indentMinus()
{
    if ( --indent_level < 0 )
        indent_level = 0;

    char *c = indent_buffer
        + ( ( MAX_INDENT - indent_level ) * CHARS_PER_INDENT_LEVEL );

    return c;
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
