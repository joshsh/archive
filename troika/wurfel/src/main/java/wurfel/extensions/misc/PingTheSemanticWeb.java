package wurfel.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.net.URL;
import java.net.URLEncoder;
import java.net.URLConnection;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

import java.io.InputStream;

public class PingTheSemanticWeb extends PrimitiveFunction
{
    private static final URI
        s_uri = Wurfel.createUri( "urn:net.dnsdojo.troika.wurfel-misc#pingTheSemanticWeb" );

    private static SAXBuilder s_saxBuilder = null;

    public PingTheSemanticWeb( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        String type;
        int maxResults;

        Iterator<Value> argIter = args.iterator();
        type = context.stringValue(
                context.castToLiteral( argIter.next() ) );
        maxResults = context.intValue(
                context.castToLiteral( argIter.next() ) );

        Document doc;

        try
        {
            if ( null == s_saxBuilder )
            {
                s_saxBuilder = new SAXBuilder( true );
                s_saxBuilder.setReuseParser( true );

                String schemaLocation = Wurfel.class.getResource( "xml/ptsw.xsd" ).toString();
                s_saxBuilder.setFeature(
                    "http://apache.org/xml/features/validation/schema", true );
                s_saxBuilder.setProperty( "http://apache.org/xml/properties/schema/"
                    + "external-noNamespaceSchemaLocation", schemaLocation );
            }

            URL url = new URL( "http://pingthesemanticweb.com/export/?serialization=xml&ns=&domain=&timeframe=any_time&type=" + type + "&nbresults=" + maxResults );
            URLConnection connection = url.openConnection();
            connection.connect();

            InputStream response = connection.getInputStream();

            synchronized( s_saxBuilder )
            {
                doc = s_saxBuilder.build( response );
            }

            response.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        NodeSet result = new NodeSet();
        Element root = doc.getRootElement();
        Iterator<Element> childIter = root.getChildren().iterator();
        while ( childIter.hasNext() )
        {
            Element child = childIter.next();
            String s = child.getAttributeValue( "url" );
            result.add( context.createLiteral( s ) );
        }

        return result;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
