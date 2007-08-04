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
	public static String getName()
	{
		return "Ripple";
	}

	public static String getVersion()
	{
		return "0.4-dev";
	}

	////////////////////////////////////////////////////////////////////////////

	static boolean initialized = false;

	static ExpressionOrder expressionOrder;
	static ExpressionAssociativity expressionAssociativity;

	static EvaluationOrder s_evaluationOrder;
	static EvaluationStyle s_evaluationStyle;

	static String s_jLineDebugOutput;

	static boolean
		s_useInference,
		s_enforceImplicitProvenance;

	static boolean s_dereferenceByNamespace;
	static long s_uriDereferencingTimeout;
	static String s_defaultNamespace;

	static boolean s_listPadding;

	////////////////////////////////////////////////////////////////////////////

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

	public static ExpressionOrder getExpressionOrder()
	{
		return expressionOrder;
	}

	public static ExpressionAssociativity getExpressionAssociativity()
	{
		return expressionAssociativity;
	}

	public static EvaluationOrder getEvaluationOrder()
	{
		return s_evaluationOrder;
	}

	public static EvaluationStyle getEvaluationStyle()
	{
		return s_evaluationStyle;
	}

	public static String getJLineDebugOutput()
	{
		return s_jLineDebugOutput;
	}

	public static boolean useInference()
	{
		return s_useInference;
	}

	public static boolean enforceImplicitProvenance()
	{
		return s_enforceImplicitProvenance;
	}

	public static boolean dereferenceByNamespace()
	{
		return s_dereferenceByNamespace;
	}

	static boolean s_preferNewestNamespaceDefinitions;

	public static boolean preferNewestNamespaceDefinitions()
	{
		return s_preferNewestNamespaceDefinitions;
	}

	public static long uriDereferencingTimeout()
	{
		return s_uriDereferencingTimeout;
	}

	public static String getDefaultNamespace()
	{
		return s_defaultNamespace;
	}

	public static boolean listPadding()
	{
		return s_listPadding;
	}

	////////////////////////////////////////////////////////////////////////////

	static RDFFormat s_exportFormat, s_cacheFormat;

	public static RDFFormat exportFormat()
	{
		return s_exportFormat;
	}

	public static RDFFormat cacheFormat()
	{
		return s_cacheFormat;
	}
	public static void setCacheFormat( final RDFFormat format )
	{
		s_cacheFormat = format;
	}

	////////////////////////////////////////////////////////////////////////////

	static boolean s_bufferTreeView;
	static int s_treeViewMaxPredicates, s_treeViewMaxObjects;

	public static boolean getBufferTreeView()
	{
		return s_bufferTreeView;
	}

	public static int getTreeViewMaxPredicates()
	{
		return s_treeViewMaxPredicates;
	}

	public static int getTreeViewMaxObjects()
	{
		return s_treeViewMaxObjects;
	}

	////////////////////////////////////////////////////////////////////////////

	static long s_courtesyDelay;

	public static long getCourtesyDelay()
	{
		return s_courtesyDelay;
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

		s_jLineDebugOutput = props.getProperty(
			"net.fortytwo.ripple.io.jline.debugOutput" );

		s_evaluationOrder = getEvaluationOrderProperty(
			props, "net.fortytwo.ripple.model.evaluation.order", EvaluationOrder.LAZY );
		s_evaluationStyle = getEvaluationStyleProperty(
			props, "net.fortytwo.ripple.model.evaluation.style", EvaluationStyle.COMPOSITIONAL );

		s_useInference = getBooleanProperty(
			props, "net.fortytwo.ripple.model.rdf.useInference", false );
		s_enforceImplicitProvenance = getBooleanProperty(
			props, "net.fortytwo.ripple.model.rdf.enforceImplicitProvenance", true );

		s_preferNewestNamespaceDefinitions = getBooleanProperty(
			props, "net.fortytwo.ripple.model.namespace.preferNewest", false );

		s_dereferenceByNamespace = getBooleanProperty(
			props, "net.fortytwo.ripple.model.uri.dereferenceByNamespace", false );
		s_uriDereferencingTimeout = getLongProperty(
			props, "net.fortytwo.ripple.model.uri.dereferencing.timeout", 2000 );
		s_defaultNamespace = getStringProperty(
			props, "net.fortytwo.ripple.model.uri.defaultNamespace", "" );

		expressionOrder = ExpressionOrder.find(
			props.getProperty( "net.fortytwo.ripple.query.syntax.order" ) );
		expressionAssociativity = ExpressionAssociativity.find(
			props.getProperty( "net.fortytwo.ripple.query.syntax.associativity" ) );

		s_cacheFormat = getRdfFormatProperty(
			props, "net.fortytwo.ripple.io.cache.format", RDFFormat.RDFXML );
		s_exportFormat = getRdfFormatProperty(
			props, "net.fortytwo.ripple.io.export.format", RDFFormat.RDFXML );

		s_bufferTreeView = getBooleanProperty(
			props, "net.fortytwo.ripple.io.treeView.bufferOutput", false );
		s_treeViewMaxPredicates = getIntProperty(
			props, "net.fortytwo.ripple.io.treeView.maxPredicates", 32 );
		if ( s_treeViewMaxPredicates < 0 )
			s_treeViewMaxPredicates = 0;
		s_treeViewMaxObjects = getIntProperty(
			props, "net.fortytwo.ripple.io.treeView.maxObjects", 32 );
		if ( s_treeViewMaxObjects < 0 )
			s_treeViewMaxObjects = 0;

		s_listPadding = getBooleanProperty(
			props, "net.fortytwo.ripple.printing.listPadding", false );

		s_courtesyDelay = getLongProperty(
			props, "net.fortytwo.ripple.client.courtesyDelay", 500 );

		initialized = true;
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
