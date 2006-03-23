/*+
 * fuzzy_output_family.h
 *
 * A fuzzy_output_family is just a list of set names, which are used as a
 * reference by the calculating functions of a FLE object.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class fuzzy_output_family : item
{
  class list *member_sets;
  class ascii *family_name;

  static class classe *item_classe;

public:

  fuzzy_output_family(int);

  fuzzy_output_family(class universe *, char *);

  //virtual ~fuzzy_output_family();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  int size();

  char *name();

  class list *members();

  int find_index(char *);

  void add(char *);

  void add(class ascii *);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static fuzzy_output_family* constructor_wrapper(int id0) { return new fuzzy_output_family(id0); }

};

classe *fuzzy_output_family::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
