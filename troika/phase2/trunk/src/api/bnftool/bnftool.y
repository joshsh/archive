%{

/**

\file  bnftool.y

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*******************************************************************************

Phase2 language API, Copyright (C) 2005 Joshua Shinavier.

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

*******************************************************************************/


#include <util/Array.h>
#include <util/Dictionary.h>


extern void
new_parse();

extern void
end_parse();

extern int
get_char_number();

extern int
get_line_number();


#define DEBUG__BNFTOOL__PARSER  1

static void
production( char *s )
{
    printf( "Matched %s\n", s );
}


/******************************************************************************/


int
yywrap()
{
    return 1;
}


void
yyerror( const char *msg )
{
    printf( "line %d, char %d: %s\n",
        get_line_number(), get_char_number(), msg );
}


/******************************************************************************/


typedef struct Rule Rule;

struct Rule
{
    char *name;
    Array *productions;

    int nullable;

    Dictionary *first_set;
    Dictionary *follow_set;
};


static Rule *
rule__new( char *name, Array *productions )
{
    Rule *r = new( Rule );
    r->name = name;
    r->productions = productions;

    r->first_set = dictionary__new();
    r->follow_set = dictionary__new();
    r->nullable = 0;

    return r;
}


static void *
rule__delete( Rule **rpp )
{
    Rule *r = *rpp;

    void *sequence__delete( Array **s )
    {
        void *helper( void **refp )
        {
            free( *refp );
            return 0;
        }

        /* Empty productions are allowed. */
        if ( *s )
        {
            array__walk( *s, ( Dist_f ) helper );
            array__delete( *s );
        }

        return 0;
    }

    if ( r->name )
        free( r->name );

    if ( r->productions )
    {
        array__walk( r->productions, ( Dist_f ) sequence__delete );
        array__delete( r->productions );
    }

    if ( r->first_set )
        dictionary__delete( r->first_set );

    if ( r->follow_set )
        dictionary__delete( r->follow_set );

    free( r );

    return 0;
}


/******************************************************************************/


static Dictionary *rule_dict;


static void
find_sets( Array *rules, Dictionary *dict )
{
    int changed = 1, i, j, k, n, n_rules = array__size( rules ), n_prods, nullable_old, size_old;
    Rule *r, *r2, *r3;
    Array *production;
    char *first, *second;
    Dictionary *d;

    void *add_to_dict( char **name )
    {
        dictionary__add( d, *name, *name );
        return 0;
    }

    /* Find the "nullable" attribute for each Rule. */
    while ( changed )
    {
        changed = 0;

        for ( i = 0; i < n_rules; i++ )
        {
            r = array__get( rules, i );

            nullable_old = r->nullable;

            n_prods = array__size( r->productions );
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( Array* ) array__get( r->productions, j );

                /* Null production makes the Rule nullable. */
                if ( !production )
                    r->nullable = 1;

                /* A nullable Rule at the head of a one of the Rule's
                   productions makes the Rule nullable. */
                else
                {
                    char *first = ( char* ) array__get( production, 0 );
                    r2 = dictionary__lookup( dict, first );

                    /* If non-terminal and nullable... */
                    if ( r2 && r2->nullable )
                        r->nullable = 1;
                }
            }

            if ( r->nullable != nullable_old )
                changed = 1;
        }
    }

    changed = 1;

    /* Find the first set for each rule. */
    while ( changed )
    {
        changed = 0;

        for ( i = 0; i < n_rules; i++ )
        {
            r = array__get( rules, i );
            size_old = hash_table__size( r->first_set );

            n_prods = array__size( r->productions );
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( Array* ) array__get( r->productions, j );

                if ( production )
                {
                    n = array__size( production );

                    for ( k = 0; k < n; k++ )
                    {
                        first = ( char* ) array__get( production, k );
                        r2 = dictionary__lookup( dict, first );
                        d = r->first_set;

                        if ( r2 )
                        {
                            dictionary__walk( r2->first_set, ( Dist_f ) add_to_dict );

                            if ( !r2->nullable )
                                break;
                        }

                        else
                        {
                            add_to_dict( &first );
                            break;
                        }
                    }
                }
            }

            if ( hash_table__size( r->first_set ) != size_old )
                changed = 1;
        }
    }

    changed = 1;

    /* Find the follow set for each Rule. */
    while ( changed )
    {
        changed = 0;

        for ( i = 0; i < n_rules; i++ )
        {
            r = array__get( rules, i );

            n_prods = array__size( r->productions );
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( Array* ) array__get( r->productions, j );

                if ( production )
                {
                    n = array__size( production );

                    for ( k = 0; k < n - 1; k++ )
                    {
                        first = ( char* ) array__get( production, k );
                        r2 = dictionary__lookup( dict, first );
                        if ( r2 )
                        {
                            size_old = hash_table__size( r2->follow_set );

                            second = ( char* ) array__get( production, k + 1 );
                            r3 = dictionary__lookup( dict, second );
                            d = r2->follow_set;

                            /* Add the first set of a nonterminal. */
                            if ( r3 )
                            {
                                dictionary__walk( r3->first_set, ( Dist_f ) add_to_dict );
                            }

                            /* Add the first set of a terminal (the symbol itself). */
                            else
                                add_to_dict( &second );

                            if ( hash_table__size( r2->follow_set ) != size_old )
                                changed = 1;
                        }
                    }

                    for ( k = n - 1; k >= 0; k-- )
                    {
                        first = ( char* ) array__get( production, k );
                        r2 = dictionary__lookup( dict, first );

                        if ( r2 )
                        {
                            size_old = hash_table__size( r2->follow_set );

                            /* Add the current Rule's own follow set. */
                            d = r2->follow_set;
                            dictionary__walk( r->follow_set, ( Dist_f ) add_to_dict );

                            if ( hash_table__size( r2->follow_set ) != size_old )
                                changed = 1;

                            if ( !r2->nullable )
                                break;
                        }

                        else
                            break;
                    }
                }
            }
        }
    }
}


