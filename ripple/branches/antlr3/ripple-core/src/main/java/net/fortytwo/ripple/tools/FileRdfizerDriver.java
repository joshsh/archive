/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.tools;

import org.openrdf.model.URI;
import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.rio.rdfxml.util.RDFXMLPrettyWriter;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;
import org.openrdf.sail.memory.MemoryStore;

import java.io.File;
import java.io.OutputStream;

import net.fortytwo.ripple.rdf.SesameOutputAdapter;
import net.fortytwo.ripple.rdf.CloseableIterationSource;
import net.fortytwo.ripple.flow.Source;
import net.fortytwo.ripple.RippleException;

public final class FileRdfizerDriver
{
	private FileRdfizerDriver()
	{
	}

	private static void printUsage()
	{
		System.out.println( "FileRdfizerDriver <fileName> [namespace]" );
	}

	private static void rdfize( final File file,
								final String namespace,
								final OutputStream out )
			throws
			org.openrdf.sail.SailException, RippleException
	{
		Sail sail = new MemoryStore();

		FileRdfizer rdfizer
			= new FileRdfizer( sail.getValueFactory() );

		URI context = sail.getValueFactory().createURI( namespace );
//		connection.setNamespace( "dir", namespace );

		SailConnection sc = sail.getConnection();
		rdfizer.addTree( file, context, namespace, sc );
		extractRDF( sc, out );
		sc.close();

		sail.shutDown();
	}

	public static void main( final String [] args )
	{
		if ( args.length != 1 )
		{
			printUsage();
			System.exit( 1 );
		}

		File file = new File( args[0] );

		try
		{
			String namespace = ( args.length > 1 )
				? args[1]
				: "urn:rdfizedFiles#" ;

			rdfize( file, namespace, System.out );
		}

		catch ( Throwable t )
		{
			System.err.println( t.toString() );
		}
	}

	private static void extractRDF( final SailConnection sc,
									final OutputStream out ) throws RippleException, SailException
	{
		RDFXMLWriter writer = new RDFXMLPrettyWriter( out );
		SesameOutputAdapter adapter = new SesameOutputAdapter( writer );

		adapter.startRDF();
		CloseableIterationSource<? extends Namespace, SailException> nsSource
				= new CloseableIterationSource(
			 			sc.getNamespaces() );
		( (Source<Namespace, RippleException>) nsSource ).writeTo( adapter.namespaceSink() );
		CloseableIterationSource<? extends Statement, SailException> stSource
				= new CloseableIterationSource(
						sc.getStatements( null, null, null, false )	);
		( (Source<Statement, RippleException>) stSource ).writeTo( adapter.statementSink() );
		adapter.endRDF();
	}
}

// kate: tab-width 4
