package net.fortytwo.ripple.cli.ast;

import wurfel.Wurfel;
import net.fortytwo.ripple.cli.Interpreter;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.Apply;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;

import org.openrdf.model.Value;

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

public class SequenceNode extends Ast
{
    private static final boolean s_useDiagrammaticSyntax = true;
    private List<Ast> children;

    public SequenceNode()
    {
        children = new ArrayList<Ast>();
    }

/*
    public void add( Ast node )
    {
        children.add( node );
    }
*/

    public void push( Ast node )
    {
        children.add( 0, node );
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException
    {
        if ( children.size() < 1 )
            throw new WurfelException( "empty sequence" );

        Iterator<Ast> iter = children.iterator();
        Value result = iter.next().evaluate( itp, mc );
        ListNode<Value> stack = new ListNode<Value>( result );

        boolean comp = ( Wurfel.getEvaluationStyle() == Wurfel.EvaluationStyle.COMPOSITIONAL );

        // Note: assuming left associativity for now.
        switch ( Wurfel.getExpressionOrder() )
        {
            case DIAGRAMMATIC:
                while ( iter.hasNext() )
                    if ( comp )
                        stack = stack.push( iter.next().evaluate( itp, mc ) );
                    else
                        result = new Apply( iter.next().evaluate( itp, mc ), result );
                break;

            case ANTIDIAGRAMMATIC:
                while ( iter.hasNext() )
                    if ( comp )
System.err.println( "not yet implemented" );
                    else
                        result = new Apply( result, iter.next().evaluate( itp, mc ) );
                break;
        }

        if ( comp )
            return stack;
        else
            return result;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
