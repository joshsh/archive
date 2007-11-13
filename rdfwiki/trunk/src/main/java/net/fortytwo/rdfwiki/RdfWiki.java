package net.fortytwo.rdfwiki;

import java.io.InputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import net.fortytwo.ripple.rdf.SailInserter;

import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.RDFParser;
import org.openrdf.rio.Rio;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.memory.MemoryStore;
import org.restlet.Component;
import org.restlet.data.MediaType;
import org.restlet.data.Protocol;
import org.restlet.resource.Variant;

public class RdfWiki
{
	private static Sail sail = null;
	private static Map<RDFFormat, MediaType> rdfFormatToMediaTypeMap;
	private static Map<MediaType, RDFFormat> mediaTypeToRdfFormatMap;
	private static List<Variant> rdfVariants = null;
	
	private static void createSail() throws Exception
	{
		sail = new MemoryStore();
		sail.initialize();
		
		// Add some test data.
		addRdf( RdfWiki.class.getResourceAsStream( "dummydata.trig" ),
				RDFFormat.TRIG, "http://example.org/error/" );
	}
	
	private static void initialize() throws Exception
	{
		createSail();
		
		rdfFormatToMediaTypeMap = new HashMap<RDFFormat, MediaType>();
		
		// Note: preserves order of insertion
		mediaTypeToRdfFormatMap = new LinkedHashMap<MediaType, RDFFormat>();
		
		// Note: the first format registered becomes the default format.
		registerRdfFormat( RDFFormat.RDFXML );
		registerRdfFormat( RDFFormat.TURTLE );
		registerRdfFormat( RDFFormat.N3 );
		registerRdfFormat( RDFFormat.NTRIPLES );
		registerRdfFormat( RDFFormat.TRIG );
		registerRdfFormat( RDFFormat.TRIX );
	}
	
	private static void registerRdfFormat( final RDFFormat format )
	{
		MediaType t;
		
		if ( RDFFormat.RDFXML == format )
		{
			t = MediaType.APPLICATION_RDF_XML;
		}
		
		else
		{
			t = new MediaType( format.getDefaultMIMEType() );
		}
		
		rdfFormatToMediaTypeMap.put( format, t );
		mediaTypeToRdfFormatMap.put( t, format );
	}
	
	public static List<Variant> getRdfVariants()
	{
		if ( null == rdfVariants )
		{
			rdfVariants = new LinkedList<Variant>();
			Iterator<MediaType> types = mediaTypeToRdfFormatMap.keySet().iterator();
			while ( types.hasNext() )
			{
				rdfVariants.add( new Variant( types.next() ) );
			}
		}
		
System.out.println( "getRdfVariants() --> " + rdfVariants );
Iterator<Variant> iter = rdfVariants.iterator();
while(iter.hasNext()){
Variant v = iter.next();
System.out.println( "    " + v + " -- " + v.getMediaType().getName() + " -- " + v.getMediaType().getMainType() + "/" + v.getMediaType().getSubType() );
}
		return rdfVariants;
	}
	
	public static String getBaseUri()
	{
		return "http://localhost:8182/";
	}
	
	public static Sail getSail()
	{
		return sail;
	}
	
	public static MediaType findMediaType( final RDFFormat format )
	{
		return rdfFormatToMediaTypeMap.get( format );
	}
	
	public static RDFFormat findRdfFormat( final MediaType mediaType )
	{
		return mediaTypeToRdfFormatMap.get( mediaType );
	}
	
	public static void addRdf( final InputStream is, final RDFFormat format, final String baseUri )
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
	
	public static void main( final String[] args ) throws Exception
	{
		initialize();

		// Create a new Restlet component and add a HTTP server connector to it
		Component component = new Component();
		component.getServers().add( Protocol.HTTP, 8182 );

		
//		Restlet resourceRestlet = new ResourceRestlet();
//		component.getDefaultHost().attach( "/resource/", resourceRestlet );
		
//		Restlet htmlRestlet = new HtmlRestlet();
//		component.getDefaultHost().attach( "/page/", htmlRestlet );

//		Restlet sparqlRestlet = new SparqlRestlet();
//		component.getDefaultHost().attach( "/sparql", sparqlRestlet );
		
		component.getDefaultHost().attach( new RootApplication() );
		component.start();
	}

/*
	public static void main( final String[] args ) throws Exception
	{
		// Creating a minimal Restlet returning "Hello World"
		Restlet restlet = new Restlet() {
			@Override
			public void handle(Request request, Response response) {
				response.setEntity("Hello World!", MediaType.TEXT_PLAIN);
			}
		};
		
		// Create the HTTP server and listen on port 8182
		new Server(Protocol.HTTP, 8182, restlet).start();
	}
*/
}
