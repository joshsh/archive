package wurfel.model;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;
import org.openrdf.model.URI;

import java.util.Collection;
import java.util.LinkedList;

public interface Function extends Value
{
//    public abstract String toString();
    public URI getUri();

    /**
     *  The fixed number of arguments which this function consumes before
     *  yielding a result.
     */
    public int arity();

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      Context context )
        throws WurfelException;

    public void checkArguments( LinkedList<Value> args )
        throws WurfelException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
