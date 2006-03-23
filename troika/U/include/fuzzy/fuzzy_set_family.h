/*+
 * fuzzy_set_family.h
 *
 * A fuzzy_set_family is bound to a particular (input) dimension and controls
 * a list of fuzzy_sets. Given a crisp input value, a fuzzy_set_family will
 * generate an array of degrees of membership appropriate to its member sets'
 * dom information.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class fuzzy_set_family : item
{
  class list *member_sets;
  class ascii *family_name;

  static class classe *item_classe;

public:

  fuzzy_set_family(int);

  fuzzy_set_family(class universe *, char *);

  //virtual ~fuzzy_set_family();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  int size();

  char *name();

  class list *members();

  void add(char *name, double, double);

  void add(class fuzzy_set *);

  int find_index(char *);

  void fuzzify(double, double *);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static fuzzy_set_family* constructor_wrapper(int id0) { return new fuzzy_set_family(id0); }

};

classe *fuzzy_set_family::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
