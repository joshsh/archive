package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.Apply;
import wurfel.model.Combinator;
import wurfel.model.NodeSet;
import wurfel.model.ModelConnection;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_U extends Combinator
{
    public Combinator_U( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "U" ), mc );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      ModelConnection mc )
        throws WurfelException
    {
        Value x, y;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();
        y = argIter.next();

        Value result = new Apply(
            y,
            new Apply( new Apply( x, x ), y ) );

        return new NodeSet( result );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
