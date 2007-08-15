/*+
 * lookup_table.h
 *
 * These use a binary search to pair a string identifier with a named "item".
 * Note: assumes that you'll want to look up existing items far more often than
 *       you will add new items to the table.
 *
 * author: Josh Shinavier
 * last edited: 7/6/04
 */
 
class lookup_table : void_array, item
{

  class reference
  {
  public:
    char *id;
    class item *ref;
    reference(class item *ref0, char *id0)
             : ref(ref0),
	       id(ascii::copy(id0))
    {}
    ~reference()
    {
      delete id;
    }
    void serialize(ofstream &out)
    {
      ascii::write(out, id);
      out << " ";
      ref->write_id(out);
    }
  };
  
  static class classe *item_classe;

public:

  lookup_table(int);

  lookup_table(class universe *);
  
  lookup_table(class universe *, int);

  virtual ~lookup_table();

  class item *get(char *);

  void add(class item *, char *);

  void serialize();
  
  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static lookup_table* constructor_wrapper(int id0) { return new lookup_table(id0); }

};

classe *lookup_table::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
