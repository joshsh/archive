/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.rdfwiki;

import java.io.InputStream;

import net.fortytwo.ripple.rdf.SailInserter;

import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.RDFParser;
import org.openrdf.rio.Rio;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.memory.MemoryStore;
import org.restlet.data.Request;

public class Demo
{
	private static Sail sail;
	
	public static void main( final String[] args ) throws Exception
	{
		// Note: we never shut down this Sail
		sail = new MemoryStore();
		sail.initialize();
		
		// Add some test data.
		addRdf( RdfWiki.class.getResourceAsStream( "dummydata.trig" ),
				RDFFormat.TRIG, "http://example.org/error/" );
		
		SailSelector selector = new SailSelector()
		{
			public Sail selectSail( final Request request ) throws Exception
			{
				return sail;
			}
		};
		
		new RdfWiki( selector );
	}
	
	private static void addRdf( final InputStream is, final RDFFormat format, final String baseUri )
			throws Exception
	{
		SailConnection sc = sail.getConnection();
		RDFHandler handler = new SailInserter( sc );
		RDFParser parser = Rio.createParser( format, sail.getValueFactory() );
		parser.setRDFHandler( handler );
		parser.parse( is, baseUri );
		sc.commit();
		sc.close();
	}
}
