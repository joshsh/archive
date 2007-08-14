package net.fortytwo.ripple.cli.jline;

import java.util.Collection;

public class DirectiveCompletor extends RippleCompletor
{
	char [] delimiters = { ' ', '\t', '\n', '\r' };

	public DirectiveCompletor( final Collection<String> alternatives )
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

		boolean ok = true;
		for ( int i = 0; i < s.length(); i++ )
		{
			char c = s.charAt( i );

			if ( isDelimiter( c ) )
			{
				if ( ok )
					index = i + 1;
			}

			else if ( '.' == c )
			{
				ok = true;
				index = i + 1;
			}

			else
				ok = false;
		}

		return index;
	}
}

// kate: tab-width 4
