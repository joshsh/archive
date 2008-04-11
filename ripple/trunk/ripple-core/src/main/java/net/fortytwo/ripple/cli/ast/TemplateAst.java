package net.fortytwo.ripple.cli.ast;

import java.util.List;

/**
 * Author: josh
 * Date: Apr 8, 2008
 * Time: 12:28:41 PM
 */
public class TemplateAst extends ListAst
{
    public TemplateAst( final ListAst template, final ListAst expression )
    {
        ListAst expr = expression;

        ListAst params = template;//.invert();
        while ( !ListAst.isNil( params ) )
        {
//System.out.println("params = " + params);
//System.out.println("    expr = " + expr);
            Ast p = params.getFirst();
//System.out.println("    p = " + p);
            if ( p instanceof ListAst )
            {
                expr = new TemplateAst( (ListAst) p, expr )
                        .push( new OperatorAst() );
            }

            else if ( p instanceof KeywordAst )
            {
                expr = new LambdaAst( ( (KeywordAst) p ).getName(), expr );
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