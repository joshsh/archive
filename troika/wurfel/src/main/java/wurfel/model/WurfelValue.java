package wurfel.model;

import wurfel.WurfelException;

import org.openrdf.model.Value;

/**
 *  A marker interface indicating a value with custom behavior.
 */
public interface WurfelValue extends Value
{
    public void printTo( WurfelPrintStream p )
        throws WurfelException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
