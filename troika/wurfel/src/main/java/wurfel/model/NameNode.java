package wurfel.model;


public class NameNode extends NullaryNode
{
    private String value;

    public NameNode( final String value )
    {
        this.value = value;
    }

    public String getValue()
    {
        return value;
    }

    public String toString()
    {
        return value;
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
