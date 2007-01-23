package wurfel;

import wurfel.cli.Interpreter;
import wurfel.lucene.LuceneTest;
import wurfel.model.EvaluationContext;

import org.apache.log4j.PropertyConfigurator;

import java.net.URL;

public class JLineTest
{
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
            EvaluationContext evalContext = new EvaluationContext( context );
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
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
