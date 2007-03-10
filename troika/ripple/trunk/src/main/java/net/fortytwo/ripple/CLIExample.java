package net.fortytwo.ripple;

import java.net.URL;

import java.util.List;
import java.util.Iterator;

import org.apache.log4j.Logger;

import org.openrdf.repository.Repository;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.sail.memory.MemoryStoreRDFSInferencer;
//import org.openrdf.sail.inferencer.MemoryStoreRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;

import net.fortytwo.ripple.cli.Interpreter;
//import net.fortytwo.ripple.lucene.LuceneTest;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;

public class CLIExample
{
    private final static Logger s_logger = Logger.getLogger( CLIExample.class );

    public static Repository createTestRepository()
        throws RippleException
    {
        try
        {
            Repository repository = Ripple.useInference()
                ? new SailRepository(
                    new MemoryStoreRDFSInferencer(
                        new MemoryStore() ) )
                : new SailRepository(
                    new MemoryStore() );
//                    new MemoryStore( new java.io.File( "net.fortytwo.ripple.tmp" ) ) ) );

            repository.initialize();

            return repository;
        }

        catch ( Throwable t )
        {
t.printStackTrace( System.err );
            throw new RippleException( t );
        }
    }

    public static void main( final String [] args )
    {
        try
        {
            Ripple.initialize();

            Repository repository = createTestRepository();

            Model model = new Model( repository, "anonymousContext" );
            ModelConnection mc = new ModelConnection( model, "for CLIExample main" );
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
