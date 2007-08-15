/*+
 * procedure.c
 *
 * For procedure objects.
 *
 * author: Josh Shinavier
 * last edited: 7/6/04
 */



/**
 * procedure::procedure(int id0)
 *
 *
 */
procedure::procedure(int id0)
       : item(id0)
{
  #ifdef PROCEDURE_DEBUG
    cout << "<> procedure::procedure(int)" << endl;
    cout.flush();
  #endif
}



/**
 * procedure::procedure(universe *u)
 *
 * Creates an empty procedure, to which multiple child procedures are to be
 * added.
 */
procedure::procedure(universe *u)
       : item(u, item_classe),
         function_ptr(NULL),
	 procedure_arguments(NULL),
	 subprocedures(NULL)
{
  #ifdef PROCEDURE_DEBUG
    cout << "<> procedure::procedure(universe *)" << endl;
    cout.flush();
  #endif
}



/**
 * procedure::procedure(universe *u, function *f)
 *
 * Creates a single-function procedure.  Note: you cannot add children to
 * such a procedure.
 */
procedure::procedure(universe *u, function *f)
       : item(u, item_classe)
{
  #ifdef PROCEDURE_DEBUG
    cout << "<> procedure::procedure(universe *, function *): \"" << f->name() << "\"" << endl;
    cout.flush();
  #endif
  
  construct(f);
}



/**
 * procedure::procedure(universe *u, char *s)
 *
 * Creates a single-function procedure.  Note: you cannot add children to
 * such a procedure.
 */
procedure::procedure(universe *u, char *s)
       : item(u, item_classe)
{
  #ifdef PROCEDURE_DEBUG
    cout << "<> procedure::procedure(universe *, char *): \"" << s << "\"" << endl;
    cout.flush();
  #endif
  
  function *f = identify_function(s);
  if (f == NULL) {
    subprocedures = new void_array();
  } else
    construct(f);
}



/**
 * procedure::~procedure()
 *
 *
 */
procedure::~procedure()
{
  #ifdef PROCEDURE_DEBUG
    cout << "<> procedure::~procedure()" << endl;
    cout.flush();
  #endif

  if (procedure_arguments != NULL)
    delete procedure_arguments;
    
  if (subprocedures != NULL) {
    for (int i=0; i<subprocedures->size(); i++) {
      subprocedure *sp = (subprocedure *) subprocedures->get(i);
      delete sp->register_map;
      delete sp->registers;
      delete sp;
    }
    delete subprocedures;
  }
}



void procedure::construct(function *f)
{
  function_ptr = f->ptr();
  void_array *args = f->arguments();
  int size = args->size();
  procedure_arguments = new void_array(size);
  for (int i=0; i<size; i++) {
    argument *arg = (argument *) args->get(i);
    procedure_arguments->append((void *) new argument(*arg));  //? Legal?
  }
}



/**
 * void_array *procedure::arguments()
 *
 *
 */
void_array *procedure::arguments()
{
  return procedure_arguments;
}



/**
 * procedure::execute(item **registers)
 *
 * If function_ptr != NULL, pass registers directly on to the wrapped function.
 * If function_ptr == NULL and children_size == 0, do nothing (empty procedure).
 * If children_size > 0, execute child procedures in order.
 *
 * Important note: we're assuming that the registers array is appropriate for
 * this procedure.
 */
void procedure::execute(item **registers)
{
  if (function_ptr != NULL) {
    function_ptr(registers);
  } else if (subprocedures->size()) {
    //...
  }
}



/**
 * void procedure::add(procedure *p, int *map)
 *
 *
 */
bool procedure::add(procedure *p, int *map)
{
  if (function_ptr == NULL) {
    //...
  }
}



void procedure::display()
{
  if (function_ptr != NULL) {
    function *f = identify_function((void *) function_ptr);
    cout << "Wrapper procedure for function \"" << f->name() << "\":" << endl;
  } else if (!subprocedures->size()) {
    cout << "Empty procedure." << endl;
    return;
  } else {
    cout << "Compound procedure (" << subprocedures->size() << " children):" << endl;
  }
  for (int i=0; i<procedure_arguments->size(); i++) {
    argument *arg = (argument *) procedure_arguments->get(i);
    if (arg->readwrite())
      cout << "w:";
    else
      cout << "r:";
    cout << arg->get_classe()->name() << " ";
  }
  cout << endl;
}



/**
 * void procedure::serialize(ofstream &out)
 *
 *
 */
void procedure::serialize(ofstream &out)
{
  #ifdef PROCEDURE_DEBUG
    cout << "<> void procedure::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t";
  if (function_ptr != NULL) {
    out << '.';
    ascii::write(out, identify_function((void *) function_ptr)->name());
  } else {
    //...
  }
  out << '\n';
}



/**
 * void procedure::deserialize(ifstream &in)
 *
 *
 */
void procedure::deserialize(ifstream &in)
{
  char *s = new char[MAX_STRLEN];
  in >> s >> s;
  if (*s == '.') {
    s++;
    char *s2 = ascii::read(s);
    construct(identify_function(s2));
    delete s2;
    s--;
  } else {
    //...
  }

  #ifdef PROCEDURE_DEBUG
    cout << "<> void procedure::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  delete s;
}



/*- end of file --------------------------------------------------------------*/
