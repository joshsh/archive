package net.fortytwo.ripple.io;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;

public class ContainerTreeView implements Sink<RippleList>
{
	private RipplePrintStream ps;
	private ModelConnection mc;
	int index = 0;

	// A three-space-indented tree seems to be the most readable.
	private static final String indent = "   ";

	private static final String indexSeparator = "  ";

	private static final int maxDepth = Ripple.getTreeViewDepth();

	public ContainerTreeView( RipplePrintStream printStream, ModelConnection mc )
		throws RippleException
	{
		ps = printStream;
		this.mc = mc;
	}

	public int size()
	{
		return index;
	}

	// Note: don't give this method an empty list.
	public void put( final RippleList list )
		throws RippleException
	{
		ps.print( "rdf:_" + ++index + indexSeparator );
		ps.print( list );
		ps.print( "\n" );

		RippleValue first = list.getFirst();

		RdfValueCollector predicates = new RdfValueCollector( mc.getModel().getBridge() );
		mc.findPredicates( first, predicates );

		int predCount = 0, lim = Ripple.getTreeViewMaxBreadth();
		for ( Iterator<RdfValue> predIter = predicates.iterator();
			predIter.hasNext(); )
		{
			RdfValue predicate = predIter.next();
			ps.print( indent );

			if ( ++predCount > lim )
			{
				ps.print( "[...]\n" );
				break;
			}

			ps.print( predicate );
			ps.print( "\n" );

			RdfValueCollector objects = new RdfValueCollector( mc.getModel().getBridge() );
			mc.multiply( first.toRdf( mc ), predicate, objects );
			int objCount = 0;

			for ( Iterator<RippleValue> objIter = objects.iterator();
				objIter.hasNext(); )
			{
				ps.print( indent );
				ps.print( indent );

				if ( ++objCount > lim )
				{
					ps.print( "[...]\n" );
					break;
				}

				ps.print( objIter.next() );
				ps.print( "\n" );
			}
		}
	}

	private class RdfValueCollector<T> implements Sink<RdfValue>
	{
		private Collection<RippleValue> collectedValues;
		private RippleList stack;
		private ModelBridge bridge;

		public RdfValueCollector( ModelBridge bridge )
		{
			this.bridge = bridge;
			collectedValues = new LinkedList<RippleValue>();
		}

		public void put( RdfValue v )
			throws RippleException
		{
			collectedValues.add(
				bridge.get( v ) );
		}

		public int size()
		{
			return collectedValues.size();
		}

		public Iterator<RippleValue> iterator()
		{
			return collectedValues.iterator();
		}
	}
}

// kate: tab-width 4
