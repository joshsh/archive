// Reference-hole-friendly; de-/serializes without restrictions.

/*
color < bitmap
char < key_map
cpp_primitive < expression
*/

#ifndef EXPRESSION_ITF_H
#define EXPRESSION_ITF_H

#include "../snpl.h"  // ^ BREADTH_FIRST_SERIALIZATION
using namespace std;



void SNPL_expression_serializer(void *, SNPL_associative_array *, SNPL_associative_array *, ofstream &);
void *SNPL_expression_deserializing_constructor(ifstream &);
void SNPL_expression_destructor(void *);
void SNPL_expression_preserializer(void *, SNPL_array *);



void SNPL_expression_preserializer(
  void *p,
  SNPL_array *data_stack)
{
  #ifdef EXPRESSION_DEBUG
    cout << "<> void SNPL_expression_preserializer(void *, SNPL_array *)" << endl;
    cout.flush();
  #endif
  
  SNPL_expression *exp = (SNPL_expression *) p;
  char *s = exp->body;
  SNPL_primitive_shell *shell;
  int params;
  
  // This expression holds nothing but a single reference to a SNPL_primitive_shell.
  if (*s == (char) REFERENCE) {

    shell = (SNPL_primitive_shell *) *((void **) ++s);
    params = shell->prim->parameters;

    for (int i=params-shell->filled+1; i<=params; i++) {
      #ifdef BREADTH_FIRST_SERIALIZATION
        data_stack->enqueue(shell->arguments[i]);
      #else
        data_stack->push(shell->arguments[i]);
      #endif      
    }

  } else {

    char *terminus = exp->body + exp->length;
    while (s < terminus)
    {
      if ((int) *s == REFERENCE) {
        #ifdef BREADTH_FIRST_SERIALIZATION
          data_stack->enqueue(*((void **) ++s));
        #else
          data_stack->push(*((void **) ++s));
        #endif 
        s += SIZEOF_POINTER;
      } else
        s++;  // Get the cursor past this token.
    }
  }
}



void SNPL_expression_serializer(
  void *p,
  SNPL_associative_array *item_indices,
  SNPL_associative_array *type_indices,
  ofstream &out)
{
  #ifdef EXPRESSION_DEBUG
    cout << "<> void SNPL_expression_serializer(void *, ofstream &)" << endl;
    cout.flush();
  #endif

  SNPL_expression *exp = (SNPL_expression *) p;
  char *s = exp->body;
  void *pointer;
  SNPL_primitive_shell *shell;
  int params;

  // This expression holds nothing but a single reference to a SNPL_primitive_shell.
  if (*s == (char) REFERENCE)
  {
    //XXX cout << "Serializing primitive shell." << endl; cout.flush();
    out << "S ";
    shell = (SNPL_primitive_shell *) *((void **) ++s);
    params = shell->prim->parameters;
    pointer = (void *) shell->prim;
    out << (int) type_indices->lookup((void *) SNPL_find_type(pointer)) - 1
        << "."
        << (int) item_indices->lookup(pointer) - 1
	<< " " << shell->filled << " { ";
    for (int i=params-shell->filled+1; i<=params; i++) {
      pointer = shell->arguments[i];
      out << (int) type_indices->lookup((void *) SNPL_find_type(pointer)) - 1
          << "."
          << (int) item_indices->lookup(pointer) - 1
	  << " ";      
    }
    out << "}";  

  } else {
    //XXX cout << "Serializing lambda expression." << endl;
    //XXX cout << "length = " << exp->length << ", bound_variables = " << exp->bound_variables << endl; cout.flush();
  
    out << "E " << exp->length << " " << exp->bound_variables;
  
    char *terminus = exp->body + exp->length;
    while (s < terminus)
    {
      switch ((int) *s)
      {
        case OPEN_SUBTERM:
	  out << " (";
          s++;  // Get the cursor past this token.
          break;

        case CLOSE_SUBTERM:
          out << " )";
          s++;  // Get the cursor past this token.
          break;

        case SELF_REFERENCE:
          out << " @";
          s++;  // Get the cursor past this token.
          break;

        case REFERENCE:
          pointer = *((void **) ++s);
          out << " "
	      << (int) type_indices->lookup((void *) SNPL_find_type(pointer)) - 1
              << "."
              << (int) item_indices->lookup(pointer) - 1;
          s += SIZEOF_POINTER;  // Get the cursor past the item reference.
          break;

        default:
          out << " " << (FIRST_PARAMETER + (exp->bound_variables-1) - (int) *s);
          s++;  // Get the cursor past this token.
      }
    }
  }
  //XXX cout << "Done serializing expression." << endl;
}



