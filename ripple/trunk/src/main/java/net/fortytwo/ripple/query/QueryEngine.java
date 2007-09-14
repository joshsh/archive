/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import java.io.PrintStream;

import java.util.Collection;
import java.util.Iterator;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.LexiconUpdater;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.io.RdfNullSink;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.io.SesameAdapter;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class QueryEngine
{
	final static Logger logger
		= Logger.getLogger( QueryEngine.class );

	Model model;
	Lexicon lexicon;
	Evaluator evaluator;

	RipplePrintStream printStream;
	PrintStream errorPrintStream;

	////////////////////////////////////////////////////////////////////////////

	public QueryEngine( final Model model,
						final Evaluator evaluator,
						final PrintStream out,
						final PrintStream err )
		throws RippleException
	{
		this.model = model;
		this.evaluator = evaluator;
		lexicon = new Lexicon( model );
		printStream = new RipplePrintStream( out, lexicon );
		errorPrintStream = err;

		initializeLexicon();
	}

	////////////////////////////////////////////////////////////////////////////

	public ModelConnection getConnection()
		throws RippleException
	{
		return getConnection( null );
	}

	public ModelConnection getConnection( final String name )
		throws RippleException
	{
		final ModelConnection mc = ( null == name )
			? new ModelConnection( model )
			: new ModelConnection( model, name );

		mc.setRdfSink( new LexiconUpdater( lexicon, new RdfNullSink() ) );

		return mc;
	}

	public Evaluator getEvaluator()
	{
		return evaluator;
	}

	public Lexicon getLexicon()
	{
		return lexicon;
	}

	public Model getModel()
	{
		return model;
	}

	public RipplePrintStream getPrintStream()
	{
		return printStream;
	}

	public PrintStream getErrorPrintStream()
	{
		return errorPrintStream;
	}

	////////////////////////////////////////////////////////////////////////////

	public String getDefaultNamespace()
		throws RippleException
	{
		String defaultNs = lexicon.resolveNamespacePrefix( "" );

		if ( null == defaultNs )
			throw new RippleException( "no default namespace is defined.  Use '@prefix : <...>.'" );

		return defaultNs;
	}

	public void uriForKeyword( final String localName, Sink<RippleValue> sink )
		throws RippleException
	{
		Collection<URI> options = lexicon.uriForKeyword( localName );
		if ( 0 == options.size() )
			errorPrintStream.println( "Warning: no values resolved for keyword " + localName );
		else if ( 1 < options.size() )
			errorPrintStream.println( "Warning: multiple values resolved for keyword " + localName );

		for ( Iterator<URI> optIter = options.iterator(); optIter.hasNext(); )
			sink.put( model.getBridge().get(
				new RdfValue( optIter.next() ) ) );
	}

	public void uriForQName( final String nsPrefix,
								final String localName,
								final Sink<RippleValue> sink )
		throws RippleException
	{
		Value v = lexicon.uriForQName( nsPrefix, localName );

		if ( null == v )
			errorPrintStream.println( "Warning: no values resolved for " + nsPrefix + ":" + localName );

		else
			sink.put( model.getBridge().get(
				new RdfValue( v ) ) );
	}

	////////////////////////////////////////////////////////////////////////////

	public void executeCommand( final Command cmd ) throws RippleException
	{
		ModelConnection mc = getConnection( "for executeCommand" );
		RippleException ex = null;

		try
		{
			cmd.execute( this, mc );
		}

		catch ( RippleException e )
		{
			ex = e;
		}

		mc.close();

		if ( null != ex )
		{
			throw ex;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void initializeLexicon() throws RippleException
	{
		LexiconUpdater updater = new LexiconUpdater( lexicon, new RdfNullSink() );

		model.readAll( updater );
	}
}

// kate: tab-width 4
