/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Source;

import org.openrdf.model.Statement;

public class RdfDiff
{
	private Collector<Statement> added, subtracted;

	public RdfDiff()
	{
		added = new Collector<Statement>();
		subtracted = new Collector<Statement>();
	}

	public void add( final Statement st ) throws RippleException
	{
		added.put( st );
	}

	public void subtract( final Statement st ) throws RippleException
	{
		subtracted.put( st );
	}

	public int countAdded()
	{
		return added.size();
	}

	public int countSubtracted()
	{
		return subtracted.size();
	}

	public Source<Statement> getAdded()
	{
		return added;
	}

	public Source<Statement> getSubtracted()
	{
		return subtracted;
	}
}

// kate: tab-width 4
