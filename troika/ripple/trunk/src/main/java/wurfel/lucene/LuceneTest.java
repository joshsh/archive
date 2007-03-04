package wurfel.lucene;

import wurfel.WurfelException;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;

import org.apache.lucene.queryParser.QueryParser;
//import org.apache.lucene.queryParser.surround.parser.QueryParser;

import org.apache.lucene.search.Hits;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

import java.io.File;

public class LuceneTest
{
//    public String queryStr( final String query )
    public static void search(File indexDir, String q)
        throws WurfelException
    {
        try
        {
//            Directory fsDir = FSDirectory.getDirectory(indexDir, false);
//            IndexSearcher is = new IndexSearcher(fsDir);

    QueryParser parser = new QueryParser( "contents", new StandardAnalyzer() );
    Query query = parser.parse( q );
    System.out.println( "query.toString() = " + query.toString() );
/*
    //        Query query = QueryParser.parse(q, "contents", new StandardAnalyzer());
            Hits hits = is.search(query);
            System.out.println("Found " + hits.length() +
                                " document(s) that matched query '" + q + "':");
            for (int i = 0; i < hits.length(); i++)
            {
                Document doc = hits.doc(i);
                System.out.println(doc.get("filename"));
            }
*/
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
