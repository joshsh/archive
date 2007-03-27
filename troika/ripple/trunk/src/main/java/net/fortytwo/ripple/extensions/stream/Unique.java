package net.fortytwo.ripple.extensions.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.filter.Filter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.ListMemoizer;
import net.fortytwo.ripple.util.Sink;

public class Unique extends PrimitiveFunction
{
	public Unique( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 0;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		sink.put(
			new RippleList(
				new Operator(
					new UniqueInner() ), stack ) );
	}

	////////////////////////////////////////////////////////////////////////////

	private static String memo = "memo";

	protected class UniqueInner extends Filter
	{
		private ListMemoizer<RippleValue,String> memoizer = null;
	
		public UniqueInner()
		{
//System.out.println( "" + this + "()" );
		}
	
		public void applyTo( RippleList stack,
							Sink<RippleList> sink,
							ModelConnection mc )
			throws RippleException
		{
			if ( null == memoizer )
			{
				memoizer = new ListMemoizer<RippleValue,String>( stack, memo );
				sink.put( stack );
//System.out.println( "put first: " + stack.getFirst() );
			}
	
			else if ( memoizer.add( stack, memo ) )
//{
				sink.put( stack );
//System.out.println( "put another: " + stack.getFirst() );
//}
//else
//System.out.println( "rejected this: " + stack.getFirst() );
		}
	}
}

// kate: tab-width 4
