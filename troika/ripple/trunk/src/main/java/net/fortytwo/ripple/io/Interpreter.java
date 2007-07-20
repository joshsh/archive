package net.fortytwo.ripple.io;

import java.io.InputStream;
import java.io.PrintStream;

import net.fortytwo.ripple.RippleException;

import org.apache.log4j.Logger;

public class Interpreter
{
	final static Logger s_logger
		= Logger.getLogger( Interpreter.class );

	RecognizerInterface recognizerInterface;

	InputStream input;
	PrintStream errorOutput;

	public Interpreter( RecognizerInterface itf,
						final InputStream in,
						final PrintStream err )
	{
		recognizerInterface = itf;
		input = in;
		errorOutput = err;
	}

	public void parse()
	{
		// Break out when a @quit directive is encountered
		for (;;)
		{
			// If there's anything in the input buffer, it's because the parser
			// ran across a syntax error.  Clear the buffer and start afresh.
			clear( input );

			RippleLexer lexer = new RippleLexer( input );
			lexer.initialize( recognizerInterface );
			RippleParser parser = new RippleParser( lexer );
			parser.initialize( recognizerInterface );

			try
			{
				parser.nt_Document();

				// If the parser has exited normally, then we're done.
				break;
			}

			// Non-fatal.
			catch ( antlr.RecognitionException e )
			{
				alert( "RecognitionException: " + e.toString() );
			}

			// Non-fatal.
			catch ( antlr.TokenStreamException e )
			{
				alert( "TokenStreamException: " + e.toString() );
			}

			// This happens, for instance, when the parser receives a value
			// which is too large for the target data type.  Non-fatal.
			catch ( NumberFormatException e )
			{
				alert( e.toString() );
			}

			// The parser has received a quit command.
			catch ( ParserQuitException e )
			{
				s_logger.debug( "quit() called on Interpreter" );

				break;
			}

/*
			catch ( InterruptedException e )
			{
System.out.println( "interrupted!" );
			}
*/
		}
	}

	void alert( final String s )
	{
		errorOutput.println( "\n" + s + "\n" );
	}

	void clear( final InputStream is )
	{
		try
		{
			int lim = is.available();
			for ( int i = 0; i < lim; i++ )
				is.read();
		}

		catch ( java.io.IOException e )
		{
			alert( "Error: " + e );
		}
	}
}

// kate: tab-width 4
