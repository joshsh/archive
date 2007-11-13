package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;
import info.aduna.iteration.CloseableIteration;

public class CloseableIterationSource<T, E extends Exception> implements Source<T>
{
	private CloseableIteration<T, E> it;
	
	public CloseableIterationSource( final CloseableIteration<T,E> iter )
	{
		it = iter;
	}
	
	public void writeTo( final Sink<T> sink) throws RippleException
	{
		if ( null == it )
		{
			return;
		}
		
		try
		{
			while ( it.hasNext() )
			{
				sink.put( it.next() );
			}
		}
		
		catch ( Throwable t )
		{
			close();
			
			if ( t instanceof RippleException )
			{
				throw (RippleException) t;
			}
			
			else
			{
				throw new RippleException( t );
			}
		}
		
		close();
	}
	
	private void close() throws RippleException
	{
		try
		{
			it.close();
		}
		
		catch ( Throwable t )
		{
			it = null;
			throw new RippleException( t );
		}
		
		it = null;
	}
}
