package net.fortytwo.ripple.model;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class ForwardPredicateQuery implements Function
{
    URI pred;

    public ForwardPredicateQuery( URI predicate )
    {
        pred = predicate;
    }

    public int arity()
    {
        return 1;
    }

    public void applyTo( RippleStack stack,
                         Sink<RippleStack> sink,
                         ModelConnection mc )
        throws RippleException
    {
        Value first = stack.getFirst();
        RippleStack rest = stack.getRest();

//        mc.multiply( first, pred, sink );

// TODO: do we need a call to "toGraph" around "first"?
        Iterator<Value> objects = mc.getModel().multiply( first, pred, mc ).iterator();
        while ( objects.hasNext() )
            sink.put( new RippleStack( objects.next(), rest ) );
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        p.print( pred );
    }

public URI getUri()
{
return null;
}

public void checkArguments( RippleStack args )
    throws RippleException
{}

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
