/*+
 * caste.h
 *
 * For defining new internal object types.
 *
 * author: Josh Shinavier
 * last edited: 7/27/04
 */

class caste : item
{
  static class classe *item_classe;

  class classe *wrapped_classe;
  caste **caste_array;
  int array_size;
  
public:
  
  caste(int);
  
  caste(class universe *, class classe *);
  
  caste(class universe *, caste **, int);
  
  virtual ~caste();
  
  void display();
  
  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static caste* constructor_wrapper(int id0) { return new caste(id0); }

};

classe *caste::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
