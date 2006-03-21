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


#include "../util/p2_array.h"
#include "../util/p2_dictionary.h"

#include <string.h>


extern void new_parse();
extern void end_parse();
extern int get_char_number();
extern int get_line_number();


#define DEBUG__BNFTOOL__PARSER  1

static void production( char *s )
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


static p2_action *delete__proc( void *p, void *ignored )
{
    free( p );
    return 0;
}


static p2_action * sequence__delete__proc( p2_array *s, void *ignored )
{
    p2_procedure proc;

    /* Empty productions are allowed. */
    if ( s )
    {
        proc.execute = ( procedure ) delete__proc;
        p2_array__distribute( s, &proc );
        p2_array__delete( s );
    }

    return 0;
}


typedef struct _rule
{
    char *name;
    p2_array *productions;

    int nullable;

    p2_dictionary *first_set;
    p2_dictionary *follow_set;

} rule;


rule *rule__new( char *name, p2_array *productions )
{
    rule *r = new( rule );
    r->name = name;
    r->productions = productions;

    r->first_set = p2_dictionary__new();
    r->follow_set = p2_dictionary__new();
    r->nullable = 0;

    return r;
}


static p2_action * rule__delete( rule *r, void *ignored )
{
    p2_procedure proc;

    if ( r->name )
        free( r->name );

    if ( r->productions )
    {
        proc.execute = ( procedure ) sequence__delete__proc;
        p2_array__distribute( r->productions, &proc );

        p2_array__delete( r->productions );
    }

    if ( r->first_set )
        p2_dictionary__delete( r->first_set );

    if ( r->follow_set )
        p2_dictionary__delete( r->follow_set );

    free( r );

    return 0;
}


/******************************************************************************/


p2_dictionary *rule_dict;


static p2_action * add_to_dict( char *name, p2_dictionary *d )
{
    p2_dictionary__add( d, name, name );
    return 0;
}


