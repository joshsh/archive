package net.fortytwo.restpipe;

import org.restlet.resource.Resource;
import org.restlet.Context;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;
import net.fortytwo.ripple.RippleException;

import java.util.Map;
import java.util.HashMap;

/**
 * Author: josh
 * Date: Apr 16, 2008
 * Time: 10:52:16 AM
 */
public class RPPResource extends Resource
{
    protected org.openrdf.model.URI selfUri;

    // TODO: re-use a shared ValueFactory
    protected ValueFactory valueFactory = new ValueFactoryImpl();

    public RPPResource( final Context context,
			final Request request,
            final Response response ) throws RippleException
	{
        super( context, request, response );

        selfUri = valueFactory.createURI(
      	        request.getResourceRef().toString() );
    }

    protected String getParameter( final String key ) throws RippleException
    {
        return getParameters().get( key );
    }

    protected String getParameter( final String key, final String defaultValue ) throws RippleException
    {
        String value = getParameter( key );

        return ( null == value )
                ? defaultValue
                : value;
    }

    protected Integer getIntParameter( final String key ) throws RippleException
    {
        String value = getParameter( key );

        try {
            return ( null == value )
                    ? null
                    : new Integer( value );
        } catch ( NumberFormatException e ) {
            throw new RippleException( e );
        }
    }

    protected Integer getIntParameter( final String key, final int defaultValue ) throws RippleException
    {
        String value = getParameter( key );

        try {
            return ( null == value )
                    ? defaultValue
                    : new Integer( value );
        } catch ( NumberFormatException e ) {
            throw new RippleException( e );
        }
    }


    private Map<String, String> parameters;

    // TODO: it seems like Restlet should make this unnecessary
    private Map<String, String> getParameters()
    {
        if ( null == parameters )
        {
            parameters = new HashMap<String, String>();
            String uri = selfUri.toString();

            if ( uri.contains( "?" ) )
            {
                String query = uri.substring( uri.indexOf( "?" ) + 1 );
                String[] pairs = query.split( "&" );
                for ( int i = 0; i < pairs.length; i++ )
                {
                    String[] pair = pairs[i].split( "=" );
                    String key = pair[0];
                    String value = pair[1];
                    parameters.put( key, value );
                }
            }
        }

        return parameters;
    }
}
