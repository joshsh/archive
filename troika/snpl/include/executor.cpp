/*+

  executor.cpp

  Contains procedures for beta-reduction of SNPL_expressions.

  author: Josh Shinavier
  last edited: 12/17/04

*/

#include "snpl.h"
using namespace std;



/*
  Global variables.
*/
void **parameter_array;  // For passing arguments to primitives.
void *single_item;
SNPL_data_type *expression_type_EXECUTOR_CPP,
	       *cpp_primitive_type_EXECUTOR_CPP;


void SNPL_executor_constructor()
{
//cout << "SNPL_executor_constructor()" << endl;
  cpp_primitive_type_EXECUTOR_CPP = SNPL_reference_type("cpp_primitive");
  expression_type_EXECUTOR_CPP = SNPL_reference_type("expression");
//cout << "cpp_primitive_type: " << (int) cpp_primitive_type_EXECUTOR_CPP << endl;
//cout << "expression_type: " << (int) expression_type_EXECUTOR_CPP << endl;
  parameter_array = (void **) malloc(MAX_PARAMETERS * sizeof(void *));
}



void SNPL_executor_destructor()
{
  free(parameter_array);
}



SNPL_expanding_string *SNPL_reduce_subexpression(void *arg, char *&p, SNPL_expression *exp)
{
//XXX cout << "SNPL_reduce_subexpression..." << endl;
  void *left_item = NULL;
  void *right_item = NULL;
  bool first_item = true;

  char ch = '\0';
  SNPL_expanding_string *s = NULL;
  SNPL_expanding_string *exp_str
    = new SNPL_expanding_string(EXPR_STRING_INITSIZE, EXPR_STRING_EXPANSION);
  exp_str->append((char) OPEN_SUBTERM);
//XXX cout << "\tappending " << OPEN_SUBTERM << endl;
//YYY exp_str->view_bytes();
  p++;  // Get past initial opening paren
  bool new_expression = false;//, nonterminate = false;

  SNPL_cpp_primitive *prim;

  // Stops on close paren or end of string (and does not take the cursor beyond it)
  while ((int) *p != CLOSE_SUBTERM)
  {
    switch ((int) *p)
    {
      case OPEN_SUBTERM:
//XXX cout << "OPEN_SUBTERM" << endl;

	s = SNPL_reduce_subexpression(arg, p, exp);
	if (s == NULL)
	  right_item = single_item;
//XXX cout << "Subterm: size = ";
//XXX if (s != NULL)
//XXX   cout << s->size() << endl;
//XXX else
//XXX   cout << "[NULL]" << endl;

	//first_item = false;
	p++;  // Get past the closing parenthesis
	break;

      case SELF_REFERENCE:
//XXX cout << "SELF_REFERENCE" << endl;
	right_item = exp;
//if (left_item == NULL)
//  right_item = NULL;
	p++;  // Get past the closing parenthesis
	break;

      case REFERENCE:
//XXX cout << "REFERENCE" << endl;
	right_item = *((void **) ++p);
//XXX view_bytes(&right_item,4);
//XXX cout << endl;
//XXX view_bytes(&left_item,4);
//XXX cout << endl;//}
	p += SIZEOF_POINTER;  // Get the cursor past the item reference.
        break;

      // This is the parameter to be resolved by arg.
      case FIRST_PARAMETER:
//XXX cout << "FIRST_PARAMETER" << endl;
	if (arg != NOARG) {
	  right_item = arg;
	} else {
//XXX cout << "first parameter ^ NOARG: " << (int) *p << endl;

	  ch = *p;
	  first_item = false;
	}
	p++;  // Get the cursor past this token.
        break;

      default:
//XXX cout << "default (other parameter)" << endl;
        //j.clear();
	if (arg != NOARG)
          //j += (char) (*p - 1);
	  ch = (char) (*p - 1);
	else {
//XXX cout << "default ^ NOARG: " << (int) *p << endl;
	  //j += *p;
	  ch = *p;
	}
     	p++;  // Get the cursor past this token.
	first_item = false;
        //break;

    }  // end switch statement

//XXX cout << "begin conditionals" << endl; cout.flush();
//XXX cout << "exp_str->size() = " << exp_str->size() << endl;
    if (right_item == NULL) {  // We've encountered a parenthesized sub-term or an unresolvable parameter.
      //if (!j.length()) {
      if ((s == NULL)&&(ch == '\0')) {// We have a right item after all!
//XXX cout << "single item." << endl;
	right_item = single_item;
//XXX view_bytes(&right_item,4);
//XXX cout << endl;
      } else {  // No right item, just a string to add to the expression builder.
	if (left_item != NULL) {  // If we had a left_item, record it before the new partial expression.
//XXX cout << "left_item != NULL" << endl;
          if (left_item == exp) {
//XXX cout << "\tappending " << SELF_REFERENCE << endl; cout.flush();
	    exp_str->append((char) SELF_REFERENCE);
//YYY exp_str->view_bytes();	  
	  } else {
//XXX cout << "appending REFERENCE" << endl; cout.flush();
            exp_str->append((char) REFERENCE);
//YYY exp_str->view_bytes();

//XXX cout << "\tappending " << REFERENCE << endl;
	    exp_str->append(left_item);
//YYY exp_str->view_bytes();	  
	  }
//XXX cout << "done appending" << endl; cout.flush();
	}
	//exp_str += j;  // Add the partial expression to the expression builder.
	if (ch != '\0') {
//XXX cout << "\tappending " << (int) ch << endl;
	  exp_str->append(ch);
//YYY exp_str->view_bytes();	  
	  ch = '\0';
	} else {
//XXX cout << "appending string from len " << exp_str->size() << endl; cout.flush();
	  exp_str->append(s);
//XXX cout << "to len " << exp_str->size() << endl;
	  delete s;
	  s = NULL;
	}
        new_expression = true;
        first_item = false;
	left_item = NULL;  // That wraps it up for application along this particular thread of evaluation.
      }
      // (else left_item is unchanged)
    }

    // We have a valid reference as the first item in the sub-term.
    if (first_item)
    {
//XXX cout << "first_item: " << endl;
      left_item = right_item;
//XXX view_bytes(&left_item,4);
//XXX cout << endl; cout.flush();

      right_item = NULL;
      first_item = false;  // No more first_item's in this function call.
    }

    if (right_item != NULL)
    {
//XXX cout << "right_item != NULL" << endl; cout.flush();
      // left_item is permanently NULL, but right_item has a value to add to the expression string.
      if (left_item == NULL) {
//XXX cout << "adding right side" << endl; cout.flush();
        if (right_item == exp) {
	  //exp_str += (char) SELF_REFERENCE;
//XXX cout << "\tappending " << SELF_REFERENCE << endl;
	  exp_str->append((char) SELF_REFERENCE);
//YYY exp_str->view_bytes();	
	} else {
          //exp_str += (char) REFERENCE;
          //exp_str += right_item;
//XXX cout << "\tappending " << REFERENCE << endl;
	  exp_str->append((char) REFERENCE);
//YYY exp_str->view_bytes();	  	  
	  exp_str->append(right_item);
	}
        right_item = NULL;
      }

      // We have a right and a left item... so apply right_item to left_item to obtain a new left_item.
      else {
//XXX cout << "Application time." << endl; cout.flush();
    // APPLICATION ##############################################################
        SNPL_data_type *dt = SNPL_find_type(right_item);
        
	// If this is another expression, recurse.
        if (dt == expression_type_EXECUTOR_CPP)
        {
//XXX cout << "Here we are recursing..." << endl; cout.flush();
          left_item = SNPL_reduce_expression(left_item,
            (SNPL_expression *) right_item);
	  //return left_item; //!!!!!
        }
        // If this is a primitive, apply left_item to it.
        else if (dt == cpp_primitive_type_EXECUTOR_CPP)
        {
//XXX cout << "<cpp_primitive> here." << endl; cout.flush();
	  left_item = SNPL_apply_primitive(left_item,
	    (SNPL_cpp_primitive *) right_item);
        }
        // If this is neither a primitive nor an expression, use the ^ operation.
        else {
//XXX cout << "SNPL_multiply()" << endl;
          left_item
            = SNPL_multiply(left_item,right_item);
	}
      }
      right_item = NULL;  // right_item needs to be NULL for the next iteration.
      //if (left_item == NULL) {
      //  new_expression = false;
	//nonterminate = true;
      //}
    // ##########################################################################
    }  // end application conditionals

    first_item = false;

  }  // end while()

  if (s != NULL)  //~ unneccessary.
    delete s;

  single_item = left_item;
  if (new_expression) { // The sub-term did not completely reduce; complete close parentheses and return the expression string.
//XXX cout << "new expression." << endl;
      //exp_str += (char) CLOSE_SUBTERM;
      exp_str->append((char) CLOSE_SUBTERM);

     //s = exp_str->copy();
     single_item = NULL;
     return exp_str;
  } else {
//XXX cout << "RETURNING NULL ##########" << endl; cout.flush();
    delete exp_str;
    return NULL;
  }
}



