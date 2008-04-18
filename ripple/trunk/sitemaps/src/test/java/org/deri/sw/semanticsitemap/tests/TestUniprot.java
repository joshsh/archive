/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package org.deri.sw.semanticsitemap.tests;

import java.io.IOException;
import java.util.List;

import org.deri.sw.semanticsitemap.Dataset;
import org.deri.sw.semanticsitemap.RobotNotAllowedException;
import org.deri.sw.semanticsitemap.SemanticSitemapParser;
import org.xml.sax.SAXException;

public class TestUniprot {

	/**
	 * @param args
	 * @throws RobotNotAllowedException 
	 * @throws SAXException 
	 * @throws IOException 
	 */
	public static void main(String[] args) throws Exception {
		SemanticSitemapParser sitemap = new SemanticSitemapParser();
		sitemap.parse("http://purl.uniprot.org/sitemap.xml");

		for(Dataset ds : sitemap.getDatasets()) {
			System.out.print("Dataset ");
			
			if(null != ds.getLabel()) {
				System.out.print('"');
				System.out.print(ds.getLabel());
				System.out.print("\" ");
			}
			
			if(null != ds.getDatasetURI()) {
				System.out.print("located at ");
				System.out.print(ds.getDatasetURI());
				System.out.print(' ');
			}
			
			System.out.println("has:");
			
			printList("data dump locations", ds.getDataDumpLocations());
			printList("data fragment dump locations", ds.getDataFragmentDumpLocations());
			printList("linked data prefixes", ds.getLinkedDataPrefixes());
			printList("sample URIs", ds.getSampleURIs());
			printList("SPARQL endpoints", ds.getSPARQLEndpointLocations());
			
			if(null != ds.getChangeFrequency()) {
				System.out.print("\tand is changed ");
				System.out.println(ds.getChangeFrequency());
			}
			
			System.out.print('\n');
		}
	}
	
	private static void printList(String label, List<String> list) {
		if(list.size() > 0) {
			System.out.print('\t');
			System.out.print(label);
			System.out.println(':');
			
			for(String item : list) {
				System.out.print('\t');
				System.out.print('\t');
				System.out.println(item);
			}
		}
	}

}
