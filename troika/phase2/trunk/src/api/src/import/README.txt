function name, return type
parameter names, parameter types
referential transparency w.r.t. each parameter


## Template header file ########################################################


/** \param a  $transparent */
int add(int a, int b);

struct _foo
{
    int p, q;
};

struct _foo *bar(int x);


## Generated header file #######################################################


extern "C"
{


void *p2_imported__add(void **args)
{
    // Return value is passed by value --> create a container.
    int *ret = malloc( sizeof( int ));

    *ret = add(
        *(( int* ) args[0] ),
        *(( int* ) args[1] ));

    return ( void* ) ret;
}


void *p2_imported__bar(void **args)
{
    // Return value is a pointer type.
    struct _foo *ret;

    ret = bar(
        *(( int* ) args[0] );

    return ( void* ) ret;
}


}  // extern "C"




