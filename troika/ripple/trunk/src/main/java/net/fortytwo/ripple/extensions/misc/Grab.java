package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.util.Sink;

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
        throws RippleException
    {
        super( mc.createRippleMiscUri( "grab" ), mc );
    }

    protected void applyInternal( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
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
                    null, null, null, /*baseUri,*/ Ripple.useInference() );
            while ( stmtIter.hasNext() )
                sink.put( new RippleStack( stmtIter.next().getSubject(), stack ) );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
