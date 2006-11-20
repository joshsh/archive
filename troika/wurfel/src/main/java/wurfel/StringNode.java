package wurfel;


public class StringNode extends ValueNode
{
    public StringNode( final String value )
    {
        super( value );
    }

    public String toString()
    {
        return "\"" + value + "\"";
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
