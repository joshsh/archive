package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.RippleException;

public class UndefinedIdentifierException extends RippleException
{
	private String identifier;

	public UndefinedIdentifierException( final String identifier )
	{
super("foo");

		this.identifier = identifier;
	}

	public String getIdentifier()
	{
		return identifier;
	}
}

// kate: tab-width 4
