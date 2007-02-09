package wurfel.cli.ast;

import wurfel.Wurfel;
import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import wurfel.model.Apply;
import wurfel.model.ModelConnection;

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

        // Note: assuming left associativity for now.
        switch ( Wurfel.getExpressionOrder() )
        {
            case DIAGRAMMATIC:
                while ( iter.hasNext() )
                    result = new Apply( iter.next().evaluate( itp, mc ), result );
                break;

            case ANTIDIAGRAMMATIC:
                while ( iter.hasNext() )
                    result = new Apply( result, iter.next().evaluate( itp, mc ) );
                break;
        }

        return result;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
