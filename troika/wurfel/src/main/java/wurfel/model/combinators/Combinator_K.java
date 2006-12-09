package wurfel.model.combinators;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.Combinator;
import wurfel.model.NodeSet;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_K extends Combinator
{
    private static final URI
        s_wurfelTestKUri = Wurfel.getWurfelTestUri( "K" );

    public Combinator_K( Context context )
        throws WurfelException
    {
        super( s_wurfelTestKUri, context );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      Context context )
        throws WurfelException
    {
        Iterator<Value> argIter = args.iterator();
        Value x = argIter.next();
//        Value y = argIter.next();

        return new NodeSet( x );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
