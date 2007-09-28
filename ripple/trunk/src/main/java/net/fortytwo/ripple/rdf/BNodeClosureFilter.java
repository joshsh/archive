package net.fortytwo.ripple.rdf;

import info.aduna.iteration.CloseableIteration;

import java.util.HashSet;
import java.util.Set;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

public class BNodeClosureFilter implements Sink<Resource>
{
	private Set<Resource> visited;
	private Sink<Statement> sink;
	private SailConnection sailConnection;
	private Buffer<Resource> buffer;

	public BNodeClosureFilter( final Sink<Statement> sink, final SailConnection sc )
	{
		this.sink = sink;
		sailConnection = sc;
		buffer = new Buffer<Resource>( this );
		visited = new HashSet<Resource>();
	}

	public void put( final Resource r )
		throws RippleException
	{
		if ( visited.contains( r ) )
		{
			return;
		}

		else
		{
			visited.add( r );
		}

		try
		{
			CloseableIteration<? extends Statement, SailException> stmtIter
				= sailConnection.getStatements(
					r, null, null, Ripple.useInference() );

			while ( stmtIter.hasNext() )
			{
				Statement st = stmtIter.next();

				sink.put( st );

				// Traverse to any neighboring blank nodes (but not to URIs).
				Value obj = st.getObject();
				if ( obj instanceof Resource
						&& !( obj instanceof URI ) )
				{
					buffer.put( (Resource) obj );
				}
			}

			stmtIter.close();
		}

		catch ( RippleException e )
		{
			throw e;
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		// Note: the buffer may be written to before it has been completely read.
		buffer.flush();
	}
}

// kate: tab-width 4
