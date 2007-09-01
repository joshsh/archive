package net.fortytwo.ripple.io;

import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Statement;

public class RdfDiff
{
	LinkedList<Statement> added, subtracted;

	public RdfDiff()
	{
		added = new LinkedList<Statement>();
		subtracted = new LinkedList<Statement>();
	}

	public void add( final Statement st )
	{
		added.add( st );
	}

	public void subtract( final Statement st )
	{
		subtracted.add( st );
	}

	public Iterator<Statement> getAddedIterator()
	{
		return added.iterator();
	}

	public Iterator<Statement> getSubtractedIterator()
	{
		return subtracted.iterator();
	}
}

// kate: tab-width 4
