/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;

import java.util.Iterator;

public class TurtleView implements Sink<RippleList>
{
	// A three-space-INDENTed tree seems to be the most readable.
	private static final String INDENT = "   ";

	private static final String INDEX_SEPARATOR = "  ";

	private RipplePrintStream ps;
	private ModelConnection mc;
	private int index = 0;

	public TurtleView( final RipplePrintStream printStream,
						final ModelConnection mc )
		throws RippleException
	{
		ps = printStream;
		this.mc = mc;
	}

	public int size()
	{
		return index;
	}

	// Note: don't give this method a nil list.
	public void put( final RippleList stack ) throws RippleException
	{
		// Grab the topmost item on the stack.
		RippleValue first = stack.getFirst();

		// View the list in right-to-left order
		RippleList list = RippleList.invert( stack );

		ps.print( "rdf:_" + ++index + INDEX_SEPARATOR );
		ps.print( Ripple.resultViewPrintEntireStack() ? list : first );
		ps.print( "\n" );

		if ( Ripple.resourceViewShowEdges() )
		{
			Collector<RippleValue> predicates = new Collector<RippleValue>();
			mc.findPredicates( first, predicates );
	
			int predCount = 0,
				predlim = Ripple.resultViewMaxPredicates(),
				objlim = Ripple.resultViewMaxObjects();
	
			for ( Iterator<RippleValue> predIter = predicates.iterator();
				predIter.hasNext(); )
			{
				ps.print( INDENT );
	
				if ( ++predCount > predlim )
				{
					ps.print( "[...]\n" );
					break;
				}
	
				RippleValue predicate = predIter.next();
				ps.print( predicate );
				ps.print( "\n" );
	
				Collector<RippleValue> objects = new Collector<RippleValue>();
				mc.multiply( first, predicate, objects );
				int objCount = 0;
	
				for ( Iterator<RippleValue> objIter = objects.iterator();
					objIter.hasNext(); )
				{
					ps.print( INDENT );
					ps.print( INDENT );
	
					if ( ++objCount > objlim )
					{
						ps.print( "[...]\n" );
						break;
					}
	
					RippleValue object = objIter.next();
					ps.print( object );
					ps.print( ( objIter.hasNext() )
						? ","
						: ( predIter.hasNext() )
							? ";" : "." );
					ps.print( "\n" );
				}
			}
		}
	}
}

// kate: tab-width 4
