package wurfel.model;

import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Value;
import org.openrdf.model.impl.URIImpl;
import org.openrdf.sesame.sail.StatementIterator;

import java.util.Collection;


// TODO: subclass LiteralImpl instead of Literal
public class Apply implements Literal
{
    private static final URI s_applyClassUri
        = new URIImpl( "urn:net.dnsdojo.troika.wurfel#Apply" );

    private Collection<Value> function, argument;

    public Apply( final Collection<Value> function, final Collection<Value> argument )
    {
        this.function = function;
        this.argument = argument;
    }


    /**
     *  Compares this object with the specified object for order. Returns a negative integer, zero, or a positive integer as this object is less than, equal to, or greater than the specified object.
     */
    public int compareTo( Object other )
    {
// TODO
        return -1;
    }

    /**
     *  Gets all statements from the RDF graph for which this value is the object.
     */
    public StatementIterator getObjectStatements()
    {
        return null;
    }

    /**
     *  Compares a literal object to another object.
     */
    public boolean equals( Object other )
    {
// TODO
        return false;
    }

    /**
     * Gets the datatype for this literal.
     */
    public URI getDatatype()
    {
        return s_applyClassUri;
    }

    /**
     *  Gets the label of this literal.
     */
    public String getLabel()
    {
// TODO
        return "Error: the value of an Apply is in its getFunction() and getArgument() members";
    }

    /**
     *  Gets the language tag for this literal, normalized to lowercase.
     */
    public String getLanguage()
    {
// TODO
        return "I don't know what to use for a language tag";
    }

    /**
     *  The hash code of a literal is defined as the hash code of its label: label.hashCode()
     */
    public int hashCode()
    {
        return getLabel().hashCode();
    }



    public Collection<Value> getFunction()
    {
        return function;
    }

    public Collection<Value> getArgument()
    {
        return argument;
    }

/*
    public String toString()
    {
        return "@" + Node.toString( argument ) + " " + Node.toString( function );
    }
*/
/*
    Collection<Node> reduce( ReductionContext context )
    {
        
    }
*/
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
