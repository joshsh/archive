package net.fortytwo.ripple;

import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Function;

import java.net.URL;

public abstract class Extension
{
    public abstract void load( ModelConnection mc )
        throws RippleException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
