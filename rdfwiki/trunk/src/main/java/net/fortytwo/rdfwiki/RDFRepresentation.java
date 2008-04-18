/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.rdfwiki;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RDFUtils;
import net.fortytwo.ripple.rdf.SesameOutputAdapter;
import net.fortytwo.ripple.rdf.RDFSource;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.rdf.RDFCollector;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParser;
import org.openrdf.rio.RDFWriter;
import org.openrdf.rio.Rio;
import org.restlet.resource.OutputRepresentation;
import org.restlet.resource.Representation;

import java.io.IOException;
import java.io.OutputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

public class RDFRepresentation extends OutputRepresentation
{
	private static final Logger LOGGER
		= Logger.getLogger( RDFRepresentation.class.getName() );
	
	private RDFSource source;
	private RDFFormat format;
	
	public RDFRepresentation( final Representation other, final String baseUri ) throws Exception
	{
		super( other.getMediaType() );
		RDFFormat format = RDFUtils.findRdfFormat( other.getMediaType() );
		
		if ( null == format )
		{
			throw new Exception( "bad media type: " + other.getMediaType() );
		}
		
		RDFParser parser = Rio.createParser( format );
		
		RDFCollector coll  = new RDFCollector();
		SesameInputAdapter sa = new SesameInputAdapter( coll );
		parser.setRDFHandler( sa );
		parser.parse( other.getStream(), baseUri );
		
		source = coll;
	}
	
	public RDFSource getSource()
	{
		return source;
	}
	
	public RDFRepresentation( final RDFSource source, final RDFFormat format ) throws RippleException
	{
		super( RDFUtils.findMediaType( format ) );
		
		this.source = source;
		this.format = format;
	}
	
	@Override
	public void write( final OutputStream os ) throws IOException
	{	
		try
		{
			RDFWriter writer = Rio.createWriter( format, os );
			SesameOutputAdapter soa = new SesameOutputAdapter( writer );
			soa.startRDF();
			source.writeTo( soa );
			soa.commentSink().put( "created by RDFWiki using the Rio 2 RDF toolkit" );
			soa.endRDF();
		}
		
		catch ( Throwable t )
		{
			if ( t instanceof IOException )
			{
				throw (IOException) t;
			}
			
			else
			{
				LOGGER.log( Level.WARNING, "failed to write RDF representation", t );
				throw new IOException( t.getMessage() );
			}
		}
	}
}
