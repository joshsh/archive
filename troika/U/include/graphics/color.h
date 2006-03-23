/*+
 * color.h
 *
 * A derivative of "color24"; the user may define, destroy and modify these as
 * needed.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class color : item, public color24
{
  static class classe *item_classe;

  // ba ^ "unsigned char"
  //unsigned char color_red, color_green, color_blue;

public:

  color(int);

  color(class universe *, int, int, int);

  color(class universe *, color24 *);

  virtual ~color();

  //int red(), green(), blue();

  bool equals(color *);

  bool equals(color24 *);

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static color* constructor_wrapper(int id0) { return new color(id0); }

};

classe *color::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
