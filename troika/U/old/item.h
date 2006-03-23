#include <fstream>
#include <cstring>

class item
{

  int item_id;
  char *item_name;

  static class classe *item_classe;

public:

  static char *copy(char *);

  static int hex_2_int(char);

  static char int_2_hex(int);

  static void write_block(ofstream &, char *);

  static char *read_block(char *);

  item(int, char *);

  item(int);

  ~item();

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  int id();

  void write_id(ofstream &);

  char *name();

  void write_formatted_name(ofstream &);

  void expand_name(char *);




  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static item* constructor_wrapper(int id0) { return new item(id0); }


};

classe *item::item_classe = NULL;
