package wurfel.model;

import wurfel.WurfelException;

import org.openrdf.model.URI;

public interface Dereferencer
{
    public void dereference( URI uri, ModelConnection mc )
        throws WurfelException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
