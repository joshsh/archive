/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import java.io.InputStream;

import net.fortytwo.ripple.flow.Collector;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.rio.RDFFormat;
import junit.framework.Assert;
import junit.framework.TestCase;

public class RdfUtilsTest extends TestCase
{
    public void testReadFromInputStream() throws Exception
    {
        InputStream is = RdfUtilsTest.class.getResourceAsStream( "rdfUtilsReadTest.ttl" );
        RdfCollector allRdf = new RdfCollector();
        SesameInputAdapter sc = new SesameInputAdapter( allRdf );
        String baseURI = "";
        RdfUtils.read( is, sc, baseURI, RDFFormat.TURTLE );
        is.close();

        Collector<Statement, RippleException> allStatements = new Collector<Statement, RippleException>();
        Collector<Namespace, RippleException> allNamespaces = new Collector<Namespace, RippleException>();
        allRdf.statementSource().writeTo( allStatements );
        allRdf.namespaceSource().writeTo( allNamespaces );

        Assert.assertEquals( 4, allStatements.size() );
        Assert.assertEquals( 3, allNamespaces.size() );
    }
}

