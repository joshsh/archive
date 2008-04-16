package net.fortytwo.rdfwiki;

import org.openrdf.sail.Sail;
import org.restlet.Component;
import org.restlet.Context;
import org.restlet.data.Protocol;
import org.restlet.data.Request;

public class RdfWiki
{
	public static final String WIKI_ATTR = "wiki";

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
}
