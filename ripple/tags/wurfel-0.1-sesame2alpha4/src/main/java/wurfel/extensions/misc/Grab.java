package wurfel.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.model.PrimitiveFunction;
import wurfel.model.Container;
import wurfel.model.ModelConnection;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.util.iterator.CloseableIterator;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.net.URL;
import java.net.URLEncoder;

public class Grab extends PrimitiveFunction
{
    public Grab( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelMiscUri( "grab" ), mc );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        String urlStr;

        Iterator<Value> argIter = args.iterator();
        urlStr = mc.stringValue(
                mc.castToLiteral( argIter.next() ) );

        try
        {
            URL url = new URL( urlStr );

            URI baseUri = mc.createUri( urlStr );
            mc.addGraph( url, baseUri );
            Container results = new Container();

            CloseableIterator<? extends Statement> stmtIter
                = mc.getConnection().getStatements(
                    null, null, null, baseUri, Wurfel.useInference() );
            while ( stmtIter.hasNext() )
                results.add( stmtIter.next().getSubject() );
            stmtIter.close();

            return results;
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
