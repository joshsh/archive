package net.fortytwo.ripple.model;

//import net.fortytwo.ripple.RippleException;
import wurfel.WurfelException;

public interface Sink<T>
{
    public void put( T t ) throws WurfelException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
