package wurfel.model.primitives;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;

import org.openrdf.model.Value;
import org.openrdf.model.URI;

import java.util.Collection;
import java.util.ArrayList;

public class ConcatenateStringsPrimitive extends PrimitiveFunction
{
    private static final URI s_wurfeltestConcatenateStringsUri
        = Wurfel.getWurfelTestUri( "concatenateStrings" );

    public ConcatenateStringsPrimitive( Context context )
        throws WurfelException
    {
        super( s_wurfeltestConcatenateStringsUri, context );
    }

    public URI getUri()
    {
        return s_wurfeltestConcatenateStringsUri;
    }

    public Collection<Value> apply( Value args[], Context context )
        throws WurfelException
    {
return new ArrayList<Value>();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
