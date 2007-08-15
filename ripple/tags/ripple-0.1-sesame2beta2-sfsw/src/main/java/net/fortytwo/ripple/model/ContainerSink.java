package net.fortytwo.ripple.model;

import java.util.Collection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

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

// kate: tab-width 4
