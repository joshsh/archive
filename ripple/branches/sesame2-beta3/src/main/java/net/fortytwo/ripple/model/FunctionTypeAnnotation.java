package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.NullSink;

import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;

// TODO
public class FunctionTypeAnnotation
{
	class Param
	{
		public String label;
		public RdfValue type;
		public String comment;
		public boolean transparency;

		public Param( final String label,
					final RdfValue type,
					final String comment,
					final boolean transparency )
		{
			this.label = label;
			this.type = type;
			this.comment = comment;
			this.transparency = transparency;
		}
	}

	private List<Param> params;
	private RdfValue returnType;
	private String label;
	private String comment;

	private RdfValue
		rplArityUri,
		rplParameterUri,
		rplParameterListUri,
		rplIsTransparentUri,
		rplParameterTypeUri,
		rplParametersUri,
		rplReturnTypeUri;

/*

	private Param getRippleParameter( final RippleValue head, ModelConnection mc )
		throws RippleException
	{
		String label = mc.stringValue(
			mc.findUniqueProduct( head, rplParameterNameUri ) ) );

		URI type = mc.castToUri(
			mc.findUniqueProduct( head, rplParameterTypeUri ) );

// TODO: actually use this, or get rid of it
		String comment = "";

		boolean transparency = mc.booleanValue(
			mc.castToLiteral(
				mc.findUniqueProduct( head, rplIsTransparentUri ) ) );

		return new Param( label, type, comment, transparency );
	}
*/
	private static String rplNs = "http://fortytwo.net/2007/03/ripple/schema#";

	public FunctionTypeAnnotation( final RdfValue self, ModelConnection mc )
		throws RippleException
	{
// TODO: actually use these, or get rid of them
		label = "";
		comment = "";

		rplParametersUri     = new RdfValue( mc.createUri( rplNs + "parameters" ) );
		rplReturnTypeUri     = new RdfValue( mc.createUri( rplNs + "returnType" ) );
		rplParameterUri      = new RdfValue( mc.createUri( rplNs + "Parameter" ) );
		rplParameterListUri  = new RdfValue( mc.createUri( rplNs + "ParameterList" ) );
		rplParameterTypeUri  = new RdfValue( mc.createUri( rplNs + "parameterType" ) );
		rplIsTransparentUri  = new RdfValue( mc.createUri( rplNs + "isTransparent" ) );

// Hack: this merely forces the model to dereference the primitive's URI so it
//       shows up in the Lexicon.
mc.multiply( self, rplReturnTypeUri, new NullSink<RdfValue>() );
/*
		returnType = mc.findUniqueProduct( self, rplReturnTypeUri );

		Resource paramList = mc.castToResource(
			mc.findUniqueProduct( self, rplParametersUri ) );
		Iterator<Value> paramIter = mc.listValue( paramList ).iterator();

		params = new ArrayList<Param>();

		while ( paramIter.hasNext() )
		{
			Value val = paramIter.next();
			if ( !( val instanceof Resource ) )
				throw new RippleException( "non-Resource encountered as an argument to a PrimitiveFunction" );
			else
				params.add( getRippleParameter( (Resource) val, mc ) );
		}
*/
	}

/*
	public String getLabel()
	{
		return label;
	}

	public URI getReturnType()
	{
		return returnType;
	}

	public String getComment()
	{
		return comment;
	}

	public int parameters()
	{
		return params.size();
	}

	private Param getParam( final int index )
		throws Exception
	{
		if ( index < 0 || index >= params.size() )
			throw new Exception( "parameter index out of bounds: " + index );

		return params.get( index );
	}

	public String getParameterLabel( final int index )
		throws Exception
	{
		Param p = getParam( index );
		return p.label;
	}

	public URI getParameterType( final int index )
		throws Exception
	{
		Param p = getParam( index );
		return p.type;
	}

	public String getParameterComment( final int index )
		throws Exception
	{
		Param p = getParam( index );
		return p.comment;
	}

	public boolean getParameterTransparency( final int index )
		throws Exception
	{
		Param p = getParam( index );
		return p.transparency;
	}

	public String toString()
	{
		String s = returnType.toString() + " <-- (";
		boolean first = true;
		Iterator<Param> iter = params.iterator();
		while( iter.hasNext() )
		{
			if ( first )
				first = false;
			else
				s += ", ";

			Param p = iter.next();
			s += p.type.toString() + " " + p.label;
		}

		s += ")";
		return s;
	}
*/
}


// kate: tab-width 4
