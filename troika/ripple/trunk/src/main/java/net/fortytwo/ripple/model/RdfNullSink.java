package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class RdfNullSink implements RdfSink
{
	public void put( final Statement st ) throws RippleException {}
	public void put( final Namespace ns ) throws RippleException {}
	public void put( final String comment ) throws RippleException {}
}

// kate: tab-width 4
