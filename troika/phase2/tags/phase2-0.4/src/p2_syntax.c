/*+

  p2_syntax.c

  Token analyzer for the Phase2 programming environment.

  last edited: 5/31/05

  Note: id and string literal parsing has a known quirk: anything containing
  a '&' character does not appear in output.  Investigate when you get a chance.

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

#include "p2_syntax.h"

//!
#include "import/int.c"
#include "import/char.c"
#include "import/double.c"
#include "import/cstring.c"

#include <string.h>



// Use (only) the standard POSIX.2 interface for regular expression matching.
// Note: stdlib.h defines _POSIX_C_SOURCE for you.
#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE
#endif

#include <regex.h>

// regex_t objects for each token type except for error_token
regex_t id_regex, int_regex, char_regex, float_regex, literal_regex;

// Command names are constrained as well.
regex_t command_regex;

// Matches a single colon.
regex_t colon_regex;



/**
  Compile regular expressions for token types.
  These will require some additional refinement, e.g.
     * escape characters in chars and string literals
     * exponential notation in ints and floats.
*/
P2_error P2_syntax_init()
{
    P2_error err;

    // What about garbage collection?
    if ( 0

         || regcomp (&int_regex, "^-\\?[0-9]\\+$", 0)

         // ASCII character in single quotes.
         || regcomp (&char_regex, "^'[ \t\r\n!-&(-~]'$", 0)

         // A numerical token is interpreted as a double if it
         // contains a decimal point (^ Forth)
         || regcomp (&float_regex, "^-\\?[0-9]\\+.[0-9]\\+$", 0)

         // String literal in (double) quotes.  Note: "" is not allowed.
         || regcomp (&literal_regex, "^\"[ \n\r\t!#-~]\\+\"$", 0)

         // No spaces, no colons (another practical constraint is that an id
         // can't contain a semicolon, but this is determined by the parser).
         || regcomp (&id_regex, "^[!-9;-~]\\+:[!-9;-~]\\+$\\|^[!-9;-~]\\+$", 0)

         // Digits, letters and underscores only in command names.
         || regcomp (&command_regex, "^[_0-9A-Za-z]\\+$", 0)

         || regcomp (&colon_regex, ":", 0)

    )
        err = BAD_REGEX;
    else
        err = P2_SUCCESS;

    // Register standard data types.
    P2_int_type = P2_register_type(P2_INT_NAME,
        (ENCODE_FORMAT) int__encode,
        (DECODE_FORMAT) int__decode,
        (DESTROY_FORMAT) int__delete,
        (CLONE_FORMAT) NULL);
    P2_char_type = P2_register_type(P2_CHAR_NAME,
        (ENCODE_FORMAT) char__encode,
        (DECODE_FORMAT) char__decode,
        (DESTROY_FORMAT) char__delete,
        (CLONE_FORMAT) NULL);
    P2_double_type = P2_register_type(P2_DOUBLE_NAME,
        (ENCODE_FORMAT) double__encode,
        (DECODE_FORMAT) double__decode,
        (DESTROY_FORMAT) double__delete,
        (CLONE_FORMAT) NULL);
    P2_cstring_type = P2_register_type(P2_CSTRING_NAME,
        (ENCODE_FORMAT) cstring__encode,
        (DECODE_FORMAT) cstring__decode,
        (DESTROY_FORMAT) cstring__delete,
        (CLONE_FORMAT) NULL);

    return err;
}



P2_error P2_syntax_end()
{
    free(&id_regex);
    free(&int_regex);
    free(&char_regex);
    free(&float_regex);
    free(&literal_regex);
    free(&command_regex);
    free(&colon_regex);

    return P2_SUCCESS;
}



/** TOKEN PARSING */////////////////////////////////////////////////////////////



// local only.
P2_atom parse_id(char *s)
{
    int i;
    char ch;
    regmatch_t rem [1];
    struct P2_identifier *id = (struct P2_identifier *) malloc(sizeof(struct P2_identifier));

    // Look for a colon.
    if (!regexec(&colon_regex, s, 1, rem, 0))
    {
        i = (int) rem[0].rm_so;
        /*if ((!i)||(i == ((int) strlen(s)-1)))
        {
            t.type = error_token;
            t.value.error_value = INVALID_IDENTIFIER;
        }
        else
        {*/
            ch = s[i];
            s[i] = '\0';
            id->dataset_id = strdup(s);
            id->local_id = strdup(s+i+1);
            s[i] = ch;
        //}
    }
    else
    {
        id->dataset_id = NULL;
        id->local_id = strdup(s);
    }

    return (P2_atom) id;
}



SEQUENCE *P2_parse_token(char *s)
{
    int i;
    char ch;
    SEQUENCE *S;// = (SEQUENCE *) malloc(sizeof(SEQUENCE));
    P2_type type;
    P2_atom atom;

    // If a token does not match any of the special syntaxes for integers,
    // doubles, literals, etc., try to interpret it as an identifier
    // (dictionary reference or special symbol).
    // Note: identifiers may not contain colons except as the delimiter between
    // the "parent" and local parts of a dictionary reference.
    if (!regexec(&int_regex, s, 0, NULL, 0))
    {
        type = P2_int_type;
	atom = P2_decode(type, s);
    }
    else if (!regexec(&char_regex, s, 0, NULL, 0))
    {
        type = P2_char_type;
	ch = *(s+2);
	*(s+2) = '\0';
        atom = P2_decode(type, s+1);
    }
    else if (!regexec(&float_regex, s, 0, NULL, 0))
    {
        type = P2_double_type;
	atom = P2_decode(type, s);
    }
    else if (!regexec(&literal_regex, s, 0, NULL, 0))
    {
//printf("found LITERAL\n");
        type = P2_cstring_type;
        i = strlen(s)-1;
        ch = s[i];
        s[i] = '\0';
        atom = P2_decode(type, s+1);
        s[i] = ch;
    }
    else if (!regexec(&id_regex, s, 0, NULL, 0))
    {
//printf("found ID\n");
        type = P2_id_type;
        atom = parse_id(s);
    }
    else {
        type = P2_error_type;
        atom = (P2_atom) INVALID_TOKEN;
    }

    //S->type = (void *) type;
    //S->value = (void *) atom;
    S = sequence__new((void *) type, (void *) atom);
    
    return S;
}



SEQUENCE *P2_parse_identifier(char *s)
{
    SEQUENCE *S;

    if (!regexec(&id_regex, s, 0, NULL, 0))
    {
        S = sequence__new((void *) P2_id_type, (void *) parse_id(s));
    }
    else
    {
        S = sequence__new((void *) P2_error_type, (void *) INVALID_IDENTIFIER);
    }

    return S;
}



int P2_valid_command_name(char *s)
{
    return !(regexec(&command_regex, s, 0, NULL, 0));
}



/*- end of file */
