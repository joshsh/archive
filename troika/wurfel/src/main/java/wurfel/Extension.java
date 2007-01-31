package wurfel;

import wurfel.model.EvaluationContext;
import wurfel.model.Function;

import java.net.URL;

public abstract class Extension
{
    protected abstract URL[] getResources();

    protected abstract Function[] getFunctions( EvaluationContext evalContext ) throws WurfelException;

    public void load( EvaluationContext evalContext )
        throws WurfelException
    {
        URL[] resources = getResources();

        if ( null != resources )
            for ( URL url : getResources() )
                evalContext.addGraph( url );

        for ( Function function : getFunctions( evalContext ) )
            evalContext.getContext().addSpecialFunction( function );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
