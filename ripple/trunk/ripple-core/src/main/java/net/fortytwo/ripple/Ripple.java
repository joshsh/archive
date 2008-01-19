/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import java.util.Properties;

import net.fortytwo.ripple.rdf.RdfUtils;

import org.apache.log4j.PropertyConfigurator;

import org.openrdf.rio.RDFFormat;

/**
 *  Read-only configuration metadata.
 */
public final class Ripple
{
	public static final String URN_BNODE_PREFIX = "urn:bnode:";

	// TODO: these should probably not be HTTP URIs
	public static final String CACHE_CONTEXT = "http://fortytwo.net/2008/01/webclosure#context";
	public static final String CACHE_MEMO = "http://fortytwo.net/2008/01/webclosure#memo";
	
	private static boolean initialized = false;

	private static RippleProperties properties;
	
	private static boolean allowDuplicateNamespaces;
	private static RDFFormat cacheFormat;
	private static boolean bufferQueryResults;
	private static boolean resourceViewShowEdges;
	private static int resultViewMaxObjects;
	private static int resultViewMaxPredicates;
	private static boolean resultViewPrintEntireStack;
	private static String defaultNamespace;
	private static boolean dereferenceUrisByNamespace;
	private static EvaluationOrder evaluationOrder;
	private static EvaluationStyle evaluationStyle;
	private static RDFFormat exportFormat;
	private static ExpressionAssociativity expressionAssociativity;
	private static ExpressionOrder expressionOrder;
	private static String jLineDebugOutput;
	private static boolean listPadding;
	private static int maxWorkerThreads;
	private static boolean preferNewestNamespaceDefinitions;
	private static boolean rejectNonAssociatedStatements;
	private static long httpConnectionCourtesyInterval;
	private static long httpConnectionTimeout;
	private static boolean useInference;
	private static boolean pullEntireModel;
	private static boolean useBlankNodes;
	private static boolean memoizeListsFromRdf;
	private static boolean useAsynchronousQueries = true;
	
// FIXME: quiet is never used
	private static boolean quiet = false;

	////////////////////////////////////////////////////////////////////////////

	// TODO
	public static boolean asynchronousQueries()
	{
		return useAsynchronousQueries; 
	}
	public static void enableAsynchronousQueries( final boolean enable )
	{
		useAsynchronousQueries = enable;
	}
	
	private Ripple()
	{
	}

