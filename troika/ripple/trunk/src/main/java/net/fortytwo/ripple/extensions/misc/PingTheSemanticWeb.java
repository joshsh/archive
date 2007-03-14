package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleStack;
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
        throws RippleException
    {
        super( mc.createRippleMiscUri( "pingTheSemanticWeb" ), mc );
    }

    protected void applyInternal( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
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

                String schemaLocation = Ripple.class.getResource( "xml/ptsw.xsd" ).toString();
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
            throw new RippleException( t );
        }

        Element root = doc.getRootElement();
        Iterator<Element> childIter = root.getChildren().iterator();
        while ( childIter.hasNext() )
        {
            Element child = childIter.next();
            String s = child.getAttributeValue( "url" );
            sink.put( new RippleStack( mc.createLiteral( s ), stack ) );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
