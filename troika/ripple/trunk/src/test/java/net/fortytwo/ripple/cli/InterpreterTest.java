/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;

public class InterpreterTest extends RippleTestCase
{
	int lineNumber;
	int getLineNumber() { return lineNumber; }
	void setLineNumber( int n ) { lineNumber = n; }

	void parse( final InputStream is, final boolean expectSuccess )
		throws Exception
	{
//		final Integer nQueries, nCommands, nEvents, lineNumber;
//		nQueries = nCommands = nEvents = lineNumber = 0;
		setLineNumber( 0 );

		final Collector<Exception> exceptions = new Collector<Exception>();

		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast )
				throws RippleException
			{
//				nQueries++;
			}
		};

		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd )
				throws RippleException
			{
//				nCommands++;
			}
		};

		PipedInputStream  writeIn = new PipedInputStream();
		final PipedOutputStream readOut = new PipedOutputStream( writeIn );

		final BufferedReader reader = new BufferedReader(
			new InputStreamReader( is ) );

		Sink<RecognizerEvent> eventSink = new Sink<RecognizerEvent>()
		{
			public void put( final RecognizerEvent event )
				throws RippleException
			{
//				nEvents++;
				switch ( event )
				{
					case NEWLINE:
System.out.println( "got a newline!!!!!!!!!!!!!!!!" );
						String line;

						// If lineNumber > 0, either a line has been
						// successfully matched, or an exception was thrown in
						// matching a line.
						if ( getLineNumber() > 0 )
						{
System.out.println( "(" + expectSuccess + ") exceptions.size() = " + exceptions.size() );
							if ( expectSuccess )
							{
								if ( exceptions.size() > 0 )
									fail( "Success case failed on line "
										+ getLineNumber() + ": " );// + line );
							}
		
							else
							{
								if ( exceptions.size() < 1 )
									fail( "Failure case succeeded on line "
										+ getLineNumber() + ": " );//+ line );
							}

							exceptions.clear();
						}

						do
						{
							try
							{
								line = reader.readLine();
							}
	
							catch ( java.io.IOException e )
							{
								throw new RippleException( e );
							}

							if ( null == line )
								throw new ParserQuitException();

							line = line.trim();

						} while ( line.startsWith( "#" ) || line.equals( "" ) );
System.out.println( "(" + expectSuccess + ") testing line " + getLineNumber() + ": " + line );

						// If we're here, there is another line of input to match.
						try
						{
System.out.println( "pushing line to readOut: " + line );
							readOut.write( line.getBytes() );
							readOut.write( '\n' );
//readOut.flush();
//							readOut.write( ' ' );
						}

						catch ( java.io.IOException e )
						{
							throw new RippleException( e );
						}

						setLineNumber( getLineNumber() + 1 );

						break;
					case ESCAPE:
						break;
					default:
						throw new RippleException( "event not yet supported: "
							+ event );
				}
			}
		};

		final RecognizerInterface itf = new RecognizerInterface(
			querySink, querySink, commandSink, eventSink, System.err );

		Sink<Exception> exceptionSink = new Sink<Exception>()
		{
			public void put( final Exception e )
				throws RippleException
			{
System.out.println( "########## got an exception: " + e );
				itf.putEvent( RecognizerEvent.NEWLINE );

				exceptions.put( e );
			}
		};

		Interpreter interpreter = new Interpreter( itf, writeIn, exceptionSink );
		interpreter.parse();
	}

	////////////////////////////////////////////////////////////////////////////

	private class SuccessCasesTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			InputStream is = InterpreterTest.class.getResourceAsStream( "successCases.rpl" );
			parse( is, true );
			is.close();
		}
	}

	private class FailureCasesTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			InputStream is = InterpreterTest.class.getResourceAsStream( "failureCases.rpl" );
			parse( is, false );
			is.close();
		}
	}

	public void runTests()
		throws Exception
	{
		testSynchronous( new SuccessCasesTest() );
		testSynchronous( new FailureCasesTest() );
	}
}

// kate: tab-width 4
