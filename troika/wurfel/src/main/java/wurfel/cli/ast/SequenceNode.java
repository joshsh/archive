package wurfel.cli.ast;

import wurfel.Context;
import wurfel.WurfelException;
import wurfel.model.Apply;

import org.openrdf.model.Value;

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

    public void add( Ast node )
    {
        children.add( node );
    }

    public Value evaluate( Context context )
        throws WurfelException
    {
        if ( children.size() < 1 )
            throw new WurfelException( "empty sequence" );

        Iterator<Ast> iter = children.iterator();
        Value result = iter.next().evaluate( context );

        // Note: uses left-associative, antidiagrammatic-order syntax.
        while ( iter.hasNext() )
            result = new Apply( result, iter.next().evaluate( context ) );

        return result;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
