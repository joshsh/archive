package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

public class SequenceNode extends Ast
{
    private List<Ast> children;

    public SequenceNode()
    {
        children = new ArrayList<Ast>();
    }

    public void push( Ast node )
    {
        children.add( 0, node );
    }

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        if ( children.size() < 1 )
            throw new RippleException( "empty sequence" );

        Iterator<Ast> iter = children.iterator();
        RippleValue result = iter.next().evaluate( itp, mc );
        RippleList stack = new RippleList( result );

        boolean comp = ( Ripple.getEvaluationStyle() == Ripple.EvaluationStyle.COMPOSITIONAL );

        // Note: assuming left associativity for now.
        switch ( Ripple.getExpressionOrder() )
        {
            case DIAGRAMMATIC:
                while ( iter.hasNext() )
                    if ( comp )
                        stack = stack.push( iter.next().evaluate( itp, mc ) );
//                    else
//                        result = new Apply( iter.next().evaluate( itp, mc ), result );
                break;

            case ANTIDIAGRAMMATIC:
                while ( iter.hasNext() )
                    if ( comp )
System.err.println( "not yet implemented" );
//                    else
//                        result = new Apply( result, iter.next().evaluate( itp, mc ) );
                break;
        }

        if ( comp )
            return stack;
        else
            return result;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
