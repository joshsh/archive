package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;

import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;

import java.io.OutputStream;
import java.io.PrintStream;

import java.util.Iterator;

public class WurfelPrintStream extends PrintStream
{
    private static final URI
        s_xsdBooleanUri = Wurfel.getXmlSchemaUri( "boolean" ),
        s_xsdDoubleUri = Wurfel.getXmlSchemaUri( "double" ),
        s_xsdIntegerUri = Wurfel.getXmlSchemaUri( "integer" ),
        s_xsdStringUri = Wurfel.getXmlSchemaUri( "string" );

    private Model model;

    public WurfelPrintStream( OutputStream out, Model model )
    {
        super( out );
        this.model = model;
    }

    private static String literalEncodingOf( final String s )
    {
// TODO
return s;
    }

    public void print( Value v )
        throws WurfelException
    {
        if ( null == v )
            print( "()" );

        else if ( v instanceof WurfelValue )
            ( (WurfelValue) v ).printTo( this );

        else
        {
            if ( v instanceof URI )
            {
                URI uri = (URI) v;
                String prefix = model.nsPrefixOf( uri );

                if ( null == prefix )
                    print( uri.toString() );
                else
                    print( prefix + ":" + uri.getLocalName() );
            }

            else if ( v instanceof Literal )
            {
                URI dataTypeUri = ( (Literal) v ).getDatatype();

// FIXME: is this equals() safe?
                if ( null != dataTypeUri && dataTypeUri.equals( s_xsdStringUri ) )
                {
                    print( "\"" );
                    print( literalEncodingOf( v.toString() ) );
                    print( "\"" );
                }

                else
                    print( v.toString() );
            }

            else
                print( v.toString() );
        }
    }

    public void print( Statement st )
        throws WurfelException
    {
        print( "    <" );
        print( st.getSubject() );
        print( ">" );

        print( " <" );
        print( st.getPredicate() );
        print( ">" );

        print( " <" );
        print( st.getObject() );
        print( ">" );
    }

    public void print( Iterator<Statement> stmtIter )
        throws WurfelException
    {
        while ( stmtIter.hasNext() )
        {
            print( stmtIter.next() );
            print( "\n" );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
