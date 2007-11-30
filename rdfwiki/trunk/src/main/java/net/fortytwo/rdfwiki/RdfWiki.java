package net.fortytwo.rdfwiki;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.openrdf.rio.RDFFormat;
import org.openrdf.sail.Sail;
import org.restlet.Component;
import org.restlet.Context;
import org.restlet.data.MediaType;
import org.restlet.data.Protocol;
import org.restlet.data.Request;
import org.restlet.resource.Variant;

public class RdfWiki
{
	public static final String WIKI_ATTR = "wiki";
	
	private static Map<RDFFormat, MediaType> rdfFormatToMediaTypeMap;
	private static Map<MediaType, RDFFormat> mediaTypeToRdfFormatMap;
	private static List<Variant> rdfVariants = null;
	private static boolean initialized = false;
	
	private SailSelector sailSelector;
	
// FIXME: temporary
private static RdfWiki singleWiki;

	public static RdfWiki getWiki( final Context context )
	{
//		return (RdfWiki) context.getAttributes().get( WIKI_ATTR );
		return singleWiki;
	}
	
	public Sail getSail( final Request request ) throws Exception
	{
		return sailSelector.selectSail( request );
	}
		
	public RdfWiki( final SailSelector selector ) throws Exception
	{
		if ( !initialized )
		{
			initialize();
			initialized = true;
		}
singleWiki = this;

		sailSelector = selector;
		
		// Create a new Restlet component and add a HTTP server connector to it
		Component component = new Component();
		component.getServers().add( Protocol.HTTP, 8182 );
		
//		Restlet resourceRestlet = new ResourceRestlet();
//		component.getDefaultHost().attach( "/resource/", resourceRestlet );
		
//		Restlet htmlRestlet = new HtmlRestlet();
//		component.getDefaultHost().attach( "/page/", htmlRestlet );

//		Restlet sparqlRestlet = new SparqlRestlet();
//		component.getDefaultHost().attach( "/sparql", sparqlRestlet );

		component.getDefaultHost().getContext().getAttributes().put( WIKI_ATTR, this );
		component.getDefaultHost().attach( new RootApplication() );
		component.start();
	}
	
	private static void initialize() throws Exception
	{	
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

/*
System.out.println( "getRdfVariants() --> " + rdfVariants );
Iterator<Variant> iter = rdfVariants.iterator();
while(iter.hasNext()){
Variant v = iter.next();
System.out.println( "    " + v + " -- " + v.getMediaType().getName() + " -- " + v.getMediaType().getMainType() + "/" + v.getMediaType().getSubType() );
}*/
		return rdfVariants;
	}
	
	public static MediaType findMediaType( final RDFFormat format )
	{
		return rdfFormatToMediaTypeMap.get( format );
	}
	
	public static RDFFormat findRdfFormat( final MediaType mediaType )
	{
		return mediaTypeToRdfFormatMap.get( mediaType );
	}
}
