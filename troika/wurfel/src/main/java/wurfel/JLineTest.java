package wurfel;

import wurfel.cli.Interpreter;
import wurfel.lucene.LuceneTest;

import org.apache.log4j.PropertyConfigurator;

public class JLineTest
{
    public static void main( final String [] args )
    {
        Wurfel.initialize();

        try
        {



LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel AND phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel OR phase2" );
LuceneTest.search( new java.io.File("/home/joshs/tmp"), "wurfel~" );





            Context context = new Context( "anonymousContext" );
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
