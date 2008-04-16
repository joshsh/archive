/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import java.io.IOException;
import java.io.InputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;

import org.apache.log4j.Logger;

import antlr.TokenStreamIOException;

public class Interpreter
{
	private static final Logger LOGGER
		= Logger.getLogger( Interpreter.class );

	private RecognizerAdapter recognizerAdapter;
	private InputStream input;
	private Sink<Exception, RippleException> exceptionSink;
	private boolean active = false;

	public Interpreter( final RecognizerAdapter rc,
						final InputStream in,
						final Sink<Exception, RippleException> exceptions )
	{
		recognizerAdapter = rc;
		input = in;
		exceptionSink = exceptions;
	}

	public void quit()
	{
		active = false;
	}

	public void parse() throws RippleException
	{
		active = true;

//System.out.println( "-- parse" );
		// Break out when a @quit directive is encountered
		while ( active )
		{
//System.out.println( "-- construct" );
			RippleLexer lexer = new RippleLexer( input );
			lexer.initialize( recognizerAdapter );
			RippleParser parser = new RippleParser( lexer );
			parser.initialize( recognizerAdapter );

			try
			{
//System.out.println( "-- antlr" );
				parser.nt_Document();

                // If the parser has exited normally, then we're done.
//System.out.println( "-- normal exit" );
				active = false;
			}

			// The parser has received a quit command.
			catch ( ParserQuitException e )
			{
//System.out.println( "-- quit" );
				LOGGER.debug( "quit() called on Interpreter" );

				active = false;
			}

            // TokenStreamIOException is considered fatal.  Two scenarios in
            // which it occurs are when the Interpreter thread has been
            // interrupted, and when the lexer has reached the end of input.
            catch ( TokenStreamIOException e )
			{
				LOGGER.debug( e );
				break;
			}
			
			catch ( Exception e )
			{
                // Handle non-fatal errors in an application-specific way.
                exceptionSink.put( e );
			}

            // If there's anything in the input buffer, it's because the parser
            // ran across a syntax error.  Clear the buffer, create a new lexer
            // and parser instance, and start afresh.
            // Note: this is a command-line usage scenario, and rules out
            // recovery from errors when the Interpreter is reading from a
            // pre-populated buffer.
            clear( input );            
        }
	}

	private static void clear( final InputStream is ) throws RippleException
	{
		try
		{
			int lim = is.available();
			for ( int i = 0; i < lim; i++ )
			{
				is.read();
			}
		}

		catch ( IOException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
