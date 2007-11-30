Sorry if this has been asked before, but are there any plans to implement SPARQL/Update in Sesame?  I know the spec is in its early stages, but it's already very useful.  I'd like to 


# Content type, data and format are good.
# Code comment appears at or near the end of the document.
curl -vH "Accept: text/rdf+n3" http://localhost:8182/resource/foo
curl -vH "Accept: text/plain" http://localhost:8182/resource/foo
curl -vH "Accept: application/rdf+xml" http://localhost:8182/resource/foo
curl -vH "Accept: application/x-trig" http://localhost:8182/resource/foo
curl -vH "Accept: application/trix" http://localhost:8182/resource/foo
curl -vH "Accept: application/x-turtle" http://localhost:8182/resource/foo

# For now, the default format is RDF/XML.
curl -v http://localhost:8182/resource/foo

# [DELETE]
# DELETE removes all statements in the given context.

curl -X DELETE http://localhost:8182/resource/foo

# [POST and PUT]
# You can only PUT or POST RDF representations.
# Any context information (e.g. in TriX or TriG representations) will be thrown
# away, as all statements are placed in a graph with a context URI equal to the
# PUT or POST URI.
# POST adds all statements in the representation to the given context.
# PUT additionally deletes any pre-existing statements before adding the new ones.

curl -H "Content-type: application/x-turtle" http://localhost:8182/resource/foo -d "\
	<http://localhost:8182/resource/foo#mary> a <http://xmlns.com/foaf/0.1/Person> ;\
        <http://www.w3.org/2000/01/rdf-schema#label> \"Mary Q. Contrary\" ;\
        <http://www.w3.org/2000/01/rdf-schema#comment> \"some chick\" ."

curl -H "Content-type: application/x-turtle" http://localhost:8182/resource/quux -d "\
	<http://localhost:8182/resource/quux> a <http://xmlns.com/foaf/0.1/Person> ;\
        <http://www.w3.org/2000/01/rdf-schema#label> \"Quux?\" ;\
        <http://www.w3.org/2000/01/rdf-schema#comment> \"oh no, not again\" ."
        

curl http://localhost:8182/search?kwd=foo
curl http://localhost:8182/users/josh


*  publishing ontologies
Y  content-negotiated hash URIs


A client can avoid content negotiation by requesting the URI for the desired representation.


Each URI has a separate representation, 



*) Content negotiation is based on Accept headers and [language preferences]

*) For now, the SPARQL endpoint bypasses content negotiation

*) SW Sitemaps file for RDFWiki
*) dump file(s)



# Content-negotiated URIs.
http://sandbox.fortytwo.net/resource/jshinavier/scripts/myprog

# Non-content-negotiated data URIs.
# Use explicit base URIs for each supported MIME type (we can't put them in the
# same URI space as the resource URI and distinguish them by local syntax,
# because the local syntax is unconstrained).
# No file extensions in the URI (?).  However, the returned data should have an appropriate file extension (?)
http://sandbox.fortytwo.net/xhtml/en/jshinavier/scripts/myprog
http://sandbox.fortytwo.net/rdfxml/jshinavier/scripts/myprog
http://sandbox.fortytwo.net/ntriples/jshinavier/scripts/myprog
http://sandbox.fortytwo.net/n3/jshinavier/scripts/myprog
http://sandbox.fortytwo.net/turtle/jshinavier/scripts/myprog
http://sandbox.fortytwo.net/trix/jshinavier/scripts/myprog
http://sandbox.fortytwo.net/trig/jshinavier/scripts/myprog

# SPARQL URIs. Don't redirect to these, because we'd rather hide the underlying
# query (and we shouldn't results in multiple MIME types from the same URI).
# Do not support SPARQL Update here for now (because it would allow a client to
# easily wipe out the entire wiki).
http://sandbox.fortytwo.net/sparql?default-graph-uri=http%3a//sandbox.fortytwo.net/page/jshinavier/scripts/myprog&query=DESCRIBE+%3Chttp%3a//sandbox.fortytwo.net/page/jshinavier/scripts/myprog%3E





Y  "thorough" base URIs -- host/mediaType/language/charset/encoding/localURI

http://sandbox.fortytwo.net/xhtml/en/utf_8/gzip/jshinavier/scripts/myprog
http://sandbox.fortytwo.net/xhtml/fr/iso_8859_1/identity/jshinavier/scripts/myprog




Note: the DESCRIBE query should probably be a wildcard so that it catches hash URIs and blank nodes, as well


http://dbpedia.openlinksw.com:8890/sparql?default-graph-uri=http%3A%2F%2Fdbpedia.org&query=DESCRIBE+%3Chttp%3A%2F%2Fdbpedia.org%2Fresource%2FThe_Beatles%3E

