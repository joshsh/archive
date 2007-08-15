package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ObservableContainer;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Observer;
import java.util.Observable;
import java.util.Set;

public class ContainerTreeView implements Observer
{
	private ObservableContainer valueSet;
	private RipplePrintStream ps;

	private Lexicon lexicon;

	// A three-space-indented tree seems to be the most readable.
	private static final String indent = "   ";

	private static final String indexSeparator = "  ";

	private static final int maxDepth = Ripple.getTreeViewDepth();

	public ContainerTreeView( ObservableContainer valueSet, RipplePrintStream printStream )
		throws RippleException
	{
		this.valueSet = valueSet;
		this.lexicon = lexicon;
		ps = printStream;
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

	private void printTreeView( RippleValue subject, int depth, String wsPrefix, ModelConnection mc )
		throws RippleException
	{
		if ( depth != maxDepth )
			ps.print( wsPrefix );

		for ( int i = 0; i < ( maxDepth - depth ) * 2; i++ )
			ps.print( indent );

		ps.print( subject );
		ps.print( "\n" );

		if ( depth > 0 )
		{
if ( subject instanceof net.fortytwo.ripple.model.RippleList )
subject = ( (net.fortytwo.ripple.model.RippleList) subject ).getFirst();

			RdfValueCollector predicates = new RdfValueCollector( mc.getModel().getBridge() );
			mc.findPredicates( subject, predicates );
//System.out.println( "" + predicates.size() + " predicates found." );
			Iterator<RdfValue> predIter = predicates.iterator();

			int predCount = 0, lim = Ripple.getTreeViewMaxBreadth();
			while ( predIter.hasNext() )
			{
				RdfValue predicate = predIter.next();

				ps.print( wsPrefix );

				for ( int i = 0; i < 1 + ( maxDepth - depth ) * 2; i++ )
					ps.print( indent );

				if ( ++predCount > lim )
				{
					ps.print( "[...]\n" );
					break;
				}

				ps.print( predicate );
				ps.print( "\n" );

				RdfValueCollector c = new RdfValueCollector( mc.getModel().getBridge() );
				mc.multiply( subject.toRdf( mc ), predicate, c );
				Iterator<RippleValue> objIter = c.iterator();

				int objCount = 0;
				while ( objIter.hasNext() )
				{
					if ( ++objCount > lim )
					{
						for ( int i = 0; i < 2 + ( maxDepth - depth ) * 2; i++ )
							ps.print( indent );
						ps.print( "[...]\n" );
						break;
					}

					else
						printTreeView( objIter.next(), depth - 1, wsPrefix, mc );
				}
			}
		}
	}

	private void refresh()
		throws RippleException
	{
		Model model = valueSet.getModel();
		Collection<RippleValue> values = valueSet.getValues();

		if ( 0 < values.size() )
			ps.println( "" );

		int treeViewDepth = Ripple.getTreeViewDepth();
		ModelConnection mc = new ModelConnection( model, "for ContainerTreeView refresh()" );

		try
		{
			int index = 0;
			Iterator<RippleValue> valuesIter = values.iterator();
			while ( valuesIter.hasNext() )
			{
//                String indexPrefix = "_" + ++index + indexSeparator;
				String indexPrefix = "rdf:_" + ++index + indexSeparator;
//                String wsPrefix = "                ".substring( 0, indexPrefix.length() );
				String wsPrefix = "";

				ps.print( indexPrefix );

				printTreeView( valuesIter.next(), treeViewDepth, wsPrefix, mc );
			}
		}

		catch ( RippleException e )
		{
			mc.close();
			throw e;
		}

		mc.close();

		ps.print( "\n" );
	}

	////////////////////////////////////////////////////////////////////////////

	private boolean changed = false;
	private boolean suspended = false;

	public synchronized void suspendEventHandling()
	{
		suspended = true;
	}

	public synchronized void resumeEventHandling()
		throws RippleException
	{
		if ( suspended )
		{
			if ( changed )
			{
				refresh();

				changed = false;
			}

			suspended = false;
		}
	}

	public void update( Observable o, Object arg )
	{
		try
		{
			if ( o == valueSet )
			{
				if ( suspended )
					changed = true;

				else
					refresh();
			}
		}

		catch ( RippleException e )
		{
			System.err.println( "\nError: " + e.toString() + "\n" );
		}
	}
}

// kate: tab-width 4
