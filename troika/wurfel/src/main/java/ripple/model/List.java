package ripple.model;

import wurfel.model.Node;

public class List<T> extends Node
{
    private T first;
    private List<T> rest;

    public T getFirst()
    {
        return first;
    }

    public List<T> getRest()
    {
        return rest;
    }

    public List( final T first )
    {
        this.first = first;
        rest = null;
    }

    public List( final T first, final List<T> rest )
    {
        this.first = first;
        this.rest = rest;
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
