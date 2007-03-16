package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.util.Sink;

import java.net.URL;

public class NewExtension extends Extension
{
    private static String baseUri = "http://fortytwo.net/2007/03/04/rpl-new#";

    public void load( ModelConnection mc )
        throws RippleException
    {
        ModelBridge bridge = mc.getModel().getBridge();

        bridge.add( new IntegerGreaterThan( baseUri + "integer-greaterThan", mc );
        bridge.add( new IntegerLessThan(    baseUri + "integer-lessThan", mc );
        bridge.add( new Assert(             baseUri + "assert", mc );
        bridge.add( new Deny(               baseUri + "deny", mc );
        bridge.add( new Equal(              baseUri + "equal", mc );
        bridge.add( new True(               baseUri + "true", mc );
        bridge.add( new False(              baseUri + "false", mc );
        bridge.add( new Or(                 baseUri + "or", mc );
        bridge.add( new Dup(                baseUri + "dup", mc );
        bridge.add( new Zap(                baseUri + "zap", mc );
        bridge.add( new Scrap(              baseUri + "scrap", mc );
        bridge.add( new Limit(              baseUri + "limit", mc );
//        bridge.add( new Unique( baseUri + "unique", mc );
        bridge.add( new Swap(               baseUri + "swap", mc );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
