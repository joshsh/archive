
[U3]
--[.] Phase2 API
--[.] Phase2 command-line interface
--[.] Phase2 GUI


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//   [unrelated] Kernighan ^ 1974 C tutorial: http://www.lysator.liu.se/c/bwk-tutor.html
// g++ -c p2_client.cpp -I../../gsoap-linux-2.7

["] factory method

[N] Consider sorting DOM data sets.


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

[.] if A and B are marked and (A, B, C), then C is marked.

    marked(A) ^ marked(B) ^ is_edge(A, B, C) ==> marked(C)

[P] mark a collection (term or set)
--[P] add all triggers to the mark queue / bunch:

      for each trigger
          pre_mark(trigger) ;

--[P] 

pre_mark(node)
{
    if (type(node) < 0)
        return ;

    else
    {
        [if using association]
            add node to "mark" queue ;
        [else]
            if (is_collection(node))
                add node to "mark" queue ;
            else
                reverse sign of type ;
    }
}

mark(node)
{
    if (is_collection(node))
    {
        for each child node
            pre_mark(child) ;
    }

    for each outbound edge (key, target)
    {
        if (!marked(target))
        {
            if (marked(key))
                pre_mark(target) ;
            else
                set_trigger(key, target) ;
        }
    }

    reverse sign of type ;
}

set_trigger(key, target)
{

}


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

[*] Args to register a regular expression and bind it to a type:
--[.] regular expression for recognition
--[.] p2_type

[*] Regular expressions are matched in the order in which they were registered.



