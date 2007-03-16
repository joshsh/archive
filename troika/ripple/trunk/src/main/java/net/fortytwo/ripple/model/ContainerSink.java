package net.fortytwo.ripple.model;

import java.util.Collection;

import org.openrdf.model.Value;

import net.fortytwo.ripple.RippleException;

public class ContainerSink extends Container implements Sink<RippleValue>
{
    public ContainerSink()
    {
        super();
    }

    public ContainerSink( RippleValue v )
    {
        super( v );
    }

    public ContainerSink( Collection<RippleValue> other )
    {
        super( other );
    }

    public void put( RippleValue v )
        throws RippleException
    {
        add( v );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
