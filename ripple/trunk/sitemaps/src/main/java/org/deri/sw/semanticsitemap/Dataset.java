/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package org.deri.sw.semanticsitemap;

import java.util.ArrayList;
import java.util.List;

/**
 * <a href="http://sw.deri.org/2007/07/sitemapextension/">The Semantic Web
 * Crawling Sitemap Extension</a>.
 * 
 * @author Leandro M. López (inkel)
 * 
 */
public class Dataset {
	private ChangeFrequency changeFrequency;
	private String label, datasetURI;
	private List<String> linkedDataPrefixes, dataDumpLocations,
			dataFragmentDumpLocations, sparqlEndpointLocations, sampleURIs;

	/**
	 * Constructs a new (empty) <code>Dataset</code>.
	 */
	public Dataset() {
		linkedDataPrefixes = new ArrayList<String>();
		dataDumpLocations = new ArrayList<String>();
		dataFragmentDumpLocations = new ArrayList<String>();
		sparqlEndpointLocations = new ArrayList<String>();
		sampleURIs = new ArrayList<String>();
	}

	/**
	 * Adds the location of an RDF data dump.
	 * 
	 * @param location
	 *            The location of an RDF data dump.
	 */
	public void addDataDumpLocation(String location) {
		dataDumpLocations.add(location);
	}

	/**
	 * The location of a fragment of an RDF data dump, used for large datasets
	 * which are "split" over several files.
	 * 
	 * @param location
	 *            The location of a fragment of an RDF data dump.
	 */
	public void addDataFragmentDumpLocation(String location) {
		dataFragmentDumpLocations.add(location);
	}

	/**
	 * A prefix for linked data that a server hosts. URI/URLs that begin with
	 * this prefix will resolve to their RDF description.
	 * 
	 * @param prefix
	 *            A prefix for linked data that a server hosts.
	 */
	public void addLinkedDataPrefix(String prefix) {
		linkedDataPrefixes.add(prefix);
	}

	/**
	 * Adds a URI/URL within the dataset which can be considered a
	 * representative "sample".
	 * 
	 * @param uri
	 *            URI/URL within the dataset.
	 */
	public void addSampleURI(String uri) {
		sampleURIs.add(uri);
	}

	/**
	 * Adds the location of a SPARQL endpoint.
	 * 
	 * @param location
	 *            The location of a SPARQL endpoint.
	 */
	public void addSPARQLEndpointLocation(String location) {
		sparqlEndpointLocations.add(location);
	}

	/**
	 * Returns the change frequency of this dataset.
	 * 
	 * @return the change frequency of this dataset.
	 */
	public ChangeFrequency getChangeFrequency() {
		return changeFrequency;
	}

	/**
	 * Gets a list of locations of RDF data dumps.
	 * 
	 * @return list of locations of RDF data dumps.
	 */
	public List<String> getDataDumpLocations() {
		return dataDumpLocations;
	}

	/**
	 * Gets a list of locations of fragments of RDF data dumps.
	 * 
	 * @return list of locations of fragments of RDF data dumps.
	 */
	public List<String> getDataFragmentDumpLocations() {
		return dataFragmentDumpLocations;
	}

	/**
	 * Returns the URI of the dataset.
	 * 
	 * @return the URI of the dataset.
	 */
	public String getDatasetURI() {
		return datasetURI;
	}

	/**
	 * Returns a label describing the dataset.
	 * 
	 * @return a label describing the dataset.
	 */
	public String getLabel() {
		return label;
	}

	/**
	 * Gets a list of prefixes for linked data that a server hosts.
	 * 
	 * @return list of prefixes for linked data that a server hosts.
	 */
	public List<String> getLinkedDataPrefixes() {
		return linkedDataPrefixes;
	}

	/**
	 * Gets a list of URI/URL within the dataset which can be considered a
	 * representative "sample".
	 * 
	 * @return a list of URI/URLs.
	 */
	public List<String> getSampleURIs() {
		return sampleURIs;
	}

	/**
	 * Gets a list of locations of SPARQL endpoints.
	 * 
	 * @return list of locations of SPARQL endpoints.
	 */
	public List<String> getSPARQLEndpointLocations() {
		return sparqlEndpointLocations;
	}

	/**
	 * Indicates how often is updated this dataset.
	 * 
	 * @param changeFrequency
	 *            The change frequency of the dataset.
	 * @see ChangeFrequency
	 */
	public void setChangeFrequency(ChangeFrequency changeFrequency) {
		this.changeFrequency = changeFrequency;
	}

	/**
	 * Set a URI for the dataset. This facilitates the provision of further
	 * future annotations. This is <strong>recommended</strong>.
	 * 
	 * @param datasetURI
	 *            The URI of the dataset.
	 */
	public void setDatasetURI(String datasetURI) {
		this.datasetURI = datasetURI;
	}

	/**
	 * Set's a label describing the dataset. This is <strong>recommended</strong>.
	 * 
	 * @param label
	 *            The label of this dataset.
	 */
	public void setLabel(String label) {
		this.label = label;
	}
}
