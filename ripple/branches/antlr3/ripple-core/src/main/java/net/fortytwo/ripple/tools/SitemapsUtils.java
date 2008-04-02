/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.tools;

import java.net.URL;

import java.util.Iterator;
import java.util.List;

import net.fortytwo.ripple.RippleException;

import org.deri.sw.semanticsitemap.Dataset;
import org.deri.sw.semanticsitemap.RobotsTXTParser;
import org.deri.sw.semanticsitemap.SemanticSitemapParser;

public final class SitemapsUtils
{
/*
http://purl.uniprot.org/sitemap.xml
http://www.rdfabout.com/sitemap.xml
*/
	private SitemapsUtils()
	{
	}

	public static void test() throws RippleException
	{
		try
		{
			testPrivate();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	private static void testPrivate() throws Exception
	{
		RobotsTXTParser parser = new RobotsTXTParser( "purl.uniprot.org" );
			boolean isSafe = parser.isRobotSafe( "Ripple-0.4", "sitemap.xml" );
System.out.println( "Is it safe?  " + ( (isSafe) ? "Yes" : "No" ) + "." );

		List<Dataset> dataSets;
		SemanticSitemapParser sm = new SemanticSitemapParser();
sm.setUserAgent( "Ripple-0.4" );
		dataSets = sm.getDatasets();
System.out.println( "dataSets.size() (before): " + dataSets.size() );
		URL url = new URL( "http://purl.uniprot.org/sitemap.xml" );
//		URL url = new URL( "http://purl.uniprot.org/taxonomy/9606" );
		sm.parse( url );
		dataSets = sm.getDatasets();
System.out.println( "dataSets.size() (after): " + dataSets.size() );
		Iterator<Dataset> dsIter = dataSets.iterator();
		while ( dsIter.hasNext() )
		{
System.out.println( "    data set:" );
			Dataset ds = dsIter.next();
System.out.println( "        ds.getChangeFrequency(): " + ds.getChangeFrequency() );
System.out.println( "        ds.getDataDumpLocations(): " + ds.getDataDumpLocations() );
System.out.println( "        ds.getDataFragmentDumpLocations(): " + ds.getDataFragmentDumpLocations() );
System.out.println( "        ds.getDatasetURI(): " + ds.getDatasetURI() );
System.out.println( "        ds.getLabel(): " + ds.getLabel() );
System.out.println( "        ds.getLinkedDataPrefixes(): " + ds.getLinkedDataPrefixes() );
System.out.println( "        ds.getSampleURIs(): " + ds.getSampleURIs() );
System.out.println( "        ds.getSPARQLEndpointLocations(): " + ds.getSPARQLEndpointLocations() );
		}
	}
}

// kate: tab-width 4
