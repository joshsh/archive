package net.fortytwo.ripple;

import java.util.Properties;

import net.fortytwo.ripple.rdf.RdfUtils;

import org.openrdf.rio.RDFFormat;

public class RippleProperties
{
	private Properties props;
	
	public RippleProperties( final Properties props )
	{
		this.props = props;
	}
	
	public String getString( final String name ) throws RippleException
	{
		String value = getProperty( name );
		return value;
	}

	public boolean getBoolean( final String name ) throws RippleException
	{
		String value = getProperty( name );
		
		return value.equals( "true" );
	}
	
	public int getInt( final String name ) throws RippleException
	{
		String value = getProperty( name );
	
		try
		{
			return ( new Integer( value ) ).intValue();
		}
		
		catch ( java.lang.NumberFormatException e )
		{
			throw new RippleException( e );
		}
	}
	
	public long getLong( final String name ) throws RippleException
	{
		String value = getProperty( name );
		
		try
		{
			return ( new Long( value ) ).longValue();
		}
		
		catch ( java.lang.NumberFormatException e )
		{
			throw new RippleException( e );
		}
	}
	
	public RDFFormat getRdfFormat( final String name ) throws RippleException
	{
		String value = getProperty( name );
		
		RDFFormat format = RdfUtils.findFormat( value );
		
		if ( null == format )
		{
			throw new RippleException( "unknown RDF format: " + value );
		}
		
		return format;
	}
		
	private String getProperty( final String name ) throws RippleException
	{
		String s = props.getProperty( name );
		
		if ( null == s )
		{
			throw new RippleException( "no value for property: " + name );
		}
		
		return s;
	}
}
