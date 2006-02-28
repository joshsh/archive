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


#include "../p2_flags.h"
#include "../util/p2_array.h"
#include "../util/p2_hash_table.h"

#include <string.h>


extern void new_parse();
extern void end_parse();
extern int get_char_number();
extern int get_line_number();


#define DEBUG__BNFTOOL__PARSER  1

void production( char *s )
{
    printf( "Matched %s\n", s );
}


/******************************************************************************/


int yywrap()
{
    return 1;
}


void yyerror( const char *msg )
{
    printf( "line %d, char %d: %s\n",
        get_line_number(), get_char_number(), msg );
}


/******************************************************************************/


void *free0( void *p )
{
    free( p );
    return p;
}


void *sequence__delete( p2_array *s )
{
    /* Empty productions are allowed. */
    if ( s )
    {
        p2_array__for_all( s, free0 );
        p2_array__delete( s );
    }

    return ( void* ) 1;
}


typedef struct _rule
{
    char *name;
    p2_array *productions;

    int nullable;

    p2_hash_table *first_set;
    p2_hash_table *follow_set;

} rule;


rule *rule__new( char *name, p2_array *productions )
{
    rule *r = new( rule );
    r->name = name;
    r->productions = productions;

    r->first_set = p2_hash_table__new( 0, 0, 0, STRING_DEFAULTS );
    r->follow_set = p2_hash_table__new( 0, 0, 0, STRING_DEFAULTS );
    r->nullable = 0;

    return r;
}


void *rule__delete( rule *r )
{
    if ( r->name )
        free( r->name );

    if ( r->productions )
    {
         p2_array__for_all( r->productions, ( void*(*)(void*) ) sequence__delete );
         p2_array__delete( r->productions );
    }

    if ( r->first_set )
        p2_hash_table__delete( r->first_set );

    if ( r->follow_set )
        p2_hash_table__delete( r->follow_set );

    free( r );
    return r;
}


/******************************************************************************/


p2_hash_table *rule_dict;


p2_hash_table *target;
static void *add_to_target( void *p )
{
    p2_hash_table__add( target, p, p );
    return ( void* ) 1;
}


