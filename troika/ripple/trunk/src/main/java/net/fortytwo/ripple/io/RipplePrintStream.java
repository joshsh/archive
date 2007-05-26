package net.fortytwo.ripple.io;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.StringUtils;

import org.openrdf.model.BNode;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.vocabulary.XMLSchema;

import java.io.OutputStream;
import java.io.PrintStream;

import java.util.Iterator;

public class RipplePrintStream extends PrintStream
{
	private Lexicon lexicon;

/*
private class Palette
{
	private static final int
		BLACK = 0,
		RED = 1,
		GREEN = 2,
		YELLOW = 3,
		BLUE = 4,
		MAGENTA = 5,
		CYAN = 6,
		WHITE = 7;

	private int bg = 7;
	private int text = 0;
	private int uri = 4;

	public String beginUri = "\u001B[0;" + 3 + uri + ";" + 4 + bg + "m";
	public String endUri = "\u001B[0;" + 3 + text + ";" + 4 + bg + "m";
}

Palette palette = new Palette();
*/

	public RipplePrintStream( OutputStream out, Lexicon lexicon )
		throws RippleException
	{
		super( out );
		this.lexicon = lexicon;
	}

	private void printUriRef( URI uri )
	{
//print( palette.beginUri );
		print( "<" + StringUtils.escapeUriString( uri.toString() ) + ">" );
//print( palette.endUri );
	}

	private void printUri( URI uri )
		throws RippleException
	{
		String symbol = lexicon.symbolFor( uri );

		if ( null == symbol )
			printUriRef( uri );
		else
			print( symbol );
	}

	private void printEscapedString( final String s )
	{
		print( '\"' );
		print( StringUtils.escapeString( s ) );
		print( '\"' );
	}

	public void print( RippleValue v )
		throws RippleException
	{
		if ( null == v )
			print( "()" );

		else
			v.printTo( this );
	}

	public void print( Value v )
		throws RippleException
	{
		if ( null == v )
			print( "()" );

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
		throws RippleException
	{
		print( "    " );
		print( st.getSubject() );

		print( " " );
		print( st.getPredicate() );

		print( " " );
		print( st.getObject() );
	}

	public void print( Iterator<Statement> stmtIter )
		throws RippleException
	{
		while ( stmtIter.hasNext() )
		{
			print( stmtIter.next() );
			print( "\n" );
		}
	}
}

// kate: tab-width 4
