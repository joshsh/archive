package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.rio.RDFHandler;

/**
 * An RdfSink which passes its input into an RDFHandler.
 */
public class SesameOutputAdapter implements RdfSink
{
	private RDFHandler handler;

	private Sink<Statement> stSink;
	private Sink<Namespace> nsSink;
	private Sink<String> cmtSink;

	public SesameOutputAdapter( final RDFHandler handler )
	{
		this.handler = handler;

		stSink = new Sink<Statement>()
		{
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
		};

		nsSink = new Sink<Namespace>()
		{
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
		};

		cmtSink = new Sink<String>()
		{
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
		};
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

	public Sink<Statement> statementSink()
	{
		return stSink;
	}

	public Sink<Namespace> namespaceSink()
	{
		return nsSink;
	}

	public Sink<String> commentSink()
	{
		return cmtSink;
	}
}

// kate: tab-width 4
