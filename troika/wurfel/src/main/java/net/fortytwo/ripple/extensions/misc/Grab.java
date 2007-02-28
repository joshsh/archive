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
import org.openrdf.model.Statement;
import org.openrdf.repository.RepositoryResult;

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

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws WurfelException
    {
        String urlStr;

        urlStr = mc.stringValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

        try
        {
            URL url = new URL( urlStr );

            URI baseUri = mc.createUri( urlStr );
            mc.addGraph( url, baseUri );

            RepositoryResult<Statement> stmtIter
                = mc.getRepositoryConnection().getStatements(
                    null, null, null, /*baseUri,*/ Wurfel.useInference() );
            while ( stmtIter.hasNext() )
                sink.put( stack.push( stmtIter.next().getSubject() ) );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
