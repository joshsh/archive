package wurfel.tools;

import java.io.File;
import java.io.OutputStream;

import org.openrdf.model.URI;
import org.openrdf.repository.Connection;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryImpl;
import org.openrdf.rio.rdfxml.RDFXMLPrettyWriter;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.sail.memory.MemoryStore;

public class FileRdfizerDriver
{
	private static void printUsage()
	{
		System.out.println( "FileRdfizerDriver <fileName> [namespace]" );
	}

	private static void rdfize( File file, String namespace, OutputStream out )
		throws
			org.openrdf.sail.SailInitializationException,
			org.openrdf.sail.SailException,
			org.openrdf.rio.RDFHandlerException
	{
		Repository repository = new RepositoryImpl(
			new MemoryStore() );
		repository.initialize();

		FileRdfizer rdfizer
			= new FileRdfizer( repository.getValueFactory() );

		URI context = repository.getValueFactory().createURI( namespace );
//		connection.setNamespace( "dir", namespace );

		Connection connection = repository.getConnection();
		rdfizer.addTree( file, context, namespace, connection );
		extractRDF( connection, out );
		connection.close();

		repository.shutDown();
	}

	public static void main( String [] args )
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

	private static void extractRDF( Connection connection, OutputStream out )
		throws org.openrdf.rio.RDFHandlerException
	{
		RDFXMLWriter writer = new RDFXMLPrettyWriter( out );

		connection.export( writer );
	}
}
