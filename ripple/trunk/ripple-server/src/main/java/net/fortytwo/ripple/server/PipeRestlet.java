package net.fortytwo.ripple.server;

import java.util.regex.Pattern;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.impl.sesame.SesameModel;
import net.fortytwo.ripple.query.Evaluator;
import net.fortytwo.ripple.query.LazyEvaluator;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;
import net.fortytwo.ripple.util.UrlFactory;

import org.restlet.Restlet;
import org.restlet.data.MediaType;
import org.restlet.data.Reference;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.data.Status;

// FIXME: an actual "pipe" would receive POST requests, process them, then POST
//        the results.  This class processes GET requests as remote queries,
//        which could be useful in its own way.
public class PipeRestlet extends Restlet
{
	private Pattern uriPattern;
	private Model model;
	private Evaluator evaluator;

	public PipeRestlet() throws RippleException
	{
// TODO: a more strict expression might be in order
		try
		{
			uriPattern = Pattern.compile( "http://[^?]+/pipe\\?filter=[^&]+&sink=[^&]+" );
		}

		catch ( java.util.regex.PatternSyntaxException e )
		{
			throw new RippleException( e );
		}

		model = new SesameModel( RdfUtils.createMemoryStoreSail(), new UrlFactory() );
		evaluator = new LazyEvaluator();
	}

	@Override
	public void handle( final Request request, final Response response )
	{
		Reference resourceRef = request.getResourceRef();
		String uri = resourceRef.toString();

		try
		{
final StringBuilder message = new StringBuilder();
			if ( !uriPattern.matcher( uri ).matches() )
			{
				response.setStatus( Status.CLIENT_ERROR_NOT_FOUND );
				response.setEntity( "", MediaType.TEXT_PLAIN );
				return;
			}

			int i0 = uri.indexOf( "?filter=" );
			int i1 = uri.indexOf( "&sink=" );

			String filterUri = StringUtils.percentDecode(
				uri.substring( i0 + 8, i1 ) );
			String sinkUri = StringUtils.percentDecode(
				uri.substring( i1 + 6 ) );
message.append( "Filter URI: " + filterUri + "\n" );
message.append( "Sink URI: " + sinkUri + "\n" );

			Collector<RippleList> outStacks = new Collector<RippleList>();
			ModelConnection mc = model.getConnection( "for PipeRestlet.handle()" );
			RippleValue filterValue = model.getBridge().get(
				new RdfValue( mc.createUri( filterUri ) ) );
			RippleList inStack = new RippleList( filterValue ).push( Operator.OP );
//			RippleList inStack = new RippleList( Operator.OP ).push( new RdfValue( mc.createUri( filterUri ) ) );
message.append( "inStack = " + inStack + "\n" );
			evaluator.applyTo( inStack, outStacks, mc );
			mc.close();

			final PostSink postSink = new PostSink( sinkUri );

			Sink<RippleList> resultSink = new Sink<RippleList>()
			{
				public void put( final RippleList l ) throws RippleException
				{
message.append( "    result: " + l + "\n" );
					postSink.put( l.toString() );
				}
			};

			outStacks.writeTo( resultSink );



response.setEntity( message.toString(), MediaType.TEXT_PLAIN );
		}

		catch ( Throwable t )
		{
			// Note: revealing the error message to the client might be a bad
			//       idea.
			response.setStatus( Status.CONNECTOR_ERROR_INTERNAL, t.toString() );
			response.setEntity( "", MediaType.TEXT_PLAIN );
		}
	}
}

// kate: tab-width 4
