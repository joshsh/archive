/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.PrintStream;

import java.net.URL;

import java.util.List;
import java.util.Iterator;

import net.fortytwo.ripple.cli.CommandLineInterface;
import net.fortytwo.ripple.io.CacheManager;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.query.Evaluator;
import net.fortytwo.ripple.query.LazyEvaluator;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.RdfUtils;

import org.apache.log4j.Logger;

import org.openrdf.repository.Repository;
import org.openrdf.rio.RDFFormat;

/**
 * Demo application.
 */
public final class Demo
{
	private static final Logger LOGGER = Logger.getLogger( Demo.class );

	private Demo()
	{
	}

	public static void demo( final File store,
							final InputStream in,
							final PrintStream out,
							final PrintStream err )
		throws RippleException
	{
//net.fortytwo.ripple.tools.SitemapsUtils.test();
		// Create a Sesame repository.
		Repository repository = RdfUtils.createMemoryStoreRepository();

		// Attach a Ripple model to the repository.
		Model model = new Model( repository, "Demo Model" );

		// Attach a query engine to the model.
		Evaluator evaluator = new LazyEvaluator();
		QueryEngine qe
			= new QueryEngine( model, evaluator, out, err );

		// Establish a connection.
		ModelConnection mc = qe.getConnection( "Demo connection" );

		// Load from store.
		if ( null != store )
		{
			LOGGER.info( "loading state from " + store );
			URL storeUrl;

			try
			{
				storeUrl = store.toURL();
			}

			catch ( java.net.MalformedURLException e )
			{
				throw new RippleException( e );
			}

			CacheManager.loadCache( storeUrl, Ripple.cacheFormat(), mc );
		}

		// Set the default namespace.
// TODO: should be done in the QueryEngine
mc.setNamespace( "", Ripple.defaultNamespace(), false );
qe.getLexicon().add( new org.openrdf.model.impl.NamespaceImpl( "", Ripple.defaultNamespace() ) );

		// Attach an interpreter to the query engine and let it read from
		// standard input.
		CommandLineInterface r = new CommandLineInterface( qe, in );
		r.run();

		// Save back to store.
		if ( null != store )
		{
			LOGGER.info( "saving state to " + store );
			OutputStream storeOut;

			try
			{
				storeOut = new FileOutputStream( store );
			}

			catch ( java.io.IOException e )
			{
				throw new RippleException( e );
			}

			// Write the cache out in the same format as it was read in.
			CacheManager.writeCacheTo( storeOut, Ripple.cacheFormat(), mc );

			try
			{
				storeOut.close();
			}

			catch ( java.io.IOException e )
			{
				throw new RippleException( e );
			}
		}

		// Must close the connection before the repository can be shut down.
		mc.close();

		List<String> openConnections = ModelConnection.listOpenConnections();
		if ( openConnections.size() > 0 )
		{
			Iterator<String> i = openConnections.iterator();
			String s = "" + openConnections.size() + " dangling connections: \"" + i.next() + "\"";

			while ( i.hasNext() )
			{
				s += ", \"" + i.next() + "\"";
			}

			LOGGER.warn( s );

			System.exit( 1 );
//			ModelConnection.closeOpenConnections();
		}

		// Shut down the Sesame repository.
		try
		{
			repository.shutDown();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	private static void printUsage()
	{
		System.out.println( "Usage:  ripple [options] [store]" );
		System.out.println( "Options:\n"
			+ "  -f <format>  Use <format> for the RDF store\n"
			+ "  -h           Print this help and exit\n"
			+ "  -q           Suppress normal output\n"
			+ "  -v           Print version information and exit" );
		System.out.println( "For more information, please see:\n"
			+ "  <URL:http://ripple.fortytwo.net/>." );
	}

	private static void printVersion()
	{
		System.out.println( Ripple.getName() + " " + Ripple.getVersion() );

		// Would be nice: list of extensions
	}

	public static void main( final String [] args )
	{
		try
		{
			// Load Ripple configuration.
			Ripple.initialize();
		}

		catch ( RippleException e )
		{
			System.err.println( "Initialization error: " + e );
			e.logError();
			System.exit( 1 );
		}

		// Default values.
		boolean quiet = false, showVersion = false, showHelp = false;
		File store = null;

		// Long options are available but are not advertised.
		LongOpt [] longOptions = {
			new LongOpt( "format", LongOpt.REQUIRED_ARGUMENT, null, 'f' ),
			new LongOpt( "help", LongOpt.NO_ARGUMENT, null, 'h' ),
			new LongOpt( "quiet", LongOpt.NO_ARGUMENT, null, 'q' ),
			new LongOpt( "version", LongOpt.NO_ARGUMENT, null, 'v' ) };

		Getopt g = new Getopt( Ripple.getName(), args, "f:hqv", longOptions );
		int c;
		while ( ( c = g.getopt() ) != -1 )
		{
			switch( c )
			{
				case 'q':
				case 2:
					quiet = true;
					break;
				case 'h':
				case 1:
					showHelp = true;
					break;
				case 'v':
				case 3:
					showVersion = true;
					break;
				case 'f':
				case 0:
					// Override the default cache format.
					RDFFormat format = RdfUtils.findFormat( g.getOptarg() );
					if ( null == format )
					{
						System.err.println( "Unknown format: " + g.getOptarg() );
						System.exit( 1 );
					}
					Ripple.setCacheFormat( format );
					break;
				case '?':
					 // Note: getopt() already printed an error
					printUsage();
					System.exit( 1 );
					break;
				default:
					System.err.print("getopt() returned " + c + "\n");
			}
		}

		int i = g.getOptind();
		if ( i < args.length )
		{
			// Too many non-option arguments.
			if ( args.length - i > 1 )
			{
				printUsage();
				System.exit( 1 );
			}

			store = new File( args[i] );
		}

		if ( showHelp )
		{
			printUsage();
			System.exit( 0 );
		}

		if ( showVersion )
		{
			printVersion();
			System.exit( 0 );
		}

		Ripple.setQuiet( quiet );
// System.out.println( "quiet = " + quiet );
// System.out.println( "showVersion = " + showVersion );
// System.out.println( "format = " + format );
// System.out.println( "store = " + store );

		try
		{
			demo( store, System.in, System.out, System.err );
		}

		catch ( RippleException e )
		{
			System.out.println( "Error: " + e );
			e.logError();
			System.exit( 1 );
		}

		// Exit despite any remaining active threads.
		System.exit( 0 );
	}
}

// kate: tab-width 4
