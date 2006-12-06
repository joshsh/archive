package wurfel.model;


public abstract class ValueNode<T> extends NullaryNode
{
    protected T value;

    public ValueNode( T value )
    {
        this.value = value;
    }

    public T getValue()
    {
        return value;
    }

    public abstract String toString();

    public String defaultToString()
    {
        return value.toString();
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
