package wurfel;

import wurfel.cli.Interpreter;
import wurfel.lucene.LuceneTest;
import wurfel.model.EvaluationContext;

import org.apache.log4j.PropertyConfigurator;

import java.net.URL;

import java.util.List;
import java.util.Iterator;

import org.apache.log4j.Logger;

public class JLineTest
{
    private final static Logger s_logger = Logger.getLogger( JLineTest.class );

    public static void main( final String [] args )
    {
        try
        {
            Wurfel.initialize();



LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel AND phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel OR phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel~" );





            Context context = new Context( "anonymousContext" );
            EvaluationContext evalContext = new EvaluationContext( context, "for JLineTest main" );
            if ( args.length == 2 )
                context.importModel( new URL( args[0] ), evalContext.createUri( args[1] ), evalContext );
            else if ( args.length == 1 )
                context.importModel( new URL( args[0] ), null, evalContext );
            evalContext.close();

            Interpreter r = new Interpreter( context );
            r.run();
        }

        catch ( Throwable t )
        {
            System.out.println( t.toString() );
        }

        List<String> openConnections = EvaluationContext.listOpenConnections();
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
