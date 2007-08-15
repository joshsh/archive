package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

import java.util.Collection;

public class AmbiguousIdentifierException extends Exception
{
    private String identifier;
    private Collection<Value> candidates;

    public AmbiguousIdentifierException( final String identifier,
                                         final Collection<Value> candidates )
    {
super("foo");

        this.identifier = identifier;
        this.candidates = candidates;
    }

    public String getIdentifier()
    {
        return identifier;
    }

    public Collection<Value> getCandidates()
    {
        return candidates;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