/******************************************************************************/


static void *
print( char **s )
{
    printf( " %s", *s );
    return 0;
}


static void *
rule__print( Rule **rpp )
{
    Rule *r = *rpp;

    void *production__print( Array **p )
    {
        printf( "   " );
        if ( *p )
            array__walk( *p, ( Dist_f ) print );
        printf( "\n" );
        return 0;
    }

    printf( "%s:\n", r->name );
    array__walk( r->productions, ( Dist_f ) production__print );
    return 0;
}


static void *
rule__print_first_set( Rule **rpp )
{
    Rule *r = *rpp;

    Array *a;

    printf( "%s:  ", r->name );
    a = dictionary__keys( r->first_set );
    array__sort( a, ( Comparator ) strcmp );
    array__walk( a, ( Dist_f ) print );
    array__delete( a );
    printf( "\n" );

    return 0;
}


static void *
rule__print_follow_set( Rule **rpp )
{
    Rule *r = *rpp;

    Array *a;

    printf( "%s:  ", r->name );
    a = dictionary__keys( r->follow_set );
    array__sort( a, ( Comparator ) strcmp );
    array__walk( a, ( Dist_f ) print );
    array__delete( a );
    printf( "\n" );

    return 0;
}


%}


%union
{
    char *string_t;
    struct Array *array_t;
    struct Rule *rule_t;
}


%token ACTION COLON SEMICOLON OR E_O_F

%token <string_t> NAME

%type <array_t> sequence production productions rules
%type <rule_t> rule

/** Report more detailed parse error messages. */
%error-verbose

%start input


%%


input:

    rules E_O_F
    {
        Array *rules = $1;

        #if DEBUG__BNFTOOL__PARSER
        production( "input ::=  rules E_O_F" );
        #endif

        find_sets( rules, rule_dict );

        /******************************/

        printf( "\n" );

        printf( "\n=== PRODUCTIONS ========================\n\n" );
        array__walk( rules, ( Dist_f ) rule__print );

        printf( "\n=== FIRST SETS =========================\n\n" );
        array__walk( rules, ( Dist_f ) rule__print_first_set );

        printf( "\n=== FOLLOW SETS ========================\n\n" );
        array__walk( rules, ( Dist_f ) rule__print_follow_set );

        printf( "\n" );

        /******************************/

        array__walk( rules, ( Dist_f ) rule__delete );
        array__delete( rules );

        dictionary__delete( rule_dict );

        /******************************/

        YYACCEPT;
    }
    ;


rules:

    {
        #if DEBUG__BNFTOOL__PARSER
        production( "rules ::=  [null]" );
        #endif

        new_parse();

        $$ = array__new( 0, 0 );
        rule_dict = dictionary__new();
    }

    | rules rule
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "rules ::=  rules rule" );
        #endif

        $$ = $1;
        array__enqueue( $$, $2 );
    }
    ;


rule:

    NAME COLON productions SEMICOLON
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "rule ::=  NAME COLON productions SEMICOLON" );
        #endif

        $$ = rule__new( $1, $3 );
        dictionary__add( rule_dict, $$->name, $$ );
    }
    ;


productions:

    production
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "productions ::=  production" );
        #endif

        $$ = array__new( 0, 0 );
        array__enqueue( $$, $1 );
    }

    | productions OR production
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "productions ::=  productions OR production" );
        #endif

        $$ = $1;
        array__enqueue( $$, $3 );
    }
    ;


production:

    action
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "production ::=  action" );
        #endif

        $$ = 0;
    }

    | sequence action
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "production ::=  sequence action" );
        #endif

        $$ = $1;
    }
    ;


sequence:

    NAME
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "sequence ::=  NAME" );
        #endif

        $$ = array__new( 0, 0 );
        array__enqueue( $$, $1 );
    }

    | sequence NAME
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "sequence ::=  sequence NAME" );
        #endif

        $$ = $1;
        array__enqueue( $$, $2 );
    }
    ;


action:

    ACTION
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "action ::=  ACTION" );
        #endif
    }
    ;


%%


int
main()
{
    return yyparse();
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
