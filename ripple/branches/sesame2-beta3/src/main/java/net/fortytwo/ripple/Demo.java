/**
*  $Id$
*  $LastChangedDate$
*  $Revision$
*  $Author$
*/

package net.fortytwo.ripple;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

	import java.awt.Color;
	import java.awt.SystemColor;

import java.io.File;
import java.io.OutputStream;
import java.io.FileOutputStream;

import java.net.URL;

import java.util.List;
import java.util.Iterator;

import org.apache.log4j.Logger;

import org.openrdf.repository.Repository;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.sail.memory.MemoryStoreRDFSInferencer;
//import org.openrdf.sail.inferencer.MemoryStoreRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;

import net.fortytwo.ripple.io.Interpreter;
import net.fortytwo.ripple.model.Evaluator;
import net.fortytwo.ripple.model.LazyEvaluator;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.query.QueryEngine;

public class Demo
{
	private final static Logger s_logger = Logger.getLogger( Demo.class );

	public static Repository createTestRepository()
		throws RippleException
	{
		try
		{
			Repository repository = Ripple.useInference()
				? new SailRepository(
					new MemoryStoreRDFSInferencer(
						new MemoryStore() ) )
				: new SailRepository(
					new MemoryStore() );
//                    new MemoryStore( new java.io.File( "net.fortytwo.ripple.tmp" ) ) ) );

			repository.initialize();

			return repository;
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	private static LongOpt [] longOptions = {
		new LongOpt( "format", LongOpt.REQUIRED_ARGUMENT, null, 'f' ),
		new LongOpt( "quiet", LongOpt.NO_ARGUMENT, null, 'q' ),
		new LongOpt( "version", LongOpt.NO_ARGUMENT, null, 'v' ) };

	private static void printUsage()
	{
		System.out.println( "usage:  ripple [-f FORMAT] [STORE]" );
	}

	public static void main( final String [] args )
	{

System.out.println( "system color: " + SystemColor.text );
//SystemColor.text = Color.red;

/*
Getopt g = new Getopt( Ripple.getName(), args, "ab:c::d" );
//
int c;
String arg;
while ((c = g.getopt()) != -1)
{
	switch(c)
	{
		case 'a':
		case 'd':
			System.out.print("You picked " + (char)c + "\n");
			break;
			//
		case 'b':
		case 'c':
			arg = g.getOptarg();
			System.out.print("You picked " + (char)c + 
							" with an argument of " +
							((arg != null) ? arg : "null") + "\n");
			break;
			//
		case '?':
			break; // getopt() already printed an error
			//
		default:
			System.out.print("getopt() returned " + c + "\n");
	}
}
System.exit( 0 );
*/






		try
		{
			File store = ( args.length > 0 ) ? new File( args[0] ) : null;

			// Load Ripple configuration.
			Ripple.initialize();

			// Create a Sesame repository.
			Repository repository = createTestRepository();

			// Attach a Ripple model to the repository.
			Model model = new Model( repository, "Demo Model" );

			// Set the default namespace.
			ModelConnection mc = new ModelConnection( model );
			mc.setNamespace( "", Ripple.getDefaultNamespace() );
			mc.close();

			// Load from store.
			if ( null != store )
			{
				s_logger.info( "loading state from " + store );
				model.load( store.toURL() );
			}

			// Attach a query engine to the model.
			Evaluator evaluator = new LazyEvaluator();
			QueryEngine qe
				= new QueryEngine( model, evaluator, System.out, System.err );

			// Attach an interpreter to the query engine and let it read from
			// standard input.
			Interpreter r = new Interpreter( qe, System.in );
			r.run();

			// Save back to store.
			if ( null != store )
			{
				s_logger.info( "saving state to " + store );
				OutputStream out = new FileOutputStream( store );
				model.writeTo( out );
				out.close();
			}

			// Shut down the Sesame repository.
			repository.shutDown();
		}

		catch ( Throwable t )
		{
			System.out.println( t.toString() );
		}

		List<String> openConnections = ModelConnection.listOpenConnections();
		if ( openConnections.size() > 0 )
		{
			Iterator<String> i = openConnections.iterator();
			String s = "" + openConnections.size() + " dangling connections: \"" + i.next() + "\"";
			while ( i.hasNext() )
				s += ", \"" + i.next() + "\"";

			s_logger.warn( s );
		}
	}
}

// kate: tab-width 4
