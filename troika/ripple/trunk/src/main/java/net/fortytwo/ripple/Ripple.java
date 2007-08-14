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
		s_containerViewBufferOutput = getBooleanProperty(
			props, "net.fortytwo.ripple.cli.containerViewBufferOutput", false );
		s_containerViewMaxPredicates = getIntProperty(
			props, "net.fortytwo.ripple.cli.containerViewMaxPredicates", 32 );
		if ( s_containerViewMaxPredicates < 0 )
			s_containerViewMaxPredicates = 0;
		s_containerViewMaxObjects = getIntProperty(
			props, "net.fortytwo.ripple.cli.containerViewMaxObjects", 32 );
		if ( s_containerViewMaxObjects < 0 )
			s_containerViewMaxObjects = 0;
		s_jLineDebugOutput = props.getProperty(
			"net.fortytwo.ripple.cli.jline.debugOutput" );

		// Program control

		// Input/Output
		s_cacheFormat = getRdfFormatProperty(
			props, "net.fortytwo.ripple.io.cacheFormat", RDFFormat.RDFXML );
		s_exportFormat = getRdfFormatProperty(
			props, "net.fortytwo.ripple.io.exportFormat", RDFFormat.RDFXML );
		s_rejectNonAssociatedStatements = getBooleanProperty(
			props, "net.fortytwo.ripple.io.rejectNonAssociatedStatements", true );
		s_preferNewestNamespaceDefinitions = getBooleanProperty(
			props, "net.fortytwo.ripple.io.preferNewestNamespaceDefinitions", false );
		s_dereferenceUrisByNamespace = getBooleanProperty(
			props, "net.fortytwo.ripple.io.dereferenceUrisByNamespace", false );
		s_urlConnectTimeout = getLongProperty(
			props, "net.fortytwo.ripple.io.urlConnectTimeout", 2000 );
		s_urlConnectCourtesyInterval = getLongProperty(
			props, "net.fortytwo.ripple.io.urlConnectCourtesyInterval", 500 );

		// Model
		s_useInference = getBooleanProperty(
			props, "net.fortytwo.ripple.model.useInference", false );
		s_listPadding = getBooleanProperty(
			props, "net.fortytwo.ripple.model.listPadding", false );

		// Queries
		s_defaultNamespace = getStringProperty(
			props, "net.fortytwo.ripple.query.defaultNamespace", "" );
		s_evaluationOrder = getEvaluationOrderProperty(
			props, "net.fortytwo.ripple.query.evaluationOrder", EvaluationOrder.LAZY );
		s_evaluationStyle = getEvaluationStyleProperty(
			props, "net.fortytwo.ripple.query.evaluationStyle", EvaluationStyle.COMPOSITIONAL );
		s_expressionAssociativity = ExpressionAssociativity.find(
			props.getProperty( "net.fortytwo.ripple.query.expressionAssociativity" ) );
		s_expressionOrder = ExpressionOrder.find(
			props.getProperty( "net.fortytwo.ripple.query.expressionOrder" ) );

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

	static ExpressionOrder s_expressionOrder;
	public static ExpressionOrder expressionOrder()
	{
		return s_expressionOrder;
	}

	static ExpressionAssociativity s_expressionAssociativity;
	public static ExpressionAssociativity expressionAssociativity()
	{
		return s_expressionAssociativity;
	}

	static EvaluationOrder s_evaluationOrder;
	public static EvaluationOrder evaluationOrder()
	{
		return s_evaluationOrder;
	}

	static EvaluationStyle s_evaluationStyle;
	public static EvaluationStyle evaluationStyle()
	{
		return s_evaluationStyle;
	}

	static String s_jLineDebugOutput;
	public static String jlineDebugOutput()
	{
		return s_jLineDebugOutput;
	}

	static boolean s_useInference;
	public static boolean useInference()
	{
		return s_useInference;
	}

	static boolean s_rejectNonAssociatedStatements;
	public static boolean rejectNonAssociatedStatements()
	{
		return s_rejectNonAssociatedStatements;
	}

	static boolean s_dereferenceUrisByNamespace;
	public static boolean dereferenceUrisByNamespace()
	{
		return s_dereferenceUrisByNamespace;
	}

	static boolean s_preferNewestNamespaceDefinitions;
	public static boolean preferNewestNamespaceDefinitions()
	{
		return s_preferNewestNamespaceDefinitions;
	}

	static long s_urlConnectTimeout;
	public static long urlConnectTimeout()
	{
		return s_urlConnectTimeout;
	}

	static String s_defaultNamespace;
	public static String defaultNamespace()
	{
		return s_defaultNamespace;
	}

	static boolean s_listPadding;
	public static boolean listPadding()
	{
		return s_listPadding;
	}

	static RDFFormat s_exportFormat;
	public static RDFFormat exportFormat()
	{
		return s_exportFormat;
	}

	static RDFFormat s_cacheFormat;
	public static RDFFormat cacheFormat()
	{
		return s_cacheFormat;
	}
	public static void setCacheFormat( final RDFFormat format )
	{
		s_cacheFormat = format;
	}

	static boolean s_containerViewBufferOutput;
	public static boolean containerViewBufferOutput()
	{
		return s_containerViewBufferOutput;
	}

	static int s_containerViewMaxPredicates;
	public static int containerViewMaxPredicates()
	{
		return s_containerViewMaxPredicates;
	}

	static int s_containerViewMaxObjects;
	public static int containerViewMaxObjects()
	{
		return s_containerViewMaxObjects;
	}

	static long s_urlConnectCourtesyInterval;
	public static long urlConnectCourtesyInterval()
	{
		return s_urlConnectCourtesyInterval;
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
			return defaultValue;

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
			return defaultValue;

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

	static EvaluationOrder getEvaluationOrderProperty(
		final Properties props,
		final String name,
		final EvaluationOrder defaultValue ) throws RippleException
	{
		String s = props.getProperty( name );

		if ( null == s )
			return defaultValue;

		else
			return EvaluationOrder.lookup( s );
	}

	static EvaluationStyle getEvaluationStyleProperty(
		final Properties props,
		final String name,
		final EvaluationStyle defaultValue ) throws RippleException
	{
		String s = props.getProperty( name );

		if ( null == s )
			return defaultValue;

		else
			return EvaluationStyle.lookup( s );
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
			return defaultValue;

		else
		{
			RDFFormat format = RdfUtils.findFormat( s );

			if ( null == format )
				throw new RippleException( "unknown RDF format: " + s );

			return format;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public enum ExpressionOrder
	{
		DIAGRAMMATIC      ( "diagrammatic" ),
		ANTIDIAGRAMMATIC  ( "antidiagrammatic" );

		private String name;
		private  ExpressionOrder( String n )
		{
			name = n;
		}

		public static ExpressionOrder find( final String name )
			throws RippleException
		{
			for ( ExpressionOrder x : ExpressionOrder.values() )
				if ( x.name.equals( name ) )
					return x;

			String msg = "unknown ExpressionOrder: '" + name + "'";
			throw new RippleException( msg );
		}
	}

	public enum ExpressionAssociativity
	{
		LEFT   ( "left" ),
		RIGHT  ( "right" );

		private String name;
		private  ExpressionAssociativity( String n )
		{
			name = n;
		}

		public static ExpressionAssociativity find( final String name )
			throws RippleException
		{
			for ( ExpressionAssociativity x : ExpressionAssociativity.values() )
				if ( x.name.equals( name ) )
					return x;

			String msg = "unknown ExpressionAssociativity: '" + name + "'";
			throw new RippleException( msg );
		}
	}

	public enum EvaluationOrder
	{
		EAGER  ( "eager" ),
		LAZY   ( "lazy" );

		private String name;

		private EvaluationOrder( String name )
		{
			this.name = name;
		}

		public static EvaluationOrder lookup( String name )
			throws RippleException
		{
			for ( EvaluationOrder order : EvaluationOrder.values() )
				if ( order.name.equals( name ) )
					return order;

			throw new RippleException( "unknown EvaluationOrder: " + name );
		}
	}

	public enum EvaluationStyle
	{
		APPLICATIVE    ( "applicative" ),
		COMPOSITIONAL  ( "compositional" );

		private String name;

		private EvaluationStyle( String name )
		{
			this.name = name;
		}

		public static EvaluationStyle lookup( String name )
			throws RippleException
		{
			for ( EvaluationStyle style : EvaluationStyle.values() )
				if ( style.name.equals( name ) )
					return style;

			throw new RippleException( "unknown EvaluationStyle: " + name );
		}
	}
}

// kate: tab-width 4
