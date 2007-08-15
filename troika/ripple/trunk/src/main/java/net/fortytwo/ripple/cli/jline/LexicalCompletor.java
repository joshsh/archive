/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.jline;

import java.util.Collection;

public class LexicalCompletor extends RippleCompletor
{
	char [] delimiters = { ' ', '\t', '\n', '\r', '(', ')', '.', ';', '/', '!' };

	public LexicalCompletor( final Collection<String> alternatives )
	{
		super( alternatives );
	}

	boolean isDelimiter( final char c )
	{
		for ( int i = 0; i < delimiters.length; i++ )
			if ( delimiters[i] == c )
				return true;

		return false;
	}

	protected int findStartIndex( final String s )
	{
		int index = 0;

		for ( int i = 0; i < s.length(); i++ )
		{
			if ( isDelimiter( s.charAt( i ) ) )
				index = i + 1;
		}

		return index;
	}
}

// kate: tab-width 4
