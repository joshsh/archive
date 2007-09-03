/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import java.io.InputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

public class Interpreter
{
	final static Logger logger
		= Logger.getLogger( Interpreter.class );

	RecognizerInterface recognizerInterface;

	InputStream input;

	Sink<Exception> exceptionSink;

	public Interpreter( final RecognizerInterface itf,
						final InputStream in,
						final Sink<Exception> exceptions )
	{
		recognizerInterface = itf;
		input = in;
		exceptionSink = exceptions;
	}

	public void parse()
		throws RippleException
	{
//System.out.println( "-- parse" );
		// Break out when a @quit directive is encountered
		for (;;)
		{
			// If there's anything in the input buffer, it's because the parser
			// ran across a syntax error.  Clear the buffer, create a new lexer
			// and parser instance, and start afresh.
			clear( input );
//System.out.println( "-- construct" );

			RippleLexer lexer = new RippleLexer( input );
			lexer.initialize( recognizerInterface );
			RippleParser parser = new RippleParser( lexer );
			parser.initialize( recognizerInterface );

			try
			{
//System.out.println( "-- antlr" );
				parser.nt_Document();

				// If the parser has exited normally, then we're done.
//System.out.println( "-- normal exit" );
				break;
			}

			// The parser has received a quit command.
			catch ( ParserQuitException e )
			{
//System.out.println( "-- quit" );
				logger.debug( "quit() called on Interpreter" );

				break;
			}

			catch ( Exception e )
			{
				exceptionSink.put( e );
			}

/*
catch ( Throwable t )
{
System.out.println( "caught some odd Throwable: " + t );
}
*/

/*
			catch ( InterruptedException e )
			{
System.out.println( "-- interrupted" );
			}
*/
		}
	}

	static void clear( final InputStream is )
		throws RippleException
	{
		try
		{
			int lim = is.available();
			for ( int i = 0; i < lim; i++ )
				is.read();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
