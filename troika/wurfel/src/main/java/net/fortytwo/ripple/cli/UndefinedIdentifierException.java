package net.fortytwo.ripple.ci;

import wurfel.WurfelException;

public class UndefinedIdentifierException extends WurfelException
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
