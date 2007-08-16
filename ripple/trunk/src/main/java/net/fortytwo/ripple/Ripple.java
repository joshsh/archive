/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple;

import java.io.IOException;

import java.net.URL;

import java.util.Properties;

import net.fortytwo.ripple.util.RdfUtils;

import org.apache.log4j.PropertyConfigurator;

import org.openrdf.rio.RDFFormat;

/**
 *  Read-only configuration metadata.
 */
public class Ripple
{
	static boolean initialized = false;

	static ExpressionOrder expressionOrder;
	static ExpressionAssociativity expressionAssociativity;
	static EvaluationOrder evaluationOrder;
	static EvaluationStyle evaluationStyle;
	static String jLineDebugOutput;
	static boolean useInference;
	static boolean rejectNonAssociatedStatements;
	static boolean dereferenceUrisByNamespace;
	static boolean preferNewestNamespaceDefinitions;
	static boolean allowDuplicateNamespaces;
	static long urlConnectTimeout;
	static String defaultNamespace;
	static boolean listPadding;
	static RDFFormat exportFormat;
	static RDFFormat cacheFormat;
	static boolean containerViewBufferOutput;
	static int containerViewMaxPredicates;
	static int containerViewMaxObjects;
	static long urlConnectCourtesyInterval;

	////////////////////////////////////////////////////////////////////////////

	public static void initialize()
		throws RippleException
	{
		if ( initialized )
			return;

		PropertyConfigurator.configure(
			Ripple.class.getResource( "log4j.properties" ) );

		Properties props = new Properties();

		try
		{
			props.load( Ripple.class.getResourceAsStream( "ripple.properties" ) );
		}

		catch ( IOException e )
		{
			throw new RippleException( "unable to load ripple.properties" );
		}

		// Command-line interface
		containerViewBufferOutput = getBooleanProperty(
			props, "net.fortytwo.ripple.cli.containerViewBufferOutput", false );
		containerViewMaxPredicates = getIntProperty(
			props, "net.fortytwo.ripple.cli.containerViewMaxPredicates", 32 );
		if ( containerViewMaxPredicates < 0 )
		{
			containerViewMaxPredicates = 0;
		}
		containerViewMaxObjects = getIntProperty(
			props, "net.fortytwo.ripple.cli.containerViewMaxObjects", 32 );
		if ( containerViewMaxObjects < 0 )
		{
			containerViewMaxObjects = 0;
		}
		jLineDebugOutput = props.getProperty(
			"net.fortytwo.ripple.cli.jline.debugOutput" );

		// Program control

		// Input/Output
		cacheFormat = getRdfFormatProperty(
			props, "net.fortytwo.ripple.io.cacheFormat", RDFFormat.RDFXML );
		exportFormat = getRdfFormatProperty(
			props, "net.fortytwo.ripple.io.exportFormat", RDFFormat.RDFXML );
		rejectNonAssociatedStatements = getBooleanProperty(
			props, "net.fortytwo.ripple.io.rejectNonAssociatedStatements", true );
		preferNewestNamespaceDefinitions = getBooleanProperty(
			props, "net.fortytwo.ripple.io.preferNewestNamespaceDefinitions", false );
		allowDuplicateNamespaces = getBooleanProperty(
			props, "net.fortytwo.ripple.io.allowDuplicateNamespaces", false );
		dereferenceUrisByNamespace = getBooleanProperty(
			props, "net.fortytwo.ripple.io.dereferenceUrisByNamespace", false );
		urlConnectTimeout = getLongProperty(
			props, "net.fortytwo.ripple.io.urlConnectTimeout", 2000 );
		urlConnectCourtesyInterval = getLongProperty(
			props, "net.fortytwo.ripple.io.urlConnectCourtesyInterval", 500 );

		// Model
		useInference = getBooleanProperty(
			props, "net.fortytwo.ripple.model.useInference", false );
		listPadding = getBooleanProperty(
			props, "net.fortytwo.ripple.model.listPadding", false );

		// Queries
		defaultNamespace = getStringProperty(
			props, "net.fortytwo.ripple.query.defaultNamespace", "" );
		evaluationOrder = EvaluationOrder.find( getStringProperty(
			props, "net.fortytwo.ripple.query.evaluationOrder", "lazy" ) );
		evaluationStyle = EvaluationStyle.find( getStringProperty(
			props, "net.fortytwo.ripple.query.evaluationStyle", "compositional" ) );
		expressionAssociativity = ExpressionAssociativity.find( getStringProperty(
			props, "net.fortytwo.ripple.query.expressionAssociativity", "left" ) );
		expressionOrder = ExpressionOrder.find( getStringProperty(
			props, "net.fortytwo.ripple.query.expressionOrder", "diagrammatic" ) );

		initialized = true;
	}

	////////////////////////////////////////////////////////////////////////////

	public static String getName()
	{
		return "Ripple";
	}

	public static String getVersion()
	{
		return "0.4-dev";
	}

// FIXME: quiet is never used
	static boolean quiet = false;

	public static boolean getQuiet()
	{
		return quiet;
	}

	public static void setQuiet( final boolean q )
	{
		quiet = q;
	}

	////////////////////////////////////////////////////////////////////////////

	public static ExpressionOrder expressionOrder()
	{
		return expressionOrder;
	}

	public static ExpressionAssociativity expressionAssociativity()
	{
		return expressionAssociativity;
	}

	public static EvaluationOrder evaluationOrder()
	{
		return evaluationOrder;
	}

	public static EvaluationStyle evaluationStyle()
	{
		return evaluationStyle;
	}

	public static String jlineDebugOutput()
	{
		return jLineDebugOutput;
	}

