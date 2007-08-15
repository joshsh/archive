/**

  This file is an extension of p2.cpp.  It defines some demo primitives
  as well as a function to build actual P2_primitives out of them and add them to a
  P2_data_set.

*/

#include "../p2_dataset.h"
#include "../sk/sk.h"

#define PRIM_ALLOC	(struct P2_primitive *) malloc(sizeof(struct P2_primitive))



int *int__add(int *a, int *b)
{
    int *c = (int *) malloc(sizeof(int));
    *c = *a + *b;
    return c;
}
int *int__subtract(int *a, int *b)
{
    int *c = (int *) malloc(sizeof(int));
    *c = *a - *b;
    return c;
}
int *int__multiply(int *a, int *b)
{
    int *c = (int *) malloc(sizeof(int));
    *c = *a * *b;
    return c;
}
int *int__divide(int *a, int *b)
{
    int *c = (int *) malloc(sizeof(int));
    *c = *a / *b;
    return c;
}

double *double__add(double *a, double *b)
{
    double *c = (double *) malloc(sizeof(double));
    *c = *a + *b;
    return c;
}
double *double__subtract(double *a, double *b)
{
    double *c = (double *) malloc(sizeof(double));
    *c = *a - *b;
    return c;
}
double *double__multiply(double *a, double *b)
{
    double *c = (double *) malloc(sizeof(double));
    *c = *a * *b;
    return c;
}
double *double__divide(double *a, double *b)
{
    double *c = (double *) malloc(sizeof(double));
    *c = *a / *b;
    return c;
}

char *system_call(char *s)
{
    system(s);
    return strdup("");
}



// temporary
void construct_primitives()
{
    P2_primitive *prim;
    //SEQUENCE *term;
    //P2_data_set *dataset = active_dataset_;
///*
    prim = PRIM_ALLOC;
    prim->name = strdup("int__add");
    prim->value = (P2_function_ptr *) int__add;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_int_type;
    prim->parameter_types[1] = (void *) P2_int_type;
    prim->parameter_names = NULL;  // Minimized because not used.  
    prim->transparency = NULL;  // Minimized because not used. 
    prim->return_type = (void *) P2_int_type;
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);
    
    prim = PRIM_ALLOC;
    prim->name = strdup("int__subtract");
    prim->value = (P2_function_ptr *) int__subtract;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_int_type;
    prim->parameter_types[1] = (void *) P2_int_type;
    prim->parameter_names = NULL;  // Minimized because not used.  
    prim->transparency = NULL;  // Minimized because not used. 
    prim->return_type = (void *) P2_int_type;
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);
    
    prim = PRIM_ALLOC;
    prim->name = strdup("int__multiply");
    prim->value = (P2_function_ptr *) int__multiply;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_int_type;
    prim->parameter_types[1] = (void *) P2_int_type;
    prim->parameter_names = NULL;  // Minimized because not used.  
    prim->transparency = NULL;  // Minimized because not used. 
    prim->return_type = (void *) P2_int_type;
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);
        
    prim = PRIM_ALLOC;
    prim->name = strdup("int__divide");
    prim->value = (P2_function_ptr *) int__divide;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_int_type;
    prim->parameter_types[1] = (void *) P2_int_type;
    prim->parameter_names = NULL;  // Minimized because not used.
    prim->transparency = NULL;  // Minimized because not used.
    prim->return_type = (void *) P2_int_type;   
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);

    prim = PRIM_ALLOC;
    prim->name = strdup("double__add");
    prim->value = (P2_function_ptr *) double__add;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_double_type;
    prim->parameter_types[1] = (void *) P2_double_type;
    prim->parameter_names = NULL;  // Minimized because not used.  
    prim->transparency = NULL;  // Minimized because not used. 
    prim->return_type = (void *) P2_double_type;
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);

    prim = PRIM_ALLOC;
    prim->name = strdup("double__subtract");
    prim->value = (P2_function_ptr *) double__subtract;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_double_type;
    prim->parameter_types[1] = (void *) P2_double_type;
    prim->parameter_names = NULL;  // Minimized because not used.  
    prim->transparency = NULL;  // Minimized because not used. 
    prim->return_type = (void *) P2_double_type;
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);
    
    prim = PRIM_ALLOC;
    prim->name = strdup("double__multiply");
    prim->value = (P2_function_ptr *) double__multiply;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_double_type;
    prim->parameter_types[1] = (void *) P2_double_type;
    prim->parameter_names = NULL;  // Minimized because not used.  
    prim->transparency = NULL;  // Minimized because not used. 
    prim->return_type = (void *) P2_double_type;
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);
        
    prim = PRIM_ALLOC;
    prim->name = strdup("double__divide");
    prim->value = (P2_function_ptr *) double__divide;
    prim->parameters = 2;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 2);
    prim->parameter_types[0] = (void *) P2_double_type;
    prim->parameter_types[1] = (void *) P2_double_type;
    prim->parameter_names = NULL;  // Minimized because not used.
    prim->transparency = NULL;  // Minimized because not used.
    prim->return_type = (void *) P2_double_type;   
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);
    
    prim = PRIM_ALLOC;
    prim->name = strdup("system");
    prim->value = (P2_function_ptr *) system_call;
    prim->parameters = 1;
    prim->parameter_types = (void **) malloc(sizeof(void *) * 1);
    prim->parameter_types[0] = (void *) P2_cstring_type;
    prim->parameter_names = NULL;  // Minimized because not used.  
    prim->transparency = NULL;  // Minimized because not used.  
    prim->return_type = (void *) P2_cstring_type;
    P2_register_primitive(prim);
    //term = sequence__new((void *) P2_primitive_type, (void *) prim);
    //dataset->add(prim->name, term);
//*/

    // Add the S and K combinators to the data set.
    //dataset->add("S", sequence__new((void *) SK_S_type, NULL));
    //dataset->add("K", sequence__new((void *) SK_K_type, NULL));
}



