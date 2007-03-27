package net.fortytwo.ripple.model;

import java.util.Collection;
//import java.util.LinkedHashSet;
import java.util.LinkedList;

import org.openrdf.model.Value;

public class Container extends LinkedList<RippleValue>//LinkedHashSet<Value>
{
	public Container()
	{
		super();
	}

	public Container( RippleValue v )
	{
		super();

		add( v );
	}

	public Container( Collection<RippleValue> other )
	{
		super();

		add( other );
	}

/*
	public boolean add( Value v )
	{
		return ( null == v ) ?
			false : ( (LinkedHashSet<Value>) this ).add( v );
	}
*/

	public boolean add( Collection<RippleValue> other )
	{
		return addAll( other );
	}
}

// kate: tab-width 4
