package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.rio.RDFHandler;

/**
 * An RdfSink which passes its input into an RDFHandler.
 */
public class SesameOutputAdapter extends RdfSink
{
	private RDFHandler handler;

	public SesameOutputAdapter( final RDFHandler handler )
	{
		this.handler = handler;
	}

	public void startRDF() throws RippleException
	{
		try
		{
			handler.startRDF();
		}

		catch ( org.openrdf.rio.RDFHandlerException e )
		{
			throw new RippleException( e );
		}
	}

	public void endRDF() throws RippleException
	{
		try
		{
			handler.endRDF();
		}

		catch ( org.openrdf.rio.RDFHandlerException e )
		{
			throw new RippleException( e );
		}
	}

	public void put( final Statement st ) throws RippleException
	{
		try
		{
			handler.handleStatement( st );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	public void put( final Namespace ns ) throws RippleException
	{
		try
		{
			handler.handleNamespace( ns.getPrefix(), ns.getName() );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	public void put( final String comment ) throws RippleException
	{
		try
		{
			handler.handleComment( comment );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}
}

// kate: tab-width 4
