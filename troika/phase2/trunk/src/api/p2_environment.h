#include "util/p2_dictionary.h"


/* Special, global data types. */
p2_type *p2_namespace_t;
p2_type *int_t, *double_t, *char_t, *cstring_t;


/** A data structure which is equivalent to a namespace containing uniquely
    named data types, function primitives, and data sets. */
typedef struct _p2_environment
{
    p2_dictionary *types;
    p2_dictionary *primitives;

    p2_dictionary *datasets;

} p2_environment;

/** \return  a new environment */
p2_environment *p2_environment__new();

/** Destroys an environment. */
void p2_environment__delete( p2_environment *env );

p2_type *p2_register_type( p2_environment *env, p2_type *type );
p2_type *p2_resolve_type( p2_environment *env, char *name );
p2_primitive *p2_register_primitive( p2_environment *env, p2_primitive *prim );
p2_primitive *p2_resolve_primitive( p2_environment *env, char *name );
//p2_dataset *p2_register_dataset( p2_environment *env, p2_dataset *dataset );
//p2_dataset *p2_resolve_dataset( p2_environment *env, char *name );
p2_atom *p2_resolve_atom( p2_environment *env, char *dataset_name, char *atom_name );


--------------------------------------------------------------------------------


#include "p2_environment.h"
#include "p2_type.h"
#include "p2_primitive.h"
#include "p2_dataset.h"


p2_environment *p2_environment__new()
{
    p2_environment *env
        = ( p2_environment* ) malloc( sizeof( p2_environment ) );

    // Create data types dictionary.
    env->types = p2_dictionary__new();

    // Create function primitives dictionary.
    env->primitives = p2_dictionary__new();

    // Create data sets dictionary.
    env->datasets = p2_dictionary__new();

    return env;
}


void p2_environment__delete( p2_environment *env )
{
    // Destroy data sets dictionary and all data sets.
    p2_dictionary__for_all( env->datasets, p2_dataset__delete );
    p2_dictionary__delete( env->datasets );

    // Destroy function primitives dictionary and all function primitives.
    p2_dictionary__for_all( env->primitives, p2_primitive__delete );
    p2_dictionary__delete( env->primitives );

    // Destroy data types dictionary and all data types.
    p2_dictionary__for_all( env->types, p2_type__delete );
    p2_dictionary__delete( env->types );

    free( env );
}

