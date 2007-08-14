package net.fortytwo.ripple.cli;

import java.io.PrintStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.util.Sink;

public class RecognizerInterface
{
	Sink<ListAst> querySink, continuingQuerySink;
	Sink<Command> commandSink;
	Sink<RecognizerEvent> eventSink;
	PrintStream errorStream;

	public RecognizerInterface( final Sink<ListAst> querySink,
								final Sink<ListAst> continuingQuerySink,
								final Sink<Command> commandSink,
								final Sink<RecognizerEvent> eventSink,
								final PrintStream errorStream )
	{
		this.querySink = querySink;
		this.continuingQuerySink = continuingQuerySink;
		this.commandSink = commandSink;
		this.eventSink = eventSink;
		this.errorStream = errorStream;
	}

	public void putQuery( final ListAst ast )
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

	public void putContinuingQuery( final ListAst ast )
	{
		try
		{
			continuingQuerySink.put( ast );
		}

		catch ( RippleException e )
		{
			errorStream.println( "\nError: " + e + "\n" );
		}
	}

	public void putCommand( final Command cmd )
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

	public void putEvent( final RecognizerEvent event )
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
	String languageTag;

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