void *SNPL_expression_deserializing_constructor(ifstream &in)
{
  #ifdef EXPRESSION_DEBUG
    cout << "<> void *SNPL_expression_deserializing_constructor(ifstream &)" << endl;
    cout.flush();
  #endif
  ///*
  void *r;
  char exp_or_shell;
  int length, bound_variables, filled;
  char *s = new char[10];
  
  in >> exp_or_shell;
  if (exp_or_shell == 'S') {
    //XXX cout << "Deserializing primitive_shell." << endl; cout.flush();
    in >> s >> filled;
    SNPL_primitive_shell *shell
      = (SNPL_primitive_shell *) malloc(sizeof(SNPL_primitive_shell));
    shell->prim = (SNPL_cpp_primitive *) SNPL_decode_item_reference(s);
    shell->filled = filled;
    shell->arguments = (void **) malloc((1+shell->prim->parameters)*sizeof(void *));    
    in >> s;  // (get past opening bracket)
    //SNPL_array *arg_array = new SNPL_array(args, 0);
    for (int i=shell->prim->parameters+1-filled; i<=shell->prim->parameters; i++) {
      in >> s;
      shell->arguments[i] = SNPL_decode_item_reference(s);
      if (shell->arguments[i] == NULL) {
        SNPL_add_reference_hole(s, shell->arguments + i);
        //XXX cout << "Encountered reference hole." << endl; cout.flush();
      }    
      //arg_array->push();
    }
    in >> s;  // (get past closing bracket)

    SNPL_expression *e
      = (SNPL_expression *) malloc(sizeof(SNPL_expression));
    e->body = (char *) malloc(5);
    e->length = 5;
    e->bound_variables = shell->prim->parameters - shell->filled;
    char *s = e->body;
    *s = (char) REFERENCE;
    s++;
    char *s2 = (char *) &shell;
    for (int i=0; i<4; i++)
    {
      *s = *s2;
      s++; s2++;
    }
    r = (void *) e;    
      
  } else {
  
    // Note: the "length" attribute is not actually used in re-creating the
    // expression.
    in >> length >> bound_variables;
  
    SNPL_expanding_string *exp_str = new SNPL_expanding_string(0,0);
    //exp_str->append((char) OPEN_SUBTERM);
    //in >> s;  // first opening parenthesis
    int paren_depth = 0, param;
    void *reference;
    
    do {
    
      in >> s;
      
      switch ((int) *s) {
        case '(':
//XXXcout << "OPEN_SUBTERM" << endl;
          exp_str->append((char) OPEN_SUBTERM);
          paren_depth++;
          break;
        case ')':
//XXXcout << "CLOSE_SUBTERM" << endl;
          exp_str->append((char) CLOSE_SUBTERM);
          paren_depth--;
          break;
        case '@':
//XXXcout << "SELF_REFERENCE" << endl;
          exp_str->append((char) SELF_REFERENCE);
	  break;
      // Note: does not check whether the reference is to an actual item.
        default:
	  if (strchr(s,'.') != NULL) {
//XXXcout << "REFERENCE" << endl;
            reference = SNPL_decode_item_reference(s);
	    if (reference == NULL) {
	      exp_str->append((char) REFERENCE_HOLE);
	      exp_str->append((void *) strdup(s));
	    } else {
              exp_str->append((char) REFERENCE);
	      exp_str->append(reference);
	    }
          } else {
//XXXcout << "[default]" << endl;
            param = ((int) *s - 48);
//XXXcout << "param = " << param << endl;
            exp_str->append((char) (FIRST_PARAMETER + (bound_variables-(param+1))));
	  }
      }

    } while (paren_depth);
    
    SNPL_expression *exp = (SNPL_expression *) malloc(sizeof(SNPL_expression));
    exp->body = exp_str->copy();
    exp->length = exp_str->size();
    exp->bound_variables = bound_variables;
    delete exp_str;
    
    // Look for reference holes
    char *p = exp->body, *s2, *terminus = exp->body+exp->length;
    while (p < terminus) {
      if (*p == (char) REFERENCE_HOLE) {
        *(p++) = (char) REFERENCE;
	s2 = (char *) *((void **) p);
	SNPL_add_reference_hole(s2, (void **) p);
	delete s2;
        p += SIZEOF_POINTER;
      } else if (*p == (char) REFERENCE)
        p += (1+SIZEOF_POINTER);
      else
        p++;
    } 
    
    r = (void *) exp;  
  } 
//XXX cout << "Done deserializing expression." << endl; cout.flush();  
  delete s;
  return r;
}



void SNPL_expression_destructor(void *p)
{
  #ifdef EXPRESSION_DEBUG
    cout << "<> void SNPL_expression_destructor(void *)" << endl;
    cout.flush();
  #endif
  
  SNPL_expression_destructor((SNPL_expression *) p);
}

#endif

