package wurfel.cli.ast;

import wurfel.Wurfel;
import wurfel.Context;
import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import wurfel.model.Apply;
import wurfel.model.EvaluationContext;

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

    public void add( Ast node )
    {
        children.add( node );
    }

    public Value evaluate( Interpreter itp, EvaluationContext evalContext )
        throws WurfelException
    {
        if ( children.size() < 1 )
            throw new WurfelException( "empty sequence" );

        Iterator<Ast> iter = children.iterator();
        Value result = iter.next().evaluate( itp, evalContext );

        // Note: assuming left associativity for now.
        switch ( Wurfel.getExpressionOrder() )
        {
            case DIAGRAMMATIC:
                while ( iter.hasNext() )
                    result = new Apply( iter.next().evaluate( itp, evalContext ), result );
                break;

            case ANTIDIAGRAMMATIC:
                while ( iter.hasNext() )
                    result = new Apply( result, iter.next().evaluate( itp, evalContext ) );
                break;
        }

        return result;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
