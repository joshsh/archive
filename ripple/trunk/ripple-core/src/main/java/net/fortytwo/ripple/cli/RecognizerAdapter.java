/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import java.io.PrintStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.util.Sink;

public class RecognizerAdapter
{
	private Sink<ListAst> querySink, continuingQuerySink;
	private Sink<Command> commandSink;
	private Sink<RecognizerEvent> eventSink;
	private PrintStream errorStream;

	// A helper variable for the lexer and parser.
	private String languageTag;

	public RecognizerAdapter( final Sink<ListAst> querySink,
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
			errorStream.println( "\nQuery error: " + e + "\n" );
			e.logError();
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
			errorStream.println( "\nQuery error: " + e + "\n" );
			e.logError();
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
			errorStream.println( "\nCommand error: " + e + "\n" );
			e.logError();
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
			errorStream.println( "\nEvent error: " + e + "\n" );
			e.logError();
		}
	}

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
