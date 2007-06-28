package net.fortytwo.ripple.extensions.graph;

import java.net.URLConnection;

import java.io.InputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

public class Triples extends PrimitiveFunction
{
	public Triples( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		URI uri;

		uri = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

		RdfSourceAdapter adapter = createAdapter( stack, sink );

		URLConnection urlConn = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( urlConn );
		HttpUtils.connect( urlConn );

		RdfUtils.read( urlConn, adapter, uri.toString() );
	}

	static RdfSourceAdapter createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink )
	{
		// Push statements to the stack as triples.
		Sink<Statement> statementSink = new Sink<Statement>()
		{
			public void put( Statement st )
				throws RippleException
			{
// Note: don't bother with the model bridge for now.
				RippleValue subj = new RdfValue( st.getSubject() );
				RippleValue pred = new RdfValue( st.getPredicate() );
				RippleValue obj = new RdfValue( st.getObject() );

				RippleList triple = new RippleList( subj ).push( pred ).push( obj );
				resultSink.put(
					new RippleList( triple, stack ) );
			}
		};

		// Discard namespace definitions.
		Sink<Namespace> namespaceSink = new NullSink<Namespace>();

		// Discard comments.
		Sink<String> commentSink = new NullSink<String>();

		RdfSourceAdapter adapter = new RdfSourceAdapter(
			statementSink,
			namespaceSink,
			commentSink );

		return adapter;
	}
}

// kate: tab-width 4
