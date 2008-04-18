/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


/**
 *
 */
package org.deri.sw.semanticsitemap;

import java.util.Hashtable;

import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;

// TODO extend DefaultHandler instead of implementing ContentHandler?

/**
 * <a href="http://sw.deri.org/2007/07/sitemapextension/">Semantic Web Crawling
 * Sitemap Extension</a> handler.
 * 
 * @author Leandro M. López (inkel)
 * 
 */
/**
 * @author inkel
 *
 */
public class SemanticSitemapHandler implements ContentHandler {

	private static final String DATASET = "dataset",
			DATASET_LABEL = "datasetLabel", DATASET_URI = "datasetURI",
			DATA_DUMP_LOCATION = "dataDumpLocation",
			DATA_FRAGMENT_DUMP_LOCATION = "dataFragmentDumpLocation",
			LINKED_DATA_PREFIX = "linkedDataPrefix",
			SPARQL_ENDPOINT = "sparqlEndoPoint", SAMPLE_URI = "sampleURI";

	private static final Hashtable<String, Boolean> ELEMENTS_WITH_DATA;

	/**
	 * Namespace used by the <a
	 * href="http://sw.deri.org/2007/07/sitemapextension/">Semantic Web Crawling
	 * Sitemap Extension</a>.
	 */
	public static final String NAMESPACE = "http://sw.deri.org/2007/07/sitemapextension/scschema.xsd";

	private static final String SITEMAP_NS = "http://www.sitemaps.org/schemas/sitemap/0.9";

	static {
		ELEMENTS_WITH_DATA = new Hashtable<String, Boolean>(7);
		ELEMENTS_WITH_DATA.put(DATASET_LABEL, true);
		ELEMENTS_WITH_DATA.put(DATASET_URI, true);
		ELEMENTS_WITH_DATA.put(DATA_DUMP_LOCATION, true);
		ELEMENTS_WITH_DATA.put(DATA_FRAGMENT_DUMP_LOCATION, true);
		ELEMENTS_WITH_DATA.put(LINKED_DATA_PREFIX, true);
		ELEMENTS_WITH_DATA.put(SPARQL_ENDPOINT, true);
		ELEMENTS_WITH_DATA.put(SAMPLE_URI, true);
	}

	private DatasetStore collector;

	private Dataset currentDataset = null;

	private StringBuffer dataBuffer = null;

	private boolean inDataset = false;

	/**
	 * Sets the object that will receive notification of found {@link Dataset}s.
	 * @param store
	 */
	public void setDatasetStore(DatasetStore store) {
		this.collector = store;
	}

	public void characters(char[] ch, int start, int length)
			throws SAXException {

		if (inDataset && null == dataBuffer) {
			dataBuffer = new StringBuffer(length);
			for (int i = start, j = start + length; i < j; i++) {
				dataBuffer.append(ch[i]);
			}
		}

	}

	public void endDocument() throws SAXException {
		// TODO Auto-generated method stub

	}

	public void endElement(String uri, String localName, String name)
			throws SAXException {

		if (NAMESPACE.equals(uri)) {
			if (DATASET.equals(localName)) {
				inDataset = false;
				collector.add(currentDataset);
				currentDataset = null;
			} else if (inDataset && ELEMENTS_WITH_DATA.containsKey(localName)) {
				String data = dataBuffer.toString();

				if (DATASET_LABEL.equals(localName)) {
					currentDataset.setLabel(data);
				} else if (DATASET_URI.equals(localName)) {
					currentDataset.setDatasetURI(data);
				} else if (DATA_DUMP_LOCATION.equals(localName)) {
					currentDataset.addDataDumpLocation(data);
				} else if (DATA_FRAGMENT_DUMP_LOCATION.equals(localName)) {
					currentDataset.addDataFragmentDumpLocation(data);
				} else if (LINKED_DATA_PREFIX.equals(localName)) {
					currentDataset.addLinkedDataPrefix(data);
				} else if (SPARQL_ENDPOINT.equals(localName)) {
					currentDataset.addSPARQLEndpointLocation(data);
				} else if (SAMPLE_URI.equals(localName)) {
					currentDataset.addSampleURI(data);
				}

				dataBuffer = null;
			}
		} else if (inDataset && SITEMAP_NS.equals(uri)) {
			if ("changefreq".equals(localName)) {
				String data = dataBuffer.toString().toUpperCase();
				currentDataset
						.setChangeFrequency(ChangeFrequency.valueOf(data));
				dataBuffer = null;
			}
		}

	}

	public void endPrefixMapping(String prefix) throws SAXException {
		// TODO Auto-generated method stub

	}

	public void ignorableWhitespace(char[] ch, int start, int length)
			throws SAXException {
		// TODO Auto-generated method stub

	}

	public void processingInstruction(String target, String data)
			throws SAXException {
		// TODO Auto-generated method stub

	}

	public void setDocumentLocator(Locator locator) {
		// TODO Auto-generated method stub

	}

	public void skippedEntity(String name) throws SAXException {
		// TODO Auto-generated method stub

	}

	public void startDocument() throws SAXException {
		// TODO Auto-generated method stub

	}

	public void startElement(String uri, String localName, String name,
			Attributes atts) throws SAXException {

		if (NAMESPACE.equals(uri)) {
			if (DATASET.equals(localName)) {
				inDataset = true;
				currentDataset = new Dataset();
			} else if (inDataset && ELEMENTS_WITH_DATA.containsKey(localName)) {
				dataBuffer = null;
			}
		} else if (SITEMAP_NS.equals(uri)) {
			if ("changefreq".equals(localName)) {
				dataBuffer = null;
			}
		}
	}

	public void startPrefixMapping(String prefix, String uri)
			throws SAXException {
		// TODO Auto-generated method stub

	}
}
