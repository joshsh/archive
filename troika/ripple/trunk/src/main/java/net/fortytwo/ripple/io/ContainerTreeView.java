package net.fortytwo.ripple.io;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;

public class ContainerTreeView implements Sink<RippleList>
{
	RipplePrintStream ps;
	ModelConnection mc;
	int index = 0;

	// A three-space-indented tree seems to be the most readable.
	static final String indent = "   ";

	static final String indexSeparator = "  ";

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
		ModelBridge bridge = mc.getModel().getBridge();

		Collector<RdfValue> predicates = new Collector<RdfValue>();
		mc.findPredicates( first, predicates );

		int predCount = 0,
			predlim = Ripple.getTreeViewMaxPredicates(),
			objlim = Ripple.getTreeViewMaxObjects();

		for ( Iterator<RdfValue> predIter = predicates.iterator();
			predIter.hasNext(); )
		{
			ps.print( indent );

			if ( ++predCount > predlim )
			{
				ps.print( "[...]\n" );
				break;
			}

			RdfValue predicate = predIter.next();
			ps.print( bridge.get( predicate ) );
			ps.print( "\n" );

			Collector<RdfValue> objects = new Collector<RdfValue>();
			mc.multiply( first.toRdf( mc ), predicate, objects );
			int objCount = 0;

			for ( Iterator<RdfValue> objIter = objects.iterator();
				objIter.hasNext(); )
			{
				ps.print( indent );
				ps.print( indent );

				if ( ++objCount > objlim )
				{
					ps.print( "[...]\n" );
					break;
				}

				RdfValue object = objIter.next();
				ps.print( bridge.get( object ) );
				ps.print( "\n" );
			}
		}
	}
}

// kate: tab-width 4
