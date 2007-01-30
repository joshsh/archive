package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.model.EvaluationContext;

import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;

import java.io.OutputStream;
import java.io.PrintStream;

import java.util.Iterator;

public class WurfelPrintStream extends PrintStream
{
    private URI
        xsdBooleanUri,
        xsdDoubleUri,
        xsdIntegerUri,
        xsdStringUri;

    private Lexicon lexicon;

    public WurfelPrintStream( OutputStream out, Lexicon lexicon, EvaluationContext evalContext )
        throws WurfelException
    {
        super( out );
        this.lexicon = lexicon;

        xsdBooleanUri = evalContext.createXmlSchemaUri( "boolean" );
        xsdDoubleUri = evalContext.createXmlSchemaUri( "double" );
        xsdIntegerUri = evalContext.createXmlSchemaUri( "integer" );
        xsdStringUri = evalContext.createXmlSchemaUri( "string" );
    }

    private void printUri( URI uri )
        throws WurfelException
    {
        String prefix = lexicon.nsPrefixOf( uri );

        if ( null == prefix )
            print( "<" + uri.toString() + ">" );
        else
            print( prefix + ":" + uri.getLocalName() );
    }

    private void printEscapedString( final String s )
    {
// TODO
print( "\"" + s + "\"" );
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
                printUri( (URI) v );

            else if ( v instanceof Literal )
            {
                URI dataTypeUri = ( (Literal) v ).getDatatype();

// FIXME: is this equals() safe?
                if ( null != dataTypeUri )
                {
                    if ( dataTypeUri.equals( xsdBooleanUri ) )
                        print( v.toString() );
                    else if ( dataTypeUri.equals( xsdDoubleUri ) )
                        print( v.toString() );
                    else if ( dataTypeUri.equals( xsdIntegerUri ) )
                        print( v.toString() );
                    else if ( dataTypeUri.equals( xsdStringUri ) )
                        printEscapedString( v.toString() );
                    else
                    {
                        printEscapedString( v.toString() );
                        print( "^^" );
                        printUri( dataTypeUri );
                    }
                }

                else
                    // For now, plain literals are printed as string-typed literals.
                    printEscapedString( v.toString() );
            }

            else
                print( v.toString() );
        }
    }

    public void print( Statement st )
        throws WurfelException
    {
        print( "    " );
        print( st.getSubject() );

        print( " " );
        print( st.getPredicate() );

        print( " " );
        print( st.getObject() );
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