	public static void initialize( final File... configFiles )
		throws RippleException
	{
		if ( initialized )
		{
			return;
		}

		PropertyConfigurator.configure(
			Ripple.class.getResource( "log4j.properties" ) );

		Properties props = new Properties();

		try
		{
			props.load( Ripple.class.getResourceAsStream( "default.properties" ) );
			
			for ( int i = 0; i < configFiles.length; i++ )
			{
				InputStream is = new FileInputStream( configFiles[i] );
				props.load( is );
				is.close();
			}
		}

		catch ( IOException e )
		{
			throw new RippleException( "unable to load default.properties" );
		}
		
		properties = new RippleProperties( props );

		// Command-line interface
		bufferQueryResults = getBooleanProperty(
			props, "net.fortytwo.ripple.cli.bufferQueryResults", false );
		resourceViewShowEdges = getBooleanProperty(
				props, "net.fortytwo.ripple.cli.resourceViewShowEdges", true );
		resultViewMaxPredicates = getIntProperty(
			props, "net.fortytwo.ripple.cli.resultViewMaxPredicates", 32 );
		resultViewMaxObjects = getIntProperty(
			props, "net.fortytwo.ripple.cli.resultViewMaxObjects", 32 );
		resultViewPrintEntireStack = getBooleanProperty(
			props, "net.fortytwo.ripple.cli.resultViewPrintEntireStack", true );
		jLineDebugOutput = getStringProperty(
			props, "net.fortytwo.ripple.cli.jline.debugOutput", null );

		// Program control
		maxWorkerThreads = getIntProperty(
			props, "net.fortytwo.ripple.control.maxWorkerThreads", 50 );

		// Input/Output
		cacheFormat = getRdfFormatProperty(
			props, "net.fortytwo.ripple.io.cacheFormat", RDFFormat.TRIG );
		if ( cacheFormat != RDFFormat.TRIG && cacheFormat != RDFFormat.TRIX )
		{
			throw new RippleException( "cache must use one of the named graph formats (TriG or TriX)" );
		}

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
		httpConnectionTimeout = getLongProperty(
			props, "net.fortytwo.ripple.io.httpConnectionTimeout", 2000 );
		httpConnectionCourtesyInterval = getLongProperty(
			props, "net.fortytwo.ripple.io.httpConnectionCourtesyInterval", 500 );

		// Model
		useInference = getBooleanProperty(
			props, "net.fortytwo.ripple.model.useInference", false );
		listPadding = getBooleanProperty(
			props, "net.fortytwo.ripple.model.listPadding", false );
		pullEntireModel = getBooleanProperty(
			props, "net.fortytwo.ripple.model.lexicon.pullEntireModel", false );
		useBlankNodes = getBooleanProperty(
			props, "net.fortytwo.ripple.model.useBlankNodes", true );
		memoizeListsFromRdf = getBooleanProperty(
				props, "net.fortytwo.ripple.model.memoizeListsFromRdf", false );
		
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
	
	public static RippleProperties getProperties()
	{
		return properties;
	}

	////////////////////////////////////////////////////////////////////////////

	public static String getName()
	{
		return "Ripple";
	}

	public static String getVersion()
	{
		return "0.5-dev";
	}

	public static boolean getQuiet()
	{
		return quiet;
	}

	public static void setQuiet( final boolean q )
	{
		quiet = q;
	}

	////////////////////////////////////////////////////////////////////////////

	public static boolean allowDuplicateNamespaces()
	{
		return allowDuplicateNamespaces;
	}

	public static RDFFormat cacheFormat()
	{
		return cacheFormat;
	}

	public static boolean bufferQueryResults()
	{
		return bufferQueryResults;
	}

	public static boolean resourceViewShowEdges()
	{
		return resourceViewShowEdges;
	}
	
	public static int resultViewMaxObjects()
	{
		return resultViewMaxObjects;
	}

	public static int resultViewMaxPredicates()
	{
		return resultViewMaxPredicates;
	}

	public static boolean resultViewPrintEntireStack()
	{
		return resultViewPrintEntireStack;
	}

	public static String defaultNamespace()
	{
		return defaultNamespace;
	}

	public static boolean dereferenceUrisByNamespace()
	{
		return dereferenceUrisByNamespace;
	}

	public static RDFFormat exportFormat()
	{
		return exportFormat;
	}

	public static ExpressionAssociativity expressionAssociativity()
	{
		return expressionAssociativity;
	}

	public static ExpressionOrder expressionOrder()
	{
		return expressionOrder;
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

	public static boolean listPadding()
	{
		return listPadding;
	}

	public static int maxWorkerThreads()
	{
		return maxWorkerThreads;
	}

	public static boolean preferNewestNamespaceDefinitions()
	{
		return preferNewestNamespaceDefinitions;
	}

	public static boolean rejectNonAssociatedStatements()
	{
		return rejectNonAssociatedStatements;
	}

	public static void setCacheFormat( final RDFFormat format )
	{
		cacheFormat = format;
	}

	public static long httpConnectionCourtesyInterval()
	{
		return httpConnectionCourtesyInterval;
	}

	public static long httpConnectionTimeout()
	{
		return httpConnectionTimeout;
	}

	public static boolean useInference()
	{
		return useInference;
	}
	
	public static boolean useBlankNodes()
	{
		return useBlankNodes;
	}
	
	public static boolean memoizeListsFromRdf()
	{
		return memoizeListsFromRdf;
	}
	
	public static boolean lexiconPullsEntireModel()
	{
		return pullEntireModel;
	}

	////////////////////////////////////////////////////////////////////////////

	private static String getStringProperty( final Properties props,
											final String name,
											final String defaultValue )
	{
		String s = props.getProperty( name );

		return ( null == s )
			? defaultValue
			: s;
	}

	private static boolean getBooleanProperty( final Properties props,
											final String name,
											final boolean defaultValue )
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

	private static long getLongProperty( final Properties props,
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

	private static RDFFormat getRdfFormatProperty(
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

		private String name;

		private ExpressionOrder( final String n )
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

		private String name;

		private ExpressionAssociativity( final String n )
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

		private String name;

		private EvaluationOrder( final String name )
		{
			this.name = name;
		}

		public static EvaluationOrder find( final String name )
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

		private String name;

		private EvaluationStyle( final String name )
		{
			this.name = name;
		}

		public static EvaluationStyle find( final String name )
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
