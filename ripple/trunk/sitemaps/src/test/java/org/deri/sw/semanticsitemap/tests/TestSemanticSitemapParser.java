/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package org.deri.sw.semanticsitemap.tests;

import java.io.IOException;

import org.deri.sw.semanticsitemap.Dataset;
import org.deri.sw.semanticsitemap.RobotNotAllowedException;
import org.deri.sw.semanticsitemap.SemanticSitemapParser;
import org.xml.sax.SAXException;

public class TestSemanticSitemapParser {

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		test("Allowed", "http://localhost/sitemap.xml");
		test("Nothing", "http://localhost/sitemap.xml");
	}

	private static void test(String userAgent, String sitemapURL)
			throws SAXException, IOException, RobotNotAllowedException {

		System.out.format("Accessing %1$s as User-Agent %2$s%n", sitemapURL,
				userAgent);

		SemanticSitemapParser parser = new SemanticSitemapParser();

		parser.setUserAgent(userAgent);

		parser.parse(sitemapURL);

		for (Dataset dataset : parser.getDatasets()) {
			System.out.format("\tDataset %1$s at %2$s%n", dataset.getLabel(),
					dataset.getDatasetURI());

			for (String location : dataset.getDataDumpLocations()) {
				System.out.println("\t\tdump at: " + location);
			}
		}
	}

}
