package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
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
        throws RippleException
    {
        Function[] functions = {
            new IntegerGreaterThan( mc ),
            new IntegerLessThan( mc ),
            new True( mc ),
            new False( mc ),
            new Or( mc ),
            new Dup( mc ),
            new Zap( mc ),
            new Scrap( mc ),
            new Unique( mc ),
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