void *SNPL_reduce_expression(void *arg, SNPL_expression *exp)
{
//XXX cout << "SNPL_reduce_expression..." << endl;
//cout << "reduce_expression:" << endl;
//view_bytes((void *) exp->body, exp->length);
  char *p = exp->body;

//XXX cout << "length of exp = " << exp->length << endl;
//XXX view_bytes((void *) exp->body, exp->length);
//XXX cout << endl; cout.flush();


  //char *s = NULL;
  SNPL_expanding_string *s = NULL;

  // Distinguish between a SNPL_primitive_shell wrapper and a general lambda
  // expression
  if (*p == (char) REFERENCE) {
    single_item = SNPL_apply_shell(arg,
      (SNPL_primitive_shell *) *((void **) ++p));
  } else {
    s = SNPL_reduce_subexpression(arg, p, exp);

  }

//XXX if (s != NULL) {
//XXX cout << "Length of exp_str = " << s->size() << endl;
//YYY view_bytes((void *) s->copy(), s->size());
//XXX cout << endl;
//XXX }

  void *r;

  //if (!exp_str.length())
  if (s == NULL) {  // We have a right item after all!
    r = single_item;
//XXX cout << "Single item: " << (int) r << endl;
  } else {
//echo("a");
    // Build and return the new SNPL_expression structure.
//echo("b");
//echo("z");
    SNPL_expression *exp2
      = (SNPL_expression *) malloc(sizeof(SNPL_expression));
    exp2->body = s->copy();
//echo("y");
    exp2->length = s->size();
//echo("c");
    if (arg != NOARG)
      exp2->bound_variables = exp->bound_variables-1;
    else
      exp2->bound_variables = exp->bound_variables;
//XXX cout << "exp2 = " << (int) exp2 << endl;
//XXX cout << "expression_type: " << (int) expression_type_EXECUTOR_CPP << endl;
    SNPL_register_item(exp2, expression_type_EXECUTOR_CPP);
    r = (void *) exp2;
  }
  return r;
}



/*- end of file --------------------------------------------------------------*/
