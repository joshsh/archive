package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.model.ModelConnection;

import org.openrdf.model.BNode;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.vocabulary.XMLSchema;

import java.io.OutputStream;
import java.io.PrintStream;

import java.util.Iterator;

public class WurfelPrintStream extends PrintStream
{
    private Lexicon lexicon;

    public WurfelPrintStream( OutputStream out, Lexicon lexicon, ModelConnection mc )
        throws WurfelException
    {
        super( out );
        this.lexicon = lexicon;
    }

    private void printUriRef( URI uri )
    {
        print( "<" + uri.toString() + ">" );
    }

    private void printQName( final String nsPrefix, final String localName )
    {
        if ( null != nsPrefix )
            print( nsPrefix );

        print( ":" );

        if ( null != localName )
            print( localName );
    }

    private void printUri( URI uri )
        throws WurfelException
    {
        String prefix = lexicon.nsPrefixOf( uri );

        if ( null == prefix )
            printUriRef( uri );
        else
            printQName( prefix, uri.getLocalName() );
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

                // Note: URI's equals() returns "true if the other object is an
                //       instance of URI  and their String-representations are
                //       equal, false  otherwise"
                if ( null != dataTypeUri )
                {
                    if ( dataTypeUri.equals( XMLSchema.BOOLEAN ) )
                        print( v.toString() );
                    else if ( dataTypeUri.equals( XMLSchema.DOUBLE ) )
                        print( v.toString() );
                    else if ( dataTypeUri.equals( XMLSchema.INTEGER ) )
                        print( v.toString() );
                    else if ( dataTypeUri.equals( XMLSchema.STRING ) )
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

                String language = ( (Literal) v ).getLanguage();
                if ( null != language )
                    print( "@" + language );
            }

            else if ( v instanceof BNode )
            {
                print( "_:" );
                print( ( (BNode) v ).getID() );
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
