/*
JENA=/home/joshs/bin/ontology/jena/Jena-2.4

javac -classpath .:$JENA/lib/jena.jar phase2/jena/*.java

javac -classpath .:$JENA/lib/jena.jar phase2/jena/RDFImporter.java

java -classpath .:$JENA/lib/jena.jar:$JENA/lib/commons-logging.jar:$JENA/lib/xercesImpl.jar:$JENA/lib/iri.jar:$JENA/lib/icu4j_3_4.jar phase2/jena/RDFImporter http://www.w3.org/2004/02/skos/core

http://www.w3.org/2004/02/skos/core

www.w3.org
    1999
        02
            22-rdf-syntax-ns
    2004
        02
            skos
                core
                    Concept

boolean
char
double
float
long
Object
RDFNode
String

*/

package phase2.jena;

import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.rdf.model.Resource;
import com.hp.hpl.jena.rdf.model.RDFNode;
import com.hp.hpl.jena.rdf.model.Statement;
import com.hp.hpl.jena.rdf.model.StmtIterator;

import com.hp.hpl.jena.vocabulary.VCARD;


////////////////////////////////////////////////////////////////////////////////

public class RDFImporter extends Object
{
    static String personURI    = "http://somewhere/JohnSmith";
    static String fullName     = "John Smith";

    private static Namespace data = null;

    private static void addStatements( Model model, String args[] )
    {
        long prev = model.size();

        for ( int i = 0; i < args.length; i++ )
        {
            model.read( args[i] );

            long size = model.size();
            System.out.println( "Added " + (size - prev) + " statements from " + args[i] + "." );
            prev = size;
        }

        System.out.println( "Final model contains " + prev + " statements." );
    }

    private static void print( RDFNode r )
    {
        System.out.print( r.toString() );

        if ( r instanceof Resource )
            data.createChildren( r.toString() );
    }

    private static void print( Statement st )
    {
        print( st.getSubject() );
        System.out.print( ", " );
        print( st.getPredicate() );
        System.out.print( ", " );
        print( st.getObject() );
        System.out.print( "\n" );
    }

    private static void writeScript( Model model )
    {
        StmtIterator iter = model.listStatements();

        while ( iter.hasNext() )
            print( iter.nextStatement() );
    }

    public static void main( String args[] )
    {
        data = new Namespace();

        // Create an empty model.
        Model model = ModelFactory.createDefaultModel();

        // create the resource
        Resource johnSmith = model.createResource( personURI );

        // add the property
        johnSmith.addProperty( VCARD.FN, fullName );

        // Import statements from argument URLs.
        addStatements( model, args );

        writeScript( model );

        data.writeScript( null );
   }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
