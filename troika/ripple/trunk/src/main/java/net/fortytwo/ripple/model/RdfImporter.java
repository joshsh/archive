package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;

public class RdfImporter implements RdfSink
{
	RdfSink sink;
	Resource[] contexts;
	ModelConnection mc;
	boolean override;

	public RdfImporter( final ModelConnection mc,
						final RdfSink sink,
						final Resource... contexts )
	{
		this.mc = mc;
		this.sink = sink;
		this.contexts = contexts;

		override = Ripple.preferNewestNamespaceDefinitions();
	}

	public void put( final Statement st ) throws RippleException
	{
		mc.add( st, contexts );
		sink.put( st );
	}

	public void put( final Namespace ns ) throws RippleException
	{
		mc.setNamespace( ns.getPrefix(), ns.getName(), override );
		sink.put( ns );
	}

	public void put( final String comment) throws RippleException
	{
		sink.put( comment );
	}
}

// kate: tab-width 4
