package net.fortytwo.linkeddata;

import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.flow.Collector;
import net.fortytwo.ripple.util.UriMap;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.vocabulary.RDF;

public class LinkedDataSailTest extends RippleTestCase
{
	private class DereferencerTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Model model = getTestModel();
			UriMap uriMap = getTestUrlFactory();
			
			// This is an example where HttpDereferencer fails by requesting the
			// full URI of a resource (rather than stripping off the local part).
			// Here, we define a mapping to a local file, so dereferencing
			// succeeds.
			uriMap.put( "http://www.holygoat.co.uk/owl/redwood/0.1/tags/Tagging",
					LinkedDataSailTest.class.getResource( "tags.owl" ).toString() );
			
			ModelConnection mc = model.getConnection( "" );
			RippleValue tagTagging = new RdfValue(
					mc.createUri( "http://www.holygoat.co.uk/owl/redwood/0.1/tags/Tagging" ) );
			RippleValue rdfType = new RdfValue( RDF.TYPE );
			
			Collector<RippleValue, RippleException> results = new Collector<RippleValue, RippleException>();
			
			mc.multiply( tagTagging, rdfType, results, false );
			assertEquals( 1, results.size() );
			
			mc.close();
		}
	}
	
	@Override
	public void runTests() throws Exception
	{
		this.testAsynchronous( new DereferencerTest() );
	}
}
