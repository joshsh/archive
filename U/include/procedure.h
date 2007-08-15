/*+
 * procedure.h
 *
 * For procedure objects.
 *
 * author: Josh Shinavier
 * last edited: 6/25/04
 */

struct subprocedure
{
  class procedure *proc;
  int *register_map;
  class item **registers;
};

 
 
class procedure : item
{
  static class classe *item_classe;  

  void (*function_ptr)(class item**);

  //int registers_size;
  //class classe **register_classes;
  //bool *register_readwrite;
  class void_array *procedure_arguments;
  
  class void_array *subprocedures;  
  
public:

  procedure(int);

  procedure(class universe *);

  procedure(class universe *, class function *);
  
  procedure(class universe *, char *);
  
  virtual ~procedure();

  void construct(class function *);
  
  //Modularity issues...
  class void_array *arguments();
  
  void execute(class item **);
  
  bool add(procedure *, int *);  
  
  void display();
  
  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static procedure* constructor_wrapper(int id0) { return new procedure(id0); }

};

classe *procedure::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/

