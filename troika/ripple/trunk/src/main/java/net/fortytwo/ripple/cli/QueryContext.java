package net.fortytwo.ripple.cli;

import java.io.PrintStream;

import java.util.Collection;
import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class QueryContext
{
	private final static Logger s_logger
		= Logger.getLogger( QueryContext.class );

	private Model model;
	private Lexicon lexicon;

	private RipplePrintStream printStream;
	private PrintStream errorPrintStream;

	////////////////////////////////////////////////////////////////////////////

	public QueryContext( final Model model,
						final PrintStream out,
						final PrintStream err )
		throws RippleException
	{
		this.model = model;
		lexicon = new Lexicon( model );
		printStream = new RipplePrintStream( out, lexicon );
		errorPrintStream = err;
	}

	////////////////////////////////////////////////////////////////////////////

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

	public void resolveKeyword( final String localName, Sink<RippleValue> sink )
		throws RippleException
	{
		Collection<URI> options = lexicon.resolveKeyword( localName );
		if ( 0 == options.size() )
			errorPrintStream.println( "Warning: no values resolved for keyword " + localName );
		else if ( 1 < options.size() )
			errorPrintStream.println( "Warning: multiple values resolved for keyword " + localName );

		for ( Iterator<URI> optIter = options.iterator(); optIter.hasNext(); )
			sink.put( model.getBridge().get(
				new RdfValue( optIter.next() ) ) );
	}

	public void resolveQName( final String nsPrefix,
								final String localName,
								final Sink<RippleValue> sink )
		throws RippleException
	{
		Value v = lexicon.resolveQName( nsPrefix, localName );

		if ( null == v )
			errorPrintStream.println( "Warning: no values resolved for " + nsPrefix + ":" + localName );

		else
			sink.put( model.getBridge().get(
				new RdfValue( v ) ) );
	}

	////////////////////////////////////////////////////////////////////////////


}

// kate: tab-width 4
