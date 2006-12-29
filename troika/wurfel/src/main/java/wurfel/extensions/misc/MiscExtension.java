package wurfel.extensions.misc;

import wurfel.Context;
import wurfel.Extension;
import wurfel.WurfelException;
import wurfel.model.Function;

import java.net.URL;

public class MiscExtension extends Extension
{
    public MiscExtension( Context context )
    {
        super( context );
    }

    protected Function[] getFunctions()
        throws WurfelException
    {
        Function[] functions = {
            new Sha1SumOf( context ),
            new UriToString( context ),
            new UrlEncoding( context ),
            new UrlTarget( context ),
            new SwoogleIt( context ),
            new Grab( context ),
            new PingTheSemanticWeb( context ) };

        return functions;
    }

    protected URL[] getResources()
    {
        URL[] urls = {
            this.getClass().getResource( "wurfel-misc.rdf" ) };

        return urls;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
