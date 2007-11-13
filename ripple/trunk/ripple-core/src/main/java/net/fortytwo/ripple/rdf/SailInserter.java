package net.fortytwo.ripple.rdf;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.RDFHandlerException;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

public class SailInserter implements RDFHandler
{
	private SailConnection sc;
	
	public SailInserter( final SailConnection connection )
	{
		sc = connection;
	}
	
	public void endRDF() throws RDFHandlerException
	{
	}

	public void handleComment( final String comment ) throws RDFHandlerException
	{
	}

	public void handleNamespace( final String prefix, final String name )
			throws RDFHandlerException
	{
		try
		{
			sc.setNamespace( prefix, name );
		}
		
		catch ( SailException e )
		{
			throw new RDFHandlerException( e );
		}
	}

	public void handleStatement( final Statement st ) throws RDFHandlerException
	{
		try
		{
			Resource context = st.getContext();
			
			if ( null == context )
			{
				sc.addStatement( st.getSubject(), st.getPredicate(), st.getObject() );
			}
			
			else
			{
				sc.addStatement( st.getSubject(), st.getPredicate(), st.getObject(), context );
			}
		}
		
		catch ( SailException e )
		{
			throw new RDFHandlerException( e );
		}
	}

	public void startRDF() throws RDFHandlerException
	{
	}
}