	public static boolean useInference()
	{
		return useInference;
	}

	public static boolean rejectNonAssociatedStatements()
	{
		return rejectNonAssociatedStatements;
	}

	public static boolean dereferenceUrisByNamespace()
	{
		return dereferenceUrisByNamespace;
	}

	public static boolean preferNewestNamespaceDefinitions()
	{
		return preferNewestNamespaceDefinitions;
	}

	public static boolean allowDuplicateNamespaces()
	{
		return allowDuplicateNamespaces;
	}

	public static long urlConnectTimeout()
	{
		return urlConnectTimeout;
	}

	public static String defaultNamespace()
	{
		return defaultNamespace;
	}

	public static boolean listPadding()
	{
		return listPadding;
	}

	public static RDFFormat exportFormat()
	{
		return exportFormat;
	}

	public static RDFFormat cacheFormat()
	{
		return cacheFormat;
	}
	public static void setCacheFormat( final RDFFormat format )
	{
		cacheFormat = format;
	}

	public static boolean containerViewBufferOutput()
	{
		return containerViewBufferOutput;
	}

	public static int containerViewMaxPredicates()
	{
		return containerViewMaxPredicates;
	}

	public static int containerViewMaxObjects()
	{
		return containerViewMaxObjects;
	}

	public static long urlConnectCourtesyInterval()
	{
		return urlConnectCourtesyInterval;
	}

	////////////////////////////////////////////////////////////////////////////

	static String getStringProperty( final Properties props,
											final String name,
											String defaultValue )
	{
		String s = props.getProperty( name );

		return ( null == s )
			? defaultValue
			: s;
	}

	static boolean getBooleanProperty( final Properties props,
											final String name,
											boolean defaultValue )
	{
		String s = props.getProperty( name );

		return ( null == s )
			? defaultValue
			: s.equals( "true" );
	}

	static int getIntProperty( final Properties props,
									final String name,
									final int defaultValue )
		throws RippleException
	{
		String s = props.getProperty( name );

		if ( null == s )
		{
			return defaultValue;
		}

		else
		{
			try
			{
				return ( new Integer( s ) ).intValue();
			}

			catch ( java.lang.NumberFormatException e )
			{
				throw new RippleException( e );
			}
		}
	}

	static long getLongProperty( final Properties props,
									final String name,
									final long defaultValue )
		throws RippleException
	{
		String s = props.getProperty( name );

		if ( null == s )
		{
			return defaultValue;
		}

		else
		{
			try
			{
				return ( new Long( s ) ).longValue();
			}

			catch ( java.lang.NumberFormatException e )
			{
				throw new RippleException( e );
			}
		}
	}

	static RDFFormat getRdfFormatProperty(
		final Properties props,
		final String name,
		final RDFFormat defaultValue ) throws RippleException
	{
// System.out.println( "########## " + RDFFormat.N3 );
// System.out.println( "########## " + RDFFormat.NTRIPLES );
// System.out.println( "########## " + RDFFormat.RDFXML );
// System.out.println( "########## " + RDFFormat.TRIG );
// System.out.println( "########## " + RDFFormat.TRIX );
// System.out.println( "########## " + RDFFormat.TURTLE );
		String s = props.getProperty( name );

		if ( null == s )
		{
			return defaultValue;
		}

		else
		{
			RDFFormat format = RdfUtils.findFormat( s );

			if ( null == format )
			{
				throw new RippleException( "unknown RDF format: " + s );
			}

			return format;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public enum ExpressionOrder
	{
		DIAGRAMMATIC      ( "diagrammatic" ),
		ANTIDIAGRAMMATIC  ( "antidiagrammatic" );

		String name;
		ExpressionOrder( String n )
		{
			name = n;
		}

		public static ExpressionOrder find( final String name )
			throws RippleException
		{
			for ( ExpressionOrder x : ExpressionOrder.values() )
			{
				if ( x.name.equals( name ) )
				{
					return x;
				}
			}

			String msg = "unknown ExpressionOrder: '" + name + "'";
			throw new RippleException( msg );
		}
	}

	public enum ExpressionAssociativity
	{
		LEFT   ( "left" ),
		RIGHT  ( "right" );

		String name;
		ExpressionAssociativity( String n )
		{
			name = n;
		}

		public static ExpressionAssociativity find( final String name )
			throws RippleException
		{
			for ( ExpressionAssociativity x : ExpressionAssociativity.values() )
			{
				if ( x.name.equals( name ) )
				{
					return x;
				}
			}

			String msg = "unknown ExpressionAssociativity: '" + name + "'";
			throw new RippleException( msg );
		}
	}

	public enum EvaluationOrder
	{
		EAGER  ( "eager" ),
		LAZY   ( "lazy" );

		String name;

		EvaluationOrder( String name )
		{
			this.name = name;
		}

		public static EvaluationOrder find( String name )
			throws RippleException
		{
			for ( EvaluationOrder order : EvaluationOrder.values() )
			{
				if ( order.name.equals( name ) )
				{
					return order;
				}
			}

			throw new RippleException( "unknown EvaluationOrder: " + name );
		}
	}

	public enum EvaluationStyle
	{
		APPLICATIVE    ( "applicative" ),
		COMPOSITIONAL  ( "compositional" );

		String name;

		EvaluationStyle( String name )
		{
			this.name = name;
		}

		public static EvaluationStyle find( String name )
			throws RippleException
		{
			for ( EvaluationStyle style : EvaluationStyle.values() )
			{
				if ( style.name.equals( name ) )
				{
					return style;
				}
			}

			throw new RippleException( "unknown EvaluationStyle: " + name );
		}
	}
}

// kate: tab-width 4
