package net.fortytwo.rdfwiki;

import org.restlet.Application;
import org.restlet.Context;
import org.restlet.Redirector;
import org.restlet.Restlet;
import org.restlet.Route;
import org.restlet.Router;

public class RootApplication extends Application
{
	@Override
	public Restlet createRoot() {
        // Create a root router
        Router router = new Router(getContext());

		// Create an application
		Application searchApp = new Application(router.getContext()) {
		    @Override
		    public Restlet createRoot() {
		        // Create a Redirector to Google search service
		        String target =
		           "http://www.google.com/search?q=site:mysite.org+{keywords}";
		        return new Redirector(getContext(), target,
		                Redirector.MODE_CLIENT_TEMPORARY);
		    }
		};
		
		Route route = router.attach("/search", searchApp);
		route.extractQuery("keywords", "kwd", true);
		
/*
        // Attach a guard to secure access to the directory
        Guard guard = new Guard(getContext(),
                ChallengeScheme.HTTP_BASIC, "Restlet tutorial");
        guard.getSecrets().put("scott", "tiger".toCharArray());
        router.attach("/docs/", guard);

        // Create a directory able to expose a hierarchy of files
        Directory directory = new Directory(getContext(), ROOT_URI);
        guard.setNext(directory);
*/
		Context context = getContext();
		Restlet nonInfoRestlet = new Redirector( context, "http://localhost:8182/representation/{rr}", Redirector.MODE_CLIENT_SEE_OTHER );
		
        // Attach the handlers to the root router
		router.attach( "/resource/", nonInfoRestlet );
        router.attach( "/representation/", InformationResource.class );

        // Return the root router
        return router;
    }
}

