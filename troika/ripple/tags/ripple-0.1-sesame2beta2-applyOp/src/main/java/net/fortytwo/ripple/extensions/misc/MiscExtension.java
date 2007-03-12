package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;

import java.net.URL;

public class MiscExtension extends Extension
{
    protected Function[] getFunctions( ModelConnection mc )
        throws RippleException
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
