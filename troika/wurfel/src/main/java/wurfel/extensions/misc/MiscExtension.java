package wurfel.extensions.misc;

import wurfel.Extension;
import wurfel.WurfelException;
import wurfel.model.Function;
import wurfel.model.EvaluationContext;

import java.net.URL;

public class MiscExtension extends Extension
{
    protected Function[] getFunctions( EvaluationContext evalContext )
        throws WurfelException
    {
        Function[] functions = {
            new Sha1SumOf( evalContext ),
            new UriToString( evalContext ),
            new UrlEncoding( evalContext ),
            new UrlTarget( evalContext ),
            new SwoogleIt( evalContext ),
            new Grab( evalContext ),
            new PingTheSemanticWeb( evalContext ) };

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
