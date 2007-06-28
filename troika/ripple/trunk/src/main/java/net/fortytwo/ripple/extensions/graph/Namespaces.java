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

public class Namespaces extends PrimitiveFunction
{
	public Namespaces( RdfValue v, ModelConnection mc )
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

		RdfSourceAdapter adapter = createAdapter( stack, sink, mc );

		URLConnection urlConn = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( urlConn );
		HttpUtils.connect( urlConn );

		RdfUtils.read( urlConn, adapter, uri.toString() );
	}

	static RdfSourceAdapter createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink,
										final ModelConnection mc )
	{
		// Discard statements.
		Sink<Statement> statementSink = new NullSink<Statement>();

		// Push namespaces to the stack as pairs.
		Sink<Namespace> namespaceSink = new Sink<Namespace>()
		{
			public void put( Namespace ns )
				throws RippleException
			{
				RippleList result = new RippleList(
					mc.createValue( ns.getPrefix() ) ).push( mc.createValue( ns.getName() ) );
				resultSink.put( new RippleList( result, stack ) );
			}
		};

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
