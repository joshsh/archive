/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfImporter;
import net.fortytwo.ripple.io.SesameAdapter;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.RdfUtils;

import org.openrdf.model.URI;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.RDFS;
import org.openrdf.rio.RDFFormat;

public class UriTest extends RippleTestCase
{
	private enum TestType
	{
		ABSID( "AbsId" ),
		ABSRF( "AbsRf" ),
		ABS2REL( "Abs2Rel" ),
		DECOMP( "Decomp" ),
		NORMCASE( "NormCase" ),
		NORMESC( "NormEsc" ),
		NORMPATH( "NormPath" ),
		INVRF( "InvRf" ),
		RELATIVE( "Relative" ),
		RELRF( "RelRf" ),
		REL2ABS( "Rel2Abs" );

		String name;

		TestType( final String n )
		{
			name = n;
		}

		public String getName()
		{
			return name;
		}

		public static TestType find( final String name )
			throws RippleException
		{
			for ( TestType type : TestType.values() )
				if ( type.name.equals( name ) )
					return type;
			throw new RippleException( "no such TestType: " + name );
		}
	}

	static RdfValue
		BASE,
		COMMENT,
		FRAG,
		LABEL,
		PATH,
		PORT,
		QUERY,
		REG,
		SCHEME,
		TEST,
		TYPE,
		URITEST,
		USER;

	static void initialize()
		throws Exception
	{
		String uriNs = "http://id.ninebynine.org/wip/2004/uritest/";
		ModelConnection mc = getTestModel().getConnection( "for GrahamKlyneCasesTest" );

		COMMENT = new RdfValue( RDFS.COMMENT );
		LABEL = new RdfValue( RDFS.LABEL );
		TYPE = new RdfValue( RDF.TYPE );

		BASE = new RdfValue( mc.createUri( uriNs + "base" ) );
		FRAG = new RdfValue( mc.createUri( uriNs + "frag" ) );
		PATH = new RdfValue( mc.createUri( uriNs + "path" ) );
		PORT = new RdfValue( mc.createUri( uriNs + "port" ) );
		QUERY = new RdfValue( mc.createUri( uriNs + "query" ) );
		REG = new RdfValue( mc.createUri( uriNs + "reg" ) );
		SCHEME = new RdfValue( mc.createUri( uriNs + "scheme" ) );
		TEST = new RdfValue( mc.createUri( uriNs + "test" ) );
		URITEST = new RdfValue( mc.createUri( uriNs + "UriTest" ) );
		USER = new RdfValue( mc.createUri( uriNs + "user" ) );

		mc.close();
	}

	static String strVal( RdfValue subj, RdfValue pred, ModelConnection mc )
		throws Exception
	{
		RdfValue obj = mc.findSingleObject( subj, pred );
		if ( null == obj )
			return null;
		else
			return obj.toString();
	}

	private class UriTestCase
	{
		public TestType type;

		public String
			base,
			comment,
			frag,
			label,
			path,
			port,
			query,
			reg,
			scheme,
			user;

		public UriTestCase( final RdfValue r, final ModelConnection mc )
			throws Exception
		{
			base = strVal( r, BASE, mc );
			comment = strVal( r, COMMENT, mc );
			frag = strVal( r, FRAG, mc );
			label = strVal( r, LABEL, mc );
			path = strVal( r, PATH, mc );
			port = strVal( r, PORT, mc );
			query = strVal( r, QUERY, mc );
			reg = strVal( r, REG, mc );
			scheme = strVal( r, SCHEME, mc );
			user = strVal( r, USER, mc );

			type = TestType.find( ((URI) mc.findSingleObject( r, TEST ).toRdf( mc ).getRdfValue()).getLocalName() );
		}

		public void test( final ModelConnection mc )
			throws Exception
		{
			System.out.println( "URI test " + label
				+ ( null == comment ? "" : " (" + comment + ")" ) );
			String fakeBase = "http://example.org/";
			URI uri;

			switch ( type )
			{
//				case ABSID:
				case ABSRF:
					assertFalse( null == base );
					uri = mc.createUri( base );
					break;
// 				case ABS2REL:
// 				case DECOMP:
//				case INVRF:  // ?
// 				case NORMCASE:
// 				case NORMESC:
// 				case NORMPATH:
// 				case RELATIVE:
				case RELRF:
					assertFalse( null == base );
					uri = mc.createUri( fakeBase + base );
					break;
//				case REL2ABS:
				default:
System.out.println( "unhandled test case!" );
			}
		}
	}

	private class GrahamKlyneCasesTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "for GrahamKlyneCasesTest" );

			// See: http://lists.w3.org/Archives/Public/uri/2006Feb/0003.html
			InputStream is = UriTest.class.getResourceAsStream( "UriTest.n3" );

			RdfImporter importer = new RdfImporter( mc );
			SesameAdapter sc = new SesameAdapter( importer );
			RdfUtils.read( is, sc, "", RDFFormat.N3 );

			Collector<RdfValue> cases = new Collector<RdfValue>();
			mc.divide( URITEST, TYPE, cases );

			Iterator<RdfValue> iter = cases.iterator();
			while ( iter.hasNext() )
			{
				RdfValue caseRdf = iter.next();
//System.out.println( "test case: " + caseRdf );
				( new UriTestCase( caseRdf, mc ) ).test( mc );
			}

			is.close();
			mc.close();
		}
	}

	private class UriNamespaceTest extends TestRunnable
	{
		void nsTest( final String uri,
					final String ns,
					final String localName,
					final ModelConnection mc )
			throws Exception
		{
			URI uriCreated = mc.createUri( uri );
			String nsCreated = uriCreated.getNamespace();
			String localNameCreated = uriCreated.getLocalName();

			assertEquals( uriCreated.toString(), uri );
			assertEquals( nsCreated, ns );
			assertEquals( localNameCreated, localName );
		}

		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "for UriNamespaceTest" );

			InputStream is = UriTest.class.getResourceAsStream( "UriNamespaceTest.txt" );

			BufferedReader reader = new BufferedReader(
				new InputStreamReader( is ) );
			int lineno = 0;

			// Break out when end of stream is reached.
			while ( true )
			{
				String line = reader.readLine();
				lineno++;

				if ( null == line )
					break;
	
				line = line.trim();
	
				if ( !line.startsWith( "#" ) && !line.equals( "" ) )
				{
					String[] args = line.split( "\t" );
					if ( args.length != 3 )
						throw new RippleException( "wrong number of aguments on line " + lineno );
					nsTest( args[0], args[1], args[2], mc );
				}
			}
	
			is.close();
			mc.close();
		}
	}

	public void runTests()
		throws Exception
	{
		initialize();

		testSynchronous( new UriNamespaceTest() );
		testSynchronous( new GrahamKlyneCasesTest() );
	}
}

// kate: tab-width 4
