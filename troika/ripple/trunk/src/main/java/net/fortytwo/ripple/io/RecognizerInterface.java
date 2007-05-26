package net.fortytwo.ripple.io;

import java.io.PrintStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.ast.ListAst;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.util.Sink;

public class RecognizerInterface
{
	private Sink<ListAst> querySink;
	private Sink<Command> commandSink;
	private Sink<RecognizerEvent> eventSink;
	private PrintStream errorStream;

	public RecognizerInterface( final Sink<ListAst> querySink,
							final Sink<Command> commandSink,
							final Sink<RecognizerEvent> eventSink,
							final PrintStream errorStream )
	{
		this.querySink = querySink;
		this.commandSink = commandSink;
		this.eventSink = eventSink;
		this.errorStream = errorStream;
	}

	public void put( final ListAst ast )
	{
		try
		{
			querySink.put( ast );
		}

		catch ( RippleException e )
		{
			errorStream.println( "\nError: " + e + "\n" );
		}
	}

	public void put( final Command cmd )
	{
		try
		{
			commandSink.put( cmd );
		}

		catch ( RippleException e )
		{
			errorStream.println( "\nError: " + e + "\n" );
		}
	}

	public void put( final RecognizerEvent event )
	{
		try
		{
			eventSink.put( event );
		}

		catch ( RippleException e )
		{
			errorStream.println( "\nError: " + e + "\n" );
		}
	}

	// A helper variable for the lexer and parser.
	private String languageTag;

	public String getLanguageTag()
	{
		return languageTag;
	}

	public void setLanguageTag( final String tag )
	{
		languageTag = tag;
	}
}

// kate: tab-width 4
