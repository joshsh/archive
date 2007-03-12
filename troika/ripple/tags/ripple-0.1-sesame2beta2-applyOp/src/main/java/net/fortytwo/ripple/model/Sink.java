package net.fortytwo.ripple.model;

//import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.RippleException;

public interface Sink<T>
{
    public void put( T t ) throws RippleException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
