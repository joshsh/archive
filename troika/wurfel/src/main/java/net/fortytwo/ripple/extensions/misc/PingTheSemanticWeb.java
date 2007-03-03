package net.fortytwo.ripple.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

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
    private static SAXBuilder s_saxBuilder = null;

    public PingTheSemanticWeb( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelMiscUri( "pingTheSemanticWeb" ), mc );
    }

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws WurfelException
    {
        String type;
        int maxResults;

        type = mc.stringValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();
        maxResults = mc.intValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

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

        Element root = doc.getRootElement();
        Iterator<Element> childIter = root.getChildren().iterator();
        while ( childIter.hasNext() )
        {
            Element child = childIter.next();
            String s = child.getAttributeValue( "url" );
            sink.put( new ListNode<Value>( mc.createLiteral( s ), stack ) );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
