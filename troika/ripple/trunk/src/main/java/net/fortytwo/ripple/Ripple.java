package net.fortytwo.ripple;

import java.net.URL;

import java.util.Properties;

import java.io.IOException;

import org.apache.log4j.PropertyConfigurator;

public class Ripple
{
	public static String getName()
	{
		return "Ripple";
	}

	public static String getVersion()
	{
		return "0.3";
	}

	////////////////////////////////////////////////////////////////////////////

	private static boolean initialized = false;

	private static ExpressionOrder expressionOrder;
	private static ExpressionAssociativity expressionAssociativity;

	private static EvaluationOrder s_evaluationOrder;
	private static EvaluationStyle s_evaluationStyle;

	private static String s_jLineDebugOutput;

	private static boolean
		s_useInference,
		s_enforceImplicitProvenance;

	private static boolean s_dereferenceByNamespace;
	private static long s_uriDereferencingTimeout;
	private static String s_defaultNamespace;

	private static boolean s_listPadding;

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

	private static String getStringProperty( final Properties props,
											final String name,
											String defaultValue )
	{
		String s = props.getProperty( name );

		return ( null == s )
			? defaultValue
			: s;
	}

	private static boolean getBooleanProperty( final Properties props,
											final String name,
											boolean defaultValue )
	{
		String s = props.getProperty( name );

		return ( null == s )
			? defaultValue
			: s.equals( "true" );
	}

	private static int getIntProperty( final Properties props,
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

	private static long getLongProperty( final Properties props,
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

	private static EvaluationOrder getEvaluationOrderProperty(
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

	private static EvaluationStyle getEvaluationStyleProperty(
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

		expressionOrder = ExpressionOrder.find(
			props.getProperty( "net.fortytwo.ripple.io.syntax.order" ) );
		expressionAssociativity = ExpressionAssociativity.find(
			props.getProperty( "net.fortytwo.ripple.io.syntax.associativity" ) );

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
