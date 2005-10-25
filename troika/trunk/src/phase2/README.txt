Type-specific libraries which manage a collection of elements of that type:

                  passed by
    -----------------------------
    p2_atom       reference
    p2_error      value
    p2_primitive  reference
    p2_type       value

    NOT p2_id     value


Aliases for (void *):

    P2_function_ptr

Utilities:

    P2_array
    P2_bunch
    P2_hash_table
    P2_set
    P2_term

C++ classes:

    P2_client
    P2_dataset


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
--[.] Phase2 command-line client
--[.] Phase2 GUI


TODO :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


/*
["] factory method

[N] Consider sorting DOM data sets.
*/


#define P2_ERROR_NAME   "P2_error"

/** The type which identifies a P2_error. */
P2_type P2_error_type;

#define P2_PRIMITIVE_NAME    "PrimitiveReference"

P2_type P2_primitive_type;

#define P2_ID_NAME       "P2_id"




    P2_error_type = P2_register_type(P2_ERROR_NAME, 0, 0, 0, 0);

    /*
    if (!all_errors_)
        all_errors_ = sequence__new((void *) P2_error_type, (void *) total_errors_);
    else
        all_errors_ = sequence__cat(all_errors_, sequence__new((void *) P2_error_type, (void *) total_errors_);
    */

    P2_primitive_type = P2_register_type(P2_PRIMITIVE_NAME,
        (ENCODE_FORMAT) P2_primitive__encode,
        (DECODE_FORMAT) P2_primitive__decode,
        //(DESTROY_FORMAT) P2_primitive__delete,
        (DESTROY_FORMAT) 0,
        (CLONE_FORMAT) 0);




