/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/libs/stream/Scrap.java $
 * $Revision: 1014 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.libs.stack.StackLibrary;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.StackMapping;
import net.fortytwo.ripple.util.Sink;

// kate: tab-width 4

/**
 * A filter which discards the stack unless the topmost item is the boolean
 * value stack:true.
 */
public class Require extends PrimitiveStackMapping
{
	private static final int ARITY = 1;

	public Require()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		RippleList stack = arg.getStack();

        PrimitiveStackMapping inner = new RequireInner( stack.getRest() );

        // TODO: it is inefficient to reduce the stack once to determine the boolean value, then again after the rest has been transmitted
        sink.put( arg.with( stack.push( Operator.OP ).push( inner ).push( Operator.OP ) ) );
    }

    private class RequireInner extends PrimitiveStackMapping
    {
        private RippleList rest;

        public RequireInner( final RippleList rest )
        {
            this.rest = rest;
        }

        public int arity()
        {
            return 1;
        }

        public void applyTo( final StackContext arg,
                             final Sink<StackContext> sink ) throws RippleException
        {
            RippleValue b;
            RippleList stack = arg.getStack();

            b = stack.getFirst();
            stack = stack.getRest();

            if ( b.equals( StackLibrary.getTrueValue() ) )
            {
                sink.put( arg.with( rest ) );
            }
        }
    }
}