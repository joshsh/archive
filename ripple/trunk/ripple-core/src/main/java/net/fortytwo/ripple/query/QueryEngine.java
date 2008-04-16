/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import java.io.PrintStream;
import java.util.Collection;
import java.util.Iterator;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.LexiconUpdater;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;

import org.apache.log4j.Logger;
import org.openrdf.model.URI;

public class QueryEngine
{
	private static final Logger LOGGER = Logger.getLogger( QueryEngine.class );

	private Model model;
	private Lexicon lexicon;
	private StackEvaluator evaluator;

	private RipplePrintStream printStream;
	private PrintStream errorPrintStream;

	////////////////////////////////////////////////////////////////////////////

	public QueryEngine( final Model model,
						final StackEvaluator evaluator,
						final PrintStream out,
						final PrintStream err )
		throws RippleException
	{
		this.model = model;
		this.evaluator = evaluator;
		lexicon = new Lexicon( model );
		printStream = new RipplePrintStream( out, lexicon );
		errorPrintStream = err;

//System.out.println("--- q");
		initializeLexicon();
//System.out.println("--- w");

        String defaultNamespace = Ripple.getProperties().getString(
                Ripple.DEFAULT_NAMESPACE );

        // Set the default namespace.
		ModelConnection mc = getConnection( "Demo connection" );

        try {
//System.out.println("--- w2");
//System.out.println("--- Ripple.defaultNamespace() = " + Ripple.defaultNamespace());
            mc.setNamespace( "", defaultNamespace, false );
//System.out.println("--- w3");
            mc.commit();
//System.out.println("--- w4");
        } finally {
            mc.close();
        }
//System.out.println("--- e");
		
		getLexicon().add( new org.openrdf.model.impl.NamespaceImpl( "", defaultNamespace ) );
//System.out.println("--- r");
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

	public StackEvaluator getEvaluator()
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

	public void uriForKeyword( final String localName, final Sink<RippleValue, RippleException> sink, final ModelConnection mc )
		throws RippleException
	{
		Collection<URI> options = lexicon.uriForKeyword( localName );

		if ( 0 == options.size() )
		{
			errorPrintStream.println( "Warning: keyword '" + localName + "' is undefined\n" );
		}

		else if ( 1 < options.size() )
		{
			errorPrintStream.println( "Warning: keyword '" + localName + "' is ambiguous\n" );
		}

		for ( Iterator<URI> optIter = options.iterator(); optIter.hasNext(); )
		{
			sink.put( mc.value( optIter.next() ) );
		}
	}

	public void uriForQName( final String nsPrefix,
								final String localName,
								final Sink<RippleValue, RippleException> sink,
								final ModelConnection mc )
		throws RippleException
	{
		String ns = lexicon.resolveNamespacePrefix( nsPrefix );

		if ( null == ns )
		{
			errorPrintStream.println( "Warning: prefix '" + nsPrefix + "' does not identify a namespace\n" );
		}

		else
		{
			sink.put( mc.value( mc.createUri( ns + localName ) ) );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void executeCommand( final Command cmd ) throws RippleException
	{
		ModelConnection mc = getConnection( "for executeCommand" );

        try {
            cmd.execute( this, mc );
		} finally {
            mc.close();
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private void initializeLexicon() throws RippleException
	{
		LexiconUpdater updater = new LexiconUpdater( lexicon );

		ModelConnection mc = getConnection();

        try {
            if ( Ripple.getProperties().getBoolean( Ripple.PULL_ENTIRE_MODEL ) );
            {
                mc.getStatements( null, null, null, updater.adderSink().statementSink(), false );
            }

            mc.getNamespaces( updater.adderSink().namespaceSink() );
        } finally {
            mc.close();
        }
    }
}

// kate: tab-width 4
