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

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
