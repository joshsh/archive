/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.linkeddata.sail;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;

import org.apache.log4j.Logger;
import org.openrdf.model.Statement;
import org.openrdf.sail.SailConnectionListener;

public class SailConnectionListenerAdapter implements SailConnectionListener
{
	private static final Logger LOGGER
		= Logger.getLogger( SailConnectionListenerAdapter.class );
	
	private Sink<Statement, RippleException> addSink, subSink;
	
	public SailConnectionListenerAdapter( final RdfDiffSink diffSink )
	{
		addSink = diffSink.adderSink().statementSink();
		subSink = diffSink.subtractorSink().statementSink();
	}
	
	public void statementAdded( final Statement st )
	{
		try
		{
			addSink.put( st );
		}
		
		catch ( RippleException e )
		{
			LOGGER.warn( "Unhandled exception", e );
		}
	}

	public void statementRemoved( final Statement st )
	{
		try
		{
			subSink.put( st );
		}
		
		catch ( RippleException e )
		{
			LOGGER.warn( "Unhandled exception", e );
		}
	}

}