static void find_sets( p2_array *rules, p2_dictionary *dict )
{
    int changed = 1, i, j, k, n, n_rules = p2_array__size( rules ), n_prods, nullable_old, size_old;
    rule *r, *r2, *r3;
    p2_array *production;
    char *first, *second;
    p2_procedure proc;

    /* Find the "nullable" attribute for each rule. */
    while ( changed )
    {
        changed = 0;

        for ( i = 0; i < n_rules; i++ )
        {
            r = ( rule* ) p2_array__get( rules, i );

            nullable_old = r->nullable;

            n_prods = p2_array__size( r->productions );
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
                    r2 = ( rule* ) p2_dictionary__lookup( dict, first );

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
            size_old = r->first_set->size;

            n_prods = p2_array__size( r->productions );
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( p2_array* ) p2_array__get( r->productions, j );

                if ( production )
                {
                    n = p2_array__size( production );

                    for ( k = 0; k < n; k++ )
                    {
                        first = ( char* ) p2_array__get( production, k );
                        r2 = ( rule* ) p2_dictionary__lookup( dict, first );
                        if ( r2 )
                        {
                            proc.execute = ( procedure ) add_to_dict;
                            proc.state = r->first_set;
                            p2_dictionary__distribute( r2->first_set, &proc );

                            if ( !r2->nullable )
                                break;
                        }

                        else
                        {
                            add_to_dict( first, r->first_set );
                            break;
                        }
                    }
                }
            }

            if ( r->first_set->size != size_old )
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

            n_prods = p2_array__size( r->productions );
            for ( j = 0; j < n_prods; j++ )
            {
                production = ( p2_array* ) p2_array__get( r->productions, j );

                if ( production )
                {
                    n = p2_array__size( production );

                    for ( k = 0; k < n - 1; k++ )
                    {
                        first = ( char* ) p2_array__get( production, k );
                        r2 = ( rule* ) p2_dictionary__lookup( dict, first );
                        if ( r2 )
                        {
                            size_old = r2->follow_set->size;

                            second = ( char* ) p2_array__get( production, k + 1 );
                            r3 = ( rule* ) p2_dictionary__lookup( dict, second );

                            /* Add the first set of a nonterminal. */
                            if ( r3 )
                            {
                                proc.execute = ( procedure ) add_to_dict;
                                proc.state = r2->follow_set;
                                p2_dictionary__distribute( r3->first_set, &proc );
                            }

                            /* Add the first set of a terminal (the symbol itself). */
                            else
                                add_to_dict( second, r2->follow_set );

                            if ( r2->follow_set->size != size_old )
                                changed = 1;
                        }
                    }

                    for ( k = n - 1; k >= 0; k-- )
                    {
                        first = ( char* ) p2_array__get( production, k );
                        r2 = ( rule* ) p2_dictionary__lookup( dict, first );

                        if ( r2 )
                        {
                            size_old = r2->follow_set->size;

                            /* Add the current rule's own follow set. */
                            proc.execute = ( procedure ) add_to_dict;
                            proc.state = r2->follow_set;
                            p2_dictionary__distribute( r->follow_set, &proc );

                            if ( r2->follow_set->size != size_old )
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


static p2_action * print_proc( char *s, void *ignored )
{
    printf( " %s", s );
    return 0;
}


static p2_action * production__print( p2_array *p, void *ignored )
{
    p2_procedure proc;
    proc.execute = ( procedure ) print_proc;

    printf( "   " );
    if ( p )
        p2_array__distribute( p, &proc );
    printf( "\n" );
    return 0;
}


static void *rule__print( rule *r )
{
    p2_procedure proc;
    proc.execute = ( procedure ) production__print;

    printf( "%s:\n", r->name );
    p2_array__distribute( r->productions, &proc );
    return 0;
}


static p2_action * rule__print_first_set( rule *r, void *ignored )
{
    p2_array *a;
    p2_procedure proc;
    proc.execute = ( procedure ) print_proc;

    printf( "%s:  ", r->name );
    a = p2_dictionary__keys( r->first_set );
    p2_array__sort( a, ( comparator ) strcmp );
    p2_array__distribute( a, &proc );
    p2_array__delete( a );
    printf( "\n" );

    return 0;
}


static p2_action * rule__print_follow_set( rule *r )
{
    p2_array *a;
    p2_procedure proc;
    proc.execute = ( procedure ) print_proc;

    printf( "%s:  ", r->name );
    a = p2_dictionary__keys( r->follow_set );
    p2_array__sort( a, ( comparator ) strcmp );
    p2_array__distribute( a, &proc );
    p2_array__delete( a );
    printf( "\n" );

    return 0;
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
        p2_array *rules = $1;

        p2_procedure print_p;
        p2_procedure delete_p;

        #if DEBUG__BNFTOOL__PARSER
        production( "input ::=  rules E_O_F" );
        #endif

        find_sets( rules, rule_dict );

        /******************************/

        printf( "\n" );

        printf( "\n=== PRODUCTIONS ========================\n\n" );
        print_p.execute = ( procedure ) rule__print;
        p2_array__distribute( rules, &print_p );

        printf( "\n=== FIRST SETS =========================\n\n" );
        print_p.execute = ( procedure ) rule__print_first_set;
        p2_array__distribute( rules, &print_p );

        printf( "\n=== FOLLOW SETS ========================\n\n" );
        print_p.execute = ( procedure ) rule__print_follow_set;
        p2_array__distribute( rules, &print_p );

        printf( "\n" );

        /******************************/

        delete_p.execute = ( procedure ) rule__delete;
        p2_array__distribute( rules, &delete_p );
        p2_array__delete( rules );

        p2_dictionary__delete( rule_dict );

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
        rule_dict = p2_dictionary__new();
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
        p2_dictionary__add( rule_dict, $$->name, $$ );
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
