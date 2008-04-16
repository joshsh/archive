/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import java.util.List;

/**
 * Author: josh
 * Date: Apr 8, 2008
 * Time: 12:28:41 PM
 */
public class LambdaAst extends ListAst
{
    private static final String
            CONS = "cons",
            DIP = "dip",
            DUP = "dup",
            POP = "pop";

    public LambdaAst( final String boundVariable, final ListAst expression )
    {
        ListAst expr = toConcatenativeExpression( boundVariable, expression );
        this.first = expr.getFirst();
        this.rest = expr.getRest();
    }

    public LambdaAst( List<String> boundVars, ListAst expression )
    {
        ListAst expr = expression;
//System.out.println( "original: " + expr );

        for ( int i = 0; i < boundVars.size(); i++ )
        {
//System.out.println( "replacing variable " + boundVars.get( i ));
            String boundVariable = boundVars.get( i );
            expr = toConcatenativeExpression( boundVariable, expr );
//System.out.println( "    intermediate: " + expr );
        }

        this.first = expr.getFirst();
        this.rest = expr.getRest();
    }

    private static ListAst toConcatenativeExpression(final String boundVariable, final ListAst expression)
    {
        int fo = firstOccurrence( boundVariable, expression );
 //System.out.println("(1) first occurrence of " + boundVariable + " in " + expression + " is " + fo );
 //        int count = countOccurrences( boundVariable, expression );

        if ( 0 > fo )
        {
            return expression
                    .push( new OperatorAst() )
                    .push( new KeywordAst( POP ) );
        }

        else
        {
           ListAst expr = expression;

           /*for ( int i = 0; i < count; i++ )
           {
               int fo = firstOccurrence( boundVariable, expr );
               expr = replaceFirstOccurrence( boundVariable, expr, fo );
           }

           for ( int i = 1; i < count; i++ )
           {
               expr = expr
                       .push( new OperatorAst() )
                       .push( new KeywordAst( "dup" ) );
           }*/

            int i = 0;
            while ( fo >= 0 )
            {
                expr = replaceFirstOccurrence( boundVariable, expr, fo );
                if ( i > 0 ) {
                    expr = expr
                            .push( new OperatorAst() )
                            .push( new KeywordAst( DUP ) );
                }
                i++;
                fo = firstOccurrence(boundVariable, expr);
//     System.out.println("(2) first occurrence of " + boundVariable + " in " + expr + " is " + fo );
            }

            return expr;
        }
    }

    private static ListAst replaceFirstOccurrence( final String name, ListAst list, final int i )
    {
        // If the to-be-replaced node is not at the head of the list, transform
        // the list so that it is at the head.
        if ( i > 0 )
        {
            ListAst head = new ListAst();
            for ( int j = 0; j < i; j++ )
            {
                head = new ListAst( list.getFirst(), head );
                list = list.getRest();
            }

            head = head.invert();
            Ast first = list.getFirst();
            list = list.getRest()
                    .push( new OperatorAst() )
                    .push( new KeywordAst( DIP ) )
                    .push( head )
                    .push( first );
        }

        Ast first = list.getFirst();

        if ( first instanceof ListAst )
        {
            return list.getRest()
                    .push( new OperatorAst() )
                    .push( new KeywordAst( CONS ) )
                    .push( toConcatenativeExpression( name, (ListAst) first ) );
        }

        // Note: assumed to be instanceof KeywordAst
        else
        {
            return list.getRest();
        }
    }

     /*private Ast replaceName( final String oldName, final String newName, final Ast ast )
     {
         if ( ast instanceof KeywordAst )
         {
             if ( ( (KeywordAst) ast ).getName().equals( oldName ) )
             {
                 return new KeywordAst( newName );
             }

             else
             {
                 return ast;
             }
         }

         else if ( ast instanceof ListAst )
         {
             ListAst list = (ListAst) ast;
             if ( ListAst.isNil( list ) )
             {
                 return list;
             }

             else
             {
                 return new ListAst( replaceName( oldName, newName, list.getFirst() ),
                         (ListAst) replaceName( oldName, newName, list.getRest() ) );
             }
         }

         else
         {
             return ast;
         }
     }

     private Ast el( ListAst list, final int i )
     {
         for ( int j = 0; j < i; j++ )
         {
             list = list.getRest();
         }

         return list.getFirst();
     }*/

    private static int countOccurrences( final String name, ListAst list )
    {
        int count = 0;
        while ( !ListAst.isNil( list ) )
        {
            Ast a = list.getFirst();
            if ( a instanceof KeywordAst )
            {
                if ( ( (KeywordAst) a ).getName().equals( name ) )
                {
                    count++;
                }
            }

            else if ( a instanceof ListAst )
            {
                int j = countOccurrences( name, (ListAst) a );
                if ( j >= 0 )
                {
                    count++;
                }
            }

            list = list.getRest();
        }

        return count;
    }

    private static int firstOccurrence( final String name, ListAst list )
    {
        int i = 0;
        while ( !ListAst.isNil( list ) )
        {
            Ast a = list.getFirst();
            if ( a instanceof KeywordAst )
            {
                if ( ( (KeywordAst) a ).getName().equals( name ) )
                {
                    return i;
                }
            }

            else if ( a instanceof ListAst )
            {
                int j = firstOccurrence( name, (ListAst) a );
                if ( j >= 0 )
                {
                    return i;
                }
            }

            list = list.getRest();
            i++;
        }

        return -1;
    }
}
