package wurfel;

import java.net.URL;

import java.util.List;
import java.util.Iterator;

import org.apache.log4j.Logger;

import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryImpl;
import org.openrdf.sail.inferencer.MemoryStoreRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;

import wurfel.cli.Interpreter;
import wurfel.lucene.LuceneTest;
import wurfel.model.Model;
import wurfel.model.ModelConnection;

public class JLineTest
{
    private final static Logger s_logger = Logger.getLogger( JLineTest.class );

    public static Repository createTestRepository()
        throws WurfelException
    {
        try
        {
            Repository repository = new RepositoryImpl(
                new MemoryStoreRDFSInferencer(
                    new MemoryStore() ) );
//                    new MemoryStore( new java.io.File( "wurfel.tmp" ) ) ) );

            repository.initialize();

            return repository;
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    public static void main( final String [] args )
    {
        try
        {
            Wurfel.initialize();

LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel AND phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel OR phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel~" );

            Repository repository = createTestRepository();

            Model model = new Model( repository, "anonymousContext" );
            ModelConnection mc = new ModelConnection( model, "for JLineTest main" );
            if ( args.length == 2 )
                mc.addGraph( new URL( args[0] ), mc.createUri( args[1] ) );
            else if ( args.length == 1 )
                mc.addGraph( new URL( args[0] ) );
            mc.close();

            Interpreter r = new Interpreter( model );
            r.run();

            repository.shutDown();
        }

        catch ( Throwable t )
        {
            System.out.println( t.toString() );
        }

        List<String> openConnections = ModelConnection.listOpenConnections();
        if ( openConnections.size() > 0 )
        {
            Iterator<String> i = openConnections.iterator();
            String s = "" + openConnections.size() + " dangling connections: \"" + i.next() + "\"";
            while ( i.hasNext() )
                s += ", \"" + i.next() + "\"";

            s_logger.warn( s );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
