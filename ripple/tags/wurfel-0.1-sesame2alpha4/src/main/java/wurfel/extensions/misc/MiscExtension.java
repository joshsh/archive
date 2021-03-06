package wurfel.extensions.misc;

import wurfel.Extension;
import wurfel.WurfelException;
import wurfel.model.Function;
import wurfel.model.ModelConnection;

import java.net.URL;

public class MiscExtension extends Extension
{
    protected Function[] getFunctions( ModelConnection mc )
        throws WurfelException
    {
        Function[] functions = {
            new Sha1SumOf( mc ),
            new UriToString( mc ),
            new UrlEncoding( mc ),
            new UrlTarget( mc ),
            new SwoogleIt( mc ),
            new Grab( mc ),
            new PingTheSemanticWeb( mc ) };

        return functions;
    }

    protected URL[] getResources()
    {
/*
        URL[] urls = {
            this.getClass().getResource( "wurfel-misc.rdf" ) };

        return urls;
*/
        return null;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
