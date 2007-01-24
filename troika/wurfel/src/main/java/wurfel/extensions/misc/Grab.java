package wurfel.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

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
    public Grab( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createUri( "urn:net.dnsdojo.troika.wurfel-misc#grab" ), evalContext );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        Context context = evalContext.getContext();

        String urlStr;

        Iterator<Value> argIter = args.iterator();
        urlStr = evalContext.stringValue(
                evalContext.castToLiteral( argIter.next() ) );

        try
        {
            URL url = new URL( urlStr );

            URI baseUri = evalContext.createRandomUri();
            context.importModel( url, baseUri, evalContext );
            NodeSet results = new NodeSet();

            CloseableIterator<? extends Statement> stmtIter
                = evalContext.getConnection().getStatements(
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
