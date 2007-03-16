package net.fortytwo.ripple.model;

import java.util.Hashtable;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class ModelBridge
{
	private Hashtable<URI, RippleValue> rdfToNativeMap;

	public ModelBridge()
	{
		rdfToNativeMap = new Hashtable<URI, RippleValue>();
	}

	/**
	 *  Note: not every RDF Value has a native equivalent (in other words,
	 *  this function MAY return <code>null</code>.
	 */
	public RippleValue toNative( Value sesameValue )
	{
		if ( sesameValue instanceof RippleValue )
			return (RippleValue) sesameValue;

		else
			return rdfToNativeMap.get( sesameValue );
	}

	/**
	 *  Note: every RippleValue has an RDF equivalent.
	 */
	public Value getRdfEquivalentOf( RippleValue v, ModelConnection mc )
		throws RippleException
	{
return v.toRdf( mc );
	}

	public void setNativeEquivalentOf( URI sesameValue, RippleValue nativeValue )
	{
		rdfToNativeMap.put( sesameValue, nativeValue );
	}

	public void add( RippleValue v )
	{
//...
	}

//	public void add( Function f )
//	{
//		setNativeEquivalentOf( f.getUri(), f );
//	}
}

// kate: tab-width 4
