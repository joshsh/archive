package net.fortytwo.ripple.extensions.services;


import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.repository.RepositoryResult;
import org.openrdf.model.vocabulary.RDF;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.net.URL;
import java.net.URLEncoder;

public class SwoogleIt extends PrimitiveFunction
{
	URI swoogleQueryResponseUri = null;
	static String swoogleNs = "http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#";
	
	public SwoogleIt()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 3;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		if ( null == swoogleQueryResponseUri )
			swoogleQueryResponseUri = mc.createUri( swoogleNs + "QueryResponse" );

		String key, searchString;
		URI queryType;

		key = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		queryType = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();
		searchString = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		String urlStr;
		URL url;

		try
		{
			urlStr = "http://logos.cs.umbc.edu:8080/swoogle31/q"
				+ "?key=" + URLEncoder.encode( key )
				+ "&queryType=" + URLEncoder.encode( queryType.getLocalName() )
				+ "&searchString=" + URLEncoder.encode( searchString );

			url = new URL( urlStr );
		}

		catch ( java.net.MalformedURLException e )
		{
			throw new RippleException( e );
		}

		mc.addGraph( url, urlStr );
//System.out.println( "baseUri = " + urlStr );

		try
		{
			RepositoryResult<Statement> stmtIter
				= mc.getRepositoryConnection().getStatements(
					null, RDF.TYPE, swoogleQueryResponseUri, /*baseUri,*/ Ripple.useInference() );
			while ( stmtIter.hasNext() )
				sink.put( new RippleList( new RdfValue( stmtIter.next().getSubject() ), stack ) );
			stmtIter.close();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}
}

// kate: tab-width 4
