package net.fortytwo.ripple.model;

import java.util.Hashtable;
import java.util.Set;

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
	 *  @return  a native data structure which is equated with the given RDF
	 *  value.  If there is no such data structure, the value itself.  This
	 *  method will never return <code>null</code>.
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

	public Set<Value> keySet()
	{
		return rdfToNativeMap.keySet();
	}
}

// kate: tab-width 4
