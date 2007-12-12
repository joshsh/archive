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
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.Collection;
import java.util.Iterator;

import net.fortytwo.ripple.cli.CommandLineInterface;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.impl.sesame.SesameModel;
import net.fortytwo.ripple.query.Evaluator;
import net.fortytwo.ripple.query.LazyEvaluator;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.sail.LinkedDataSail;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.Rio;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailException;


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
		Sail baseSail = RdfUtils.createMemoryStoreSail();
		SailRepository repo = new SailRepository( baseSail );
		
		if ( null != store )
		{
			loadFromFile( repo, store );
		}
		
		UrlFactory urlFactory = new UrlFactory();
		Sail sail = new LinkedDataSail( baseSail, urlFactory );
		
		try
		{
			sail.initialize();
		}
		
		catch ( SailException e )
		{
			throw new RippleException( e );
		}

		// Attach a Ripple model to the repository.
		Model model = new SesameModel( sail, urlFactory );

		// Attach a query engine to the model.
		Evaluator evaluator = new LazyEvaluator();
		QueryEngine qe
			= new QueryEngine( model, evaluator, out, err );

		// Attach an interpreter to the query engine and let it read from
		// standard input.
		CommandLineInterface r = new CommandLineInterface( qe, in );
		r.run();

		Collection<ModelConnection> openConnections = model.openConnections();
		if ( openConnections.size() > 0 )
		{
			Iterator<ModelConnection> i = openConnections.iterator();
			String s = "" + openConnections.size() + " dangling connections: \"" + i.next().getName() + "\"";

			while ( i.hasNext() )
			{
				s += ", \"" + i.next().getName() + "\"";
			}

			LOGGER.warn( s );

			System.exit( 1 );
//			model.closeOpenConnections();
		}

		try
		{
			sail.shutDown();
		}
	
		catch ( SailException e )
		{
			throw new RippleException( e );
		}

		// Save back to store.
		if ( null != store )
		{
			saveToFile( repo, store );
		}

		try
		{
			repo.shutDown();
			baseSail.shutDown();
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

		// Would be nice: list of libraries
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
	

	private static void loadFromFile( final Repository repo, final File file ) throws RippleException
	{
		LOGGER.info( "loading state from " + file );

		FileInputStream in;
		
		try
		{
			in = new FileInputStream( file );
		}
		
		catch ( FileNotFoundException e )
		{
			LOGGER.info( "file " + file + " does not exist. It will be created on shutdown." );
			return;
		}
		
		try
		{
			RepositoryConnection rc = repo.getConnection();
			rc.add( in, "urn:nobaseuri#", Ripple.cacheFormat() );
			rc.commit();
			rc.close();
		}
		
		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
		
		try
		{
			in.close();
		}
		
		catch ( IOException e )
		{
			throw new RippleException( e );
		}
	}
	
	private static void saveToFile( final Repository repo, final File file ) throws RippleException
	{
		FileOutputStream out;
		
		try
		{
			out = new FileOutputStream( file );
		}
		
		catch ( FileNotFoundException e )
		{
			throw new RippleException( e );
		}
		
		RDFHandler writer = Rio.createWriter( Ripple.cacheFormat(), out );
		
		try
		{
			RepositoryConnection rc = repo.getConnection();
			rc.export( writer );
			rc.close();
		}
		
		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
		
		try
		{
			out.close();
		}
		
		catch ( IOException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
