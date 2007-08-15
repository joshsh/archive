/*+
 * ascii.h
 *
 * For ascii text strings.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class ascii : item
{
  char *ascii_key;

  static class classe *item_classe;

public:

  ascii(int);

  ascii(class universe *, char *);

  virtual ~ascii();

  virtual int compare_to(item *);

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  char *key();

  static char *copy(char *);

  static int hex_2_int(char);

  static char int_2_hex(int);

  static void write(ofstream &, char *);

  static char *read(char *);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static ascii* constructor_wrapper(int id0) { return new ascii(id0); }

};

classe *ascii::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
