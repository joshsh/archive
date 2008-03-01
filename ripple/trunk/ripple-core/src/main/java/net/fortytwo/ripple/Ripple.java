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

    public static final String
            BUFFER_QUERY_RESULTS                = "net.fortytwo.ripple.cli.bufferQueryResults",
            RESOURCE_VIEW_SHOW_EDGES            = "net.fortytwo.ripple.cli.resourceViewShowEdges",
            RESULT_VIEW_MAX_OBJECTS             = "net.fortytwo.ripple.cli.resultViewMaxObjects",
            RESULT_VIEW_MAX_PREDICATES          = "net.fortytwo.ripple.cli.resultViewMaxPredicates",
            JLINE_DEBUG_OUTPUT                  = "net.fortytwo.ripple.cli.jline.debugOutput",
            RESULT_VIEW_PRINT_ENTIRE_STACK      = "net.fortytwo.ripple.cli.resultViewPrintEntireStack",
            MAX_WORKER_THREADS                  = "net.fortytwo.ripple.control.maxWorkerThreads",
            ALLOW_DUPLICATE_NAMESPACES          = "net.fortytwo.ripple.io.allowDuplicateNamespaces",
            CACHE_FORMAT                        = "net.fortytwo.ripple.io.cacheFormat",
            DEREFERENCE_URIS_BY_NAMESPACE       = "net.fortytwo.ripple.io.dereferenceUrisByNamespace",
            EXPORT_FORMAT                       = "net.fortytwo.ripple.io.exportFormat",
            PREFER_NEWEST_NAMESPACE_DEFINITIONS = "net.fortytwo.ripple.io.preferNewestNamespaceDefinitions",
            REJECT_NONASSOCIATED_STATEMENTS     = "net.fortytwo.ripple.io.rejectNonAssociatedStatements",
            HTTPCONNECTION_COURTESY_INTERVAL    = "net.fortytwo.ripple.io.httpConnectionCourtesyInterval",
            HTTPCONNECTION_TIMEOUT              = "net.fortytwo.ripple.io.httpConnectionTimeout",
            LIST_PADDING                        = "net.fortytwo.ripple.model.listPadding",
            USE_INFERENCE                       = "net.fortytwo.ripple.model.useInference",
            PULL_ENTIRE_MODEL                   = "net.fortytwo.ripple.model.lexicon.pullEntireModel",
            USE_BLANK_NODES                     = "net.fortytwo.ripple.model.useBlankNodes",
            MEMOIZE_LISTS_FROM_RDF              = "net.fortytwo.ripple.model.memoizeListsFromRdf",
            DEFAULT_NAMESPACE                   = "net.fortytwo.ripple.query.defaultNamespace",
            EVALUATION_ORDER                    = "net.fortytwo.ripple.query.evaluationOrder",
            EVALUATION_STYLE                    = "net.fortytwo.ripple.query.evaluationStyle",
            EXPRESSION_ASSOCIATIVITY            = "net.fortytwo.ripple.query.expressionAssociativity",
            EXPRESSION_ORDER                    = "net.fortytwo.ripple.query.expressionOrder",
            // TODO: .........
            USE_ASYNCHRONOUS_QUERIES            = "";

    private static boolean initialized = false;

	private static RippleProperties properties;
	
    // TODO: get rid of these
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

    // TODO: move this
    public static boolean useInference() throws RippleException
	{
		return properties.getBoolean( USE_INFERENCE );
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
