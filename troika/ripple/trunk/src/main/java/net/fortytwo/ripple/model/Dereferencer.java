package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.URI;

public interface Dereferencer
{
public void dereference( URI uri, ModelConnection mc )
    throws RippleException;

    public void dereference( RdfValue rv, ModelConnection mc )
        throws RippleException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
