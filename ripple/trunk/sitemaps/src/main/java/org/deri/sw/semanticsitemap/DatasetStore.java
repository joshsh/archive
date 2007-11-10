package org.deri.sw.semanticsitemap;

import java.util.List;

/**
 * The DatasetStore allows a {@link SemanticSitemap} to store the
 * {@link Dataset}s found while parsing a Sitemap implementing <a
 * href="http://sw.deri.org/2007/07/sitemapextension/">The Semantic Web Crawling
 * Sitemap Extension</a>.
 *
 * @author Leandro M. López (inkel)
 *
 */
public interface DatasetStore {
    /**
     * Called when a {@link Dataset} is found and parsed.
     *
     * @param dataset
     */
    public void add(Dataset dataset);

    /**
     * Returns the list of {@link Dataset}s stored.
     *
     * @return A {@link List} of the {@link Dataset}s stored.
     */
    public List<Dataset> getDatasets();
}
