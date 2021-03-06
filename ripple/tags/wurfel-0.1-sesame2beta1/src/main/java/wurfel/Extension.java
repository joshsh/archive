package wurfel;

import wurfel.model.ModelConnection;
import wurfel.model.Function;

import java.net.URL;

public abstract class Extension
{
    protected abstract URL[] getResources();

    protected abstract Function[] getFunctions( ModelConnection mc ) throws WurfelException;

    public void load( ModelConnection mc )
        throws WurfelException
    {
        URL[] resources = getResources();

        if ( null != resources )
            for ( URL url : getResources() )
                mc.addGraph( url );

        for ( Function function : getFunctions( mc ) )
            mc.getModel().addSpecialFunction( function );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
