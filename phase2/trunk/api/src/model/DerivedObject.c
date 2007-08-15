/*
    _get :foaf http://xmlns.com/foaf/0.1/;

    _put . myfile.p2;

    _model world := + foaf rdf

    :world = :foaf + :rdf;

    :mine -> :world - :foaf.

    .mine := .world - .foaf;

    graph.triples

    graph:triples

    graph/triples

    NOT -> V (S K) K.


<Namespace> root:data:graph:triples:properties :

<Namespace> root.data.graph.triples.properties :


    
*/



struct DerivedObject
{
    Object *cached;
    Object *derivation;
};