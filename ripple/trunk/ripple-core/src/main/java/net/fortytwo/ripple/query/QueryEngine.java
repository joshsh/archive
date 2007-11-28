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

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.LexiconUpdater;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class QueryEngine
{
	private static final Logger LOGGER
		= Logger.getLogger( QueryEngine.class );

	private Model model;
	private Lexicon lexicon;
	private Evaluator evaluator;

	private RipplePrintStream printStream;
	private PrintStream errorPrintStream;

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
		return model.getConnection( name, new LexiconUpdater( lexicon ) );
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
		{
			throw new RippleException( "no default namespace is defined.  Use '@prefix : <...>.'\n" );
		}

		return defaultNs;
	}

	public void uriForKeyword( final String localName, final Sink<RippleValue> sink, final ModelConnection mc )
		throws RippleException
	{
		Collection<URI> options = lexicon.uriForKeyword( localName );

		if ( 0 == options.size() )
		{
			errorPrintStream.println( "Warning: keyword " + localName + " is undefined\n" );
		}

		else if ( 1 < options.size() )
		{
			errorPrintStream.println( "Warning: keyword " + localName + " is ambiguous\n" );
		}

		for ( Iterator<URI> optIter = options.iterator(); optIter.hasNext(); )
		{
			sink.put( mc.value( optIter.next() ) );
		}
	}

	public void uriForQName( final String nsPrefix,
								final String localName,
								final Sink<RippleValue> sink,
								final ModelConnection mc )
		throws RippleException
	{
		URI v = lexicon.uriForQName( nsPrefix, localName, mc );

		if ( null == v )
		{
			errorPrintStream.println( "Warning: " + nsPrefix + ":" + localName + " is undefined\n" );
		}

		else
		{
			sink.put( mc.value( v ) );
		}
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

	private void initializeLexicon() throws RippleException
	{
		LexiconUpdater updater = new LexiconUpdater( lexicon );

		ModelConnection mc = getConnection();
		mc.getStatements( null, null, null, updater.adderSink().statementSink() );
		mc.getNamespaces( updater.adderSink().namespaceSink() );
		mc.close();
	}
}

// kate: tab-width 4
