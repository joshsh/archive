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
        private Sink<RippleList> sink;
        private RippleList stack;

        public ForwardPredicateQueryResultSink( RippleList stack, Sink<RippleList> sink )
        {
            this.stack = stack;
            this.sink = sink;
        }

        public void put( RdfValue v )
            throws RippleException
        {
            sink.put(
                new RippleList( bridge.get( v ), stack ) );
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

	public boolean isOperator()
	{
		return false;
	}

    public void applyTo( RippleList stack,
                         Sink<RippleList> sink,
                         ModelConnection mc )
        throws RippleException
    {
        bridge = mc.getModel().getBridge();

        RippleValue first = stack.getFirst();
        RippleList rest = stack.getRest();

        Sink<RdfValue> querySink = new ForwardPredicateQueryResultSink( rest, sink );

        mc.getModel().multiply( first.toRdf( mc ), pred, querySink, mc );
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
