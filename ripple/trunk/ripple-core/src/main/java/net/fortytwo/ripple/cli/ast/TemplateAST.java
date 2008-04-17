/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

/**
 * Author: josh
 * Date: Apr 8, 2008
 * Time: 12:28:41 PM
 */
public class TemplateAST extends ListAST
{
    public TemplateAST( final ListAST template, final ListAST expression )
    {
        ListAST expr = expression;

        ListAST params = template;//.invert();
        while ( !ListAST.isNil( params ) )
        {
//System.out.println("params = " + params);
//System.out.println("    expr = " + expr);
            AST p = params.getFirst();
//System.out.println("    p = " + p);
            if ( p instanceof ListAST)
            {
                expr = new TemplateAST( (ListAST) p, expr )
                        .push( new OperatorAST() );
            }

            else if ( p instanceof KeywordAST)
            {
                expr = new LambdaAST( ( (KeywordAST) p ).getName(), expr );
            }

            else
            {
                throw new IllegalArgumentException( "bad parameter AST: " + p );
            }

            params = params.getRest();
        }

        this.first = expr.getFirst();
        this.rest = expr.getRest();
    }
}