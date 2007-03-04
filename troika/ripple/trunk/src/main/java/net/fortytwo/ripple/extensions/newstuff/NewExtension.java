package net.fortytwo.ripple.extensions.newstuff;

import wurfel.Extension;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import java.net.URL;

public class NewExtension extends Extension
{
    private static String baseUri = "http://fortytwo.net/2007/03/04/rpl-new#";
    public static String getBaseUri()
    {
        return baseUri;
    }

    protected Function[] getFunctions( ModelConnection mc )
        throws WurfelException
    {
        Function[] functions = {
            new Or( mc ),
            new Dup( mc ),
            new Zap( mc ),
            new Swap( mc ) };

        return functions;
    }

    protected URL[] getResources()
    {
/*
        URL[] urls = {
            this.getClass().getResource( "wurfel-test.rdf" ) };

        return urls;
*/
        return null;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
