Type-specific libraries which manage a collection of elements of that type:

                  passed by
    -----------------------------
    p2_atom       reference
    p2_error      value
    p2_primitive  reference
    p2_type       value

    NOT p2_id     value


Aliases for (void *):

    p2_function_ptr

Utilities:

    p2_array
    p2_bunch
    p2_hash_table
    p2_set
    p2_term

C++ classes:

    p2_client
    p2_dataset


Libraries which require an init() and end():

    p2
    p2_atom
    p2_client (!)
    p2_command (!)
    p2_error
    p2_primitive
    p2_syntax
    p2_type

No init() / end():

    p2_dataset (C++)
    p2_id
    utilities (p2_array, p2_bunch, p2_hash_table, p2_set, p2_term)

Oddballs:

    p2_flags.h



::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

[U3]
--[.] Phase2 API
--[.] Phase2 command-line interface
--[.] Phase2 GUI


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//   [unrelated] Kernighan ^ 1974 C tutorial: http://www.lysator.liu.se/c/bwk-tutor.html
// g++ -c p2_client.cpp -I../../gsoap-linux-2.7


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


TODO :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


/*
["] factory method

[N] Consider sorting DOM data sets.
*/


#define P2_ERROR_NAME   "p2_error"

/** The type which identifies a p2_error. */
p2_type P2_error_type;

#define P2_PRIMITIVE_NAME    "PrimitiveReference"

p2_type P2_primitive_type;

#define P2_ID_NAME       "p2_id"




    p2_error_type = p2_register_type(P2_ERROR_NAME, 0, 0, 0, 0);

    /*
    if (!all_errors_)
        all_errors_ = sequence__new((void *) p2_error_type, (void *) total_errors_);
    else
        all_errors_ = sequence__cat(all_errors_, sequence__new((void *) p2_error_type, (void *) total_errors_);
    */

    p2_primitive_type = p2_register_type(P2_PRIMITIVE_NAME,
        (ENCODE_FORMAT) p2_primitive__encode,
        (DECODE_FORMAT) p2_primitive__decode,
        //(DESTROY_FORMAT) p2_primitive__delete,
        (DESTROY_FORMAT) 0,
        (CLONE_FORMAT) 0);




