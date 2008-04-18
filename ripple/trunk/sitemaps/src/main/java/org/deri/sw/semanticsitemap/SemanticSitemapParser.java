/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package org.deri.sw.semanticsitemap;

import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

/**
 * <p>Provides a parser and {@link DatasetStore} for <a
 * href="http://www.sitemaps.org/protocol.php">Sitemap protocol</a> files that
 * implements the <a href="http://sw.deri.org/2007/07/sitemapextension/">The
 * Semantic Web Crawling Sitemap Extension</a>.</p>
 *
 * <h2>Example</h2>
 *
 * <pre>
 * SemanticSitemapParser parser = new SemanticSitemapParser();
 * parser.setUserAgent(&quot;SemRobot/1.0 (DERI Semantic Sitemap Robot)&quot;);
 * parser.parse("http://example.org/sitemap.xml");
 *
 * for ({@link Dataset} dataset : parser.getDatasets()) {
 *     System.out.format(&quot;\tDataset %1$s at %2$s%n&quot;,
 *         dataset.getLabel(),
 *         dataset.getDatasetURI());
 *
 *     for (String location : dataset.getDataDumpLocations()) {
 *         System.out.println(&quot;\t\tdump at: &quot; + location);
 *     }
 * }
 * </pre>
 *
 * @author Leandro M. L�ez (inkel)
 *
 */
public class SemanticSitemapParser implements DatasetStore {
    private String userAgent = "Semantic Sitemap Parser/1.0";

    private List<Dataset> datasets;

    private SemanticSitemapHandler handler;

    private XMLReader reader;

    /**
     * Constructs a new {@code SemanticSitemapParser}.
     *
     * @throws SAXException
     *             if can not create an {@link XMLReader}.
     */
    public SemanticSitemapParser() throws SAXException {
        handler = new SemanticSitemapHandler();
        handler.setDatasetStore(this);

        reader = XMLReaderFactory.createXMLReader();
        reader.setFeature("http://xml.org/sax/features/namespaces", true);
        reader.setContentHandler(handler);

        this.datasets = new ArrayList<Dataset>();
    }

    /*
     * (non-Javadoc)
     *
     * @see org.deri.sw.semanticsitemap.DatasetStore#add(org.deri.sw.semanticsitemap.Dataset)
     */
//    @Override
    public void add(Dataset dataset) {
        this.datasets.add(dataset);
    }

    /*
     * (non-Javadoc)
     *
     * @see org.deri.sw.semanticsitemap.DatasetStore#getDatasets()
     */
//    @Override
    public List<Dataset> getDatasets() {
        return this.datasets;
    }

    /**
     * Parses url for presence of {@link Dataset}s as defined by <a
     * href="http://sw.deri.org/2007/07/sitemapextension/">The Semantic Web
     * Crawling Sitemap Extension</a>.
     *
     * @param url
     *            URL of a <a
     *            href="http://www.sitemaps.org/protocol.php">Sitemap protocol</a>
     *            file.
     * @throws IOException
     *             if an I/O exception occurs.
     * @throws SAXException
     *             if can not parse the URL.
     * @throws RobotNotAllowedException
     *             if access to this URL is disallowed by <a
     *             href="http://www.robotstxt.org/wc/exclusion.html#robotstxt">The
     *             Robots Exclusion Protocol</a>.
     */
    public void parse(URL url) throws IOException, SAXException,
            RobotNotAllowedException {
        RobotsTXTParser robots = new RobotsTXTParser(url.getHost());

        if (!robots.isRobotSafe(userAgent, url.getFile())) {
            throw new RobotNotAllowedException(url);
        }

        this.datasets.clear();

        reader.parse(new InputSource(url.openStream()));

    }

    /**
     * Parses url for presence of {@link Dataset}s as defined by <a
     * href="http://sw.deri.org/2007/07/sitemapextension/">The Semantic Web
     * Crawling Sitemap Extension</a>.
     *
     * @param url
     *            URL of a <a
     *            href="http://www.sitemaps.org/protocol.php">Sitemap protocol</a>
     *            file.
     * @throws IOException
     *             if an I/O exception occurs.
     * @throws SAXException
     *             if can not parse the URL.
     * @throws RobotNotAllowedException
     *             if access to this URL is disallowed by <a
     *             href="http://www.robotstxt.org/wc/exclusion.html#robotstxt">The
     *             Robots Exclusion Protocol</a>.
     */
    public void parse(String url) throws IOException, SAXException,
            RobotNotAllowedException {
        parse(new URL(url));
    }

    /**
     * Returns the parser's identification sent as User-Agent HTTP header.
     *
     * @return The {@link String} used as User-Agent HTTP header.
     */
    public String getUserAgent() {
        return userAgent;
    }

    /**
     * Sets the parser's identification sent as User-Agent HTTP header.
     *
     * @param userAgent
     *            The nre User-Agent string.
     */
    public void setUserAgent(String userAgent) {
        this.userAgent = userAgent;
    }
}
