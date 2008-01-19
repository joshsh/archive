package net.fortytwo.ripple.rdf;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.RDFHandlerException;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;
import org.openrdf.sail.Sail;

public class SailInserter implements RDFHandler
{
	private Sail sail;
	private SailConnection sailConnection;

	public SailInserter( final Sail sail )
	{
		this.sail = sail;
		sailConnection = null;
	}

	public void startRDF() throws RDFHandlerException
	{
		try
		{
			sailConnection = sail.getConnection();
		}

		catch ( SailException e )
		{
			throw new RDFHandlerException( e );
		}
	}

	public void endRDF() throws RDFHandlerException
	{
		try
		{
			sailConnection.commit();
			sailConnection.close();
		}

		catch ( SailException e )
		{
			throw new RDFHandlerException( e );
		}
	}

	public void handleNamespace( final String prefix, final String uri ) throws RDFHandlerException
	{
		try
		{
			sailConnection.setNamespace( prefix, uri );
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
			sailConnection.addStatement( st.getSubject(), st.getPredicate(), st.getObject(), st.getContext() );
		}

		catch ( SailException e )
		{
			throw new RDFHandlerException( e );
		}
	}

	public void handleComment( final String comment ) throws RDFHandlerException
	{
		// Do nothing.
	}
}
