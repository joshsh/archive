package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.HttpUtils;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;
import org.restlet.data.MediaType;
import org.restlet.resource.Representation;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.channels.ReadableByteChannel;
import java.nio.channels.WritableByteChannel;

/**
 * Author: josh
 * Date: Jan 16, 2008
 * Time: 3:46:20 PM
 */
public class HttpRepresentation extends Representation
{
    private InputStream inputStream = null;
    private HttpMethod method;

    // Note: the URI is immediately dereferenced
    public HttpRepresentation( final String uri ) throws RippleException
    {
        method = HttpUtils.createRdfGetMethod( uri );
        HttpUtils.registerMethod( method );

		HttpClient client = HttpUtils.createClient();

		try
		{
			client.executeMethod( method );
		}
		
		catch ( IOException e )
		{
			throw new RippleException( e );
		}

		InputStream is = null;

		try
		{
			is = method.getResponseBodyAsStream();
		}

		catch ( IOException e )
		{
			throw new RippleException( e );
		}

		inputStream = new HttpRepresentationInputStream( is );

		String mtStr = method.getResponseHeader( HttpUtils.CONTENT_TYPE ).getValue().split( ";" )[0];
		MediaType mt = new MediaType( mtStr );
//System.out.println( "discovered media type is: " + mt );
		setMediaType( mt );
    }

    public ReadableByteChannel getChannel() throws IOException
    {
        return null;  //To change body of implemented methods use File | Settings | File Templates.
    }

    public InputStream getStream() throws IOException
    {
        return inputStream;
    }

    public void write( final OutputStream outputStream ) throws IOException
    {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    public void write( final WritableByteChannel writableByteChannel ) throws IOException
    {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    /**
     * A helper InputStream which closes the HTTP connection when it itself is
	 * closed
     */
    private class HttpRepresentationInputStream extends InputStream
    {
        private InputStream innerInputStream;

        public HttpRepresentationInputStream( final InputStream is )
        {
            innerInputStream = is;
        }

        public int read() throws IOException
        {
            return innerInputStream.read();
        }
        
        @Override
        public void close() throws IOException
        {
            method.releaseConnection();

            innerInputStream.close();
        }
    }
}
