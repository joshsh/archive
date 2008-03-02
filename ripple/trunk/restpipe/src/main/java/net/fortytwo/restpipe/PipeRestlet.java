package net.fortytwo.restpipe;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.*;
import net.fortytwo.ripple.model.impl.sesame.SesameModel;
import net.fortytwo.ripple.query.Evaluator;
import net.fortytwo.ripple.query.LazyEvaluator;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;
import net.fortytwo.ripple.util.UriMap;

import org.restlet.Component;
import org.restlet.Restlet;
import org.restlet.Server;
import org.restlet.data.CharacterSet;
import org.restlet.data.ClientInfo;
import org.restlet.data.Encoding;
import org.restlet.data.Language;
import org.restlet.data.MediaType;
import org.restlet.data.Preference;
import org.restlet.data.Protocol;
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

        // TODO: use a shared map
        UriMap uriMap = new UriMap();

        model = new SesameModel( RdfUtils.createMemoryStoreSail(), uriMap );
		evaluator = new LazyEvaluator();
	}

	@Override
	public void handle( final Request request, final Response response )
	{
		try
		{
            Reference resourceRef = request.getResourceRef();
            String uri = resourceRef.toString();

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

			Collector<StackContext> outStacks = new Collector<StackContext>();
			ModelConnection mc = model.getConnection( "for PipeRestlet.handle()" );
			RippleValue filterValue = model.getBridge().get(
				new RdfValue( mc.createUri( filterUri ) ) );
			RippleList inStack = mc.list( filterValue ).push( Operator.OP );
            StackContext arg = new StackContext( inStack, mc );
//			RippleList inStack = new RippleList( Operator.OP ).push( new RdfValue( mc.createUri( filterUri ) ) );
message.append( "inStack = " + inStack + "\n" );
			evaluator.applyTo( arg, outStacks );
			mc.close();

			final PostSink postSink = new PostSink( sinkUri );

			Sink<StackContext> resultSink = new Sink<StackContext>()
			{
				public void put( final StackContext arg ) throws RippleException
				{
message.append( "    result: " + arg + "\n" );
					postSink.put( arg.toString() );
				}
			};

			outStacks.writeTo( resultSink );

response.setEntity( message.toString(), MediaType.TEXT_PLAIN );
		}

		catch ( Throwable t )
		{
            new RippleException(t).logError( false );
            
            // Note: revealing the error message to the client might be a bad
			//       idea.
			response.setStatus( Status.CONNECTOR_ERROR_INTERNAL, t.toString() );
			response.setEntity( "", MediaType.TEXT_PLAIN );
		}
	}
}

// kate: tab-width 4
