package net.fortytwo.ripple.model;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

public class ForwardPredicateQuery implements Function
{
    private RdfValue pred;
    private ModelBridge bridge;

    private class ForwardPredicateQueryResultSink implements Sink<RdfValue>
    {
        private Sink<RippleStack> sink;
        private RippleStack stack;

        public ForwardPredicateQueryResultSink( RippleStack stack, Sink<RippleStack> sink )
        {
            this.stack = stack;
            this.sink = sink;
        }

        public void put( RdfValue v )
            throws RippleException
        {
            sink.put(
                new RippleStack( bridge.get( v ), stack ) );
        }
    }

    public ForwardPredicateQuery( RdfValue predicate )
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
        bridge = mc.getModel().getBridge();

        RippleValue first = stack.getFirst();
        RippleStack rest = stack.getRest();

        Sink<RdfValue> querySink = new ForwardPredicateQueryResultSink( rest, sink );

        mc.multiply( first.toRdf(), pred, querySink );
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        p.print( pred );
    }

public RdfValue toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
