/*+
 * fuzzy_set.h
 *
 * These are specifically fuzzy input sets as far as our project is concerned
 * (there is no feedback involved, but there could be). They each contain a
 * name identifier and two values which, in combination with its neighboring
 * sets in the family, determine the set's degree-of-membership function.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class fuzzy_set : item
{
  double dom_min, dom_max;
  class ascii *fuzzy_set_name;

  static class classe *item_classe;

public:

  fuzzy_set(int);

  fuzzy_set(class universe *, char *, double, double);

  //virtual ~fuzzy_set();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  double min();

  double max();

  char *name();

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static fuzzy_set* constructor_wrapper(int id0) { return new fuzzy_set(id0); }

};

classe *fuzzy_set::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
