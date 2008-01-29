package net.fortytwo.ripple.cli;

import java.io.IOException;
import java.io.PrintStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

public class ParserExceptionSink implements Sink<Exception>
{
	private PrintStream errorPrintStream;
	
	public ParserExceptionSink( final PrintStream ps )
	{
		errorPrintStream = ps;
	}
	
	public void put( final Exception e )
	{
		// This happens, for instance, when the parser receives a value
		// which is too large for the target data type.  Non-fatal.
		if ( e instanceof NumberFormatException )
		{
			alert( e.toString() );
		}

		// Non-fatal.
		else if ( e instanceof antlr.RecognitionException )
		{
			alert( "Parser error: " + e.toString() );
		}

		// Non-fatal.
		else if ( e instanceof antlr.TokenStreamException )
		{
			/*Throwable cause = e.getCause();
			if ( null != cause && cause instanceof IOException )
			{
				
			}*/
			
			alert( "Lexer error: " + e.toString() );
		}

		else
		{
System.out.println("GOT AN ERROR!!!!!!!!!");
			alert( "Error: " + e.toString() );
			( new RippleException( e ) ).logError();
		}
	}
	
	private void alert( final String s )
	{
		errorPrintStream.println( "\n" + s + "\n" );
	}
};
