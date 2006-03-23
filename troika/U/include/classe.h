/*+
 * class.h
 *
 * A wrapper class to manage objects derived from "item".
 *
 * author: Josh Shinavier
 * last edited: 6/27/04
 */

class classe //Don't need to be items, as we're not putting them in lists.
{
  int classe_id;//, classe_size;
  char *classe_name;
  class item *(*constructor_wrapper)(int);
  //class linked_list *instances;

public:

  classe(int, char *, class item*(*)(int), void (*)(classe *));

  ~classe();  //Remember to make this virtual if deriving from "item"

  int id();

  //int size();

  char *name();

  //int add(class item *);

  //void remove(class item *);

  item *create(int);

  //Just in case "classe" is ever made a derivative of "item"
  //void serialize(ofstream &);
  //void deserialize(ifstream &);

};

/*- end of file --------------------------------------------------------------*/