static void find_sets( p2_array *rules, p2_hash_table *dict )
{
    int changed = 1, i, j, k, n, n_rules = rules->size, n_prods, nullable_old, size_old;
    rule *r, *r2, *r3;
    p2_array *production;
    char *first, *second;

    /* Find the "nullable" attribute for each rule. */
    while ( changed )
    {
        changed = 0;

        for ( i = 0; i < n_rules; i++ )
        {
            r = ( rule* ) p2_array__get( rules, i );

            nullable_old = r->nullable;

            n_prods = r->productions->size;
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( p2_array* ) p2_array__get( r->productions, j );

                /* Null production makes the rule nullable. */
                if ( !production )
                    r->nullable = 1;

                /* A nullable rule at the head of a one of the rule's
                   productions makes the rule nullable. */
                else
                {
                    char *first = ( char* ) p2_array__get( production, 0 );
                    r2 = ( rule* ) p2_hash_table__lookup( dict, first );

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
            r = ( rule* ) p2_array__get( rules, i );
            target = r->first_set;
            size_old = target->size;

            n_prods = r->productions->size;
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( p2_array* ) p2_array__get( r->productions, j );

                if ( production )
                {
                    n = production->size;

                    for ( k = 0; k < n; k++ )
                    {
                        first = ( char* ) p2_array__get( production, k );
                        r2 = ( rule* ) p2_hash_table__lookup( dict, first );
                        if ( r2 )
                        {
                            p2_hash_table__for_all_keys(
                                r2->first_set,
                                ( void*(*)(void*) ) add_to_target );

                            if ( !r2->nullable )
                                break;
                        }

                        else
                        {
                            add_to_target( first );
                            break;
                        }
                    }
                }
            }

            if ( target->size != size_old )
                changed = 1;
        }
    }

    changed = 1;

    /* Find the follow set for each rule. */
    while ( changed )
    {
        changed = 0;

        for ( i = 0; i < n_rules; i++ )
        {
            r = ( rule* ) p2_array__get( rules, i );

            n_prods = r->productions->size;
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( p2_array* ) p2_array__get( r->productions, j );

                if ( production )
                {
                    n = production->size;

                    for ( k = 0; k < n - 1; k++ )
                    {
                        first = ( char* ) p2_array__get( production, k );
                        r2 = ( rule* ) p2_hash_table__lookup( dict, first );
                        if ( r2 )
                        {
                            target = r2->follow_set;
                            size_old = target->size;

                            second = ( char* ) p2_array__get( production, k + 1 );
                            r3 = ( rule* ) p2_hash_table__lookup( dict, second );

                            /* Add the first set of a nonterminal. */
                            if ( r3 )
                                p2_hash_table__for_all_keys(
                                    r3->first_set,
                                    ( void*(*)(void*) ) add_to_target );

                            /* Add the first set of a terminal (the symbol itself). */
                            else
                                add_to_target( second );

                            if ( target->size != size_old )
                                changed = 1;
                        }
                    }

                    for ( k = n - 1; k >= 0; k-- )
                    {
                        first = ( char* ) p2_array__get( production, k );
                        r2 = ( rule* ) p2_hash_table__lookup( dict, first );

                        if ( r2 )
                        {
                            target = r2->follow_set;
                            size_old = target->size;

                            /* Add the current rule's own follow set. */
                            p2_hash_table__for_all_keys(
                                r->follow_set,
                                ( void*(*)(void*) ) add_to_target );

                            if ( target->size != size_old )
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


void *print0( char *s )
{
    printf( " %s", s );
    return ( void* ) 1;
}


void *production__print( p2_array *p )
{
    printf( "   " );
    if ( p )
        p2_array__for_all( p, ( void*(*)(void*) ) print0 );
    printf( "\n" );
    return ( void* ) 1;
}


void *rule__print( rule *r )
{
    printf( "%s:\n", r->name );
    p2_array__for_all( r->productions, ( void*(*)(void*) ) production__print );
    return ( void* ) 1;
}


p2_array *target_array;
static void *add_to_target_array( void *p )
{
    p2_array__enqueue( target_array, p );
    return ( void* ) 1;
}


void *rule__print_first_set( rule *r )
{
    printf( "%s:  ", r->name );
    target_array = p2_array__new( 0, 0 );
    p2_hash_table__for_all_keys( r->first_set, ( void*(*)(void*) ) add_to_target_array );
    p2_array__mergesort( target_array, ( int(*)(void*, void*) ) strcmp );
    p2_array__for_all( target_array, ( void*(*)(void*) ) print0 );
    p2_array__delete( target_array );
    printf( "\n" );
    return ( void* ) 1;
}


void *rule__print_follow_set( rule *r )
{
    printf( "%s:  ", r->name );
    target_array = p2_array__new( 0, 0 );
    p2_hash_table__for_all_keys( r->follow_set, ( void*(*)(void*) ) add_to_target_array );
    p2_array__mergesort( target_array, ( int(*)(void*, void*) ) strcmp );
    p2_array__for_all( target_array, ( void*(*)(void*) ) print0 );
    p2_array__delete( target_array );
    printf( "\n" );
    return ( void* ) 1;
}


%}


%union
{
    char *string_t;
    struct _p2_array *array_t;
    struct _rule *rule_t;
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
        #if DEBUG__BNFTOOL__PARSER
        production( "input ::=  rules E_O_F" );
        #endif

        p2_array *rules = $1;
        find_sets( rules, rule_dict );

        /******************************/

        printf( "\n" );

        printf( "\n=== PRODUCTIONS ========================\n\n" );
        p2_array__for_all( rules, ( void*(*)(void*) ) rule__print );

        printf( "\n=== FIRST SETS =========================\n\n" );
        p2_array__for_all( rules, ( void*(*)(void*) ) rule__print_first_set );

        printf( "\n=== FOLLOW SETS ========================\n\n" );
        p2_array__for_all( rules, ( void*(*)(void*) ) rule__print_follow_set );

        printf( "\n" );

        /******************************/

        p2_array__for_all( rules, ( void*(*)(void*) ) rule__delete );
        p2_array__delete( rules );

        p2_hash_table__delete( rule_dict );

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

        $$ = p2_array__new( 0, 0 );
        rule_dict = p2_hash_table__new( 0, 0, 0, STRING_DEFAULTS );
    }

    | rules rule
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "rules ::=  rules rule" );
        #endif

        $$ = $1;
        p2_array__enqueue( $$, $2 );
    }
    ;


rule:

    NAME COLON productions SEMICOLON
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "rule ::=  NAME COLON productions SEMICOLON" );
        #endif

        $$ = rule__new( $1, $3 );
        p2_hash_table__add( rule_dict, $$->name, $$ );
    }
    ;


productions:

    production
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "productions ::=  production" );
        #endif

        $$ = p2_array__new( 0, 0 );
        p2_array__enqueue( $$, $1 );
    }

    | productions OR production
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "productions ::=  productions OR production" );
        #endif

        $$ = $1;
        p2_array__enqueue( $$, $3 );
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

        $$ = p2_array__new( 0, 0 );
        p2_array__enqueue( $$, $1 );
    }

    | sequence NAME
    {
        #if DEBUG__BNFTOOL__PARSER
        production( "sequence ::=  sequence NAME" );
        #endif

        $$ = $1;
        p2_array__enqueue( $$, $2 );
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


int main()
{
    return yyparse();
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
