package net.fortytwo.ripple.model;

import java.util.Hashtable;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

public class ModelBridge
{
	private Hashtable<Value, RippleValue> rdfToNativeMap;

	public ModelBridge()
	{
		rdfToNativeMap = new Hashtable<Value, RippleValue>();
	}

	/**
	*  Note: not every RDF Value has a native equivalent (in other words,
	*  this function MAY return <code>null</code>.
	*/
	public RippleValue get( RdfValue rdf )
	{
		RippleValue rpl =  rdfToNativeMap.get( rdf.getRdfValue() );

		if ( null == rpl )
			rpl = rdf;

		return rpl;
	}

	public void add( RippleValue v, ModelConnection mc )
		throws RippleException
	{
		rdfToNativeMap.put( v.toRdf( mc ).getRdfValue(), v );
	}
}

// kate: tab-width 4
