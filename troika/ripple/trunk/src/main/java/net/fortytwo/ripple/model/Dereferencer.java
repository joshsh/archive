package net.fortytwo.ripple.model;

import java.util.Collection;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.URI;

public interface Dereferencer
{
public void dereference( URI uri, ModelConnection mc )
    throws RippleException;

    public void dereference( RdfValue rv, ModelConnection mc )
        throws RippleException;

    public void forget( RdfValue rv, ModelConnection mc )
        throws RippleException;

    public void addSuccessMemo( final String memo );
    public void addFailureMemo( final String memo );

    public Collection<String> getSuccessMemos();
    public Collection<String> getFailureMemos();
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
