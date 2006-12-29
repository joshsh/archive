package wurfel;

import wurfel.model.Function;

import java.net.URL;

public abstract class Extension
{
    protected Context context;

    protected Extension( Context context )
    {
        this.context = context;
    }

    protected abstract URL[] getResources();

    protected abstract Function[] getFunctions() throws WurfelException;

    public void load()
        throws WurfelException
    {
        for ( URL url : getResources() )
            context.importModel( url, Wurfel.createRandomUri() );

        for ( Function function : getFunctions() )
            context.addSpecialFunction( function );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
