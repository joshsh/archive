package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

import java.util.Collection;

/**
 * Author: josh
 * Date: Mar 11, 2008
 * Time: 8:45:29 AM
 */
public class AddToCollectionSink<T> implements Sink<T>
{
    private Collection<T> collection;

    public AddToCollectionSink(final Collection<T> coll)
    {
        this.collection = coll;
    }

    public void put(final T t) throws RippleException
    {
        this.collection.add(t);
    }
}
