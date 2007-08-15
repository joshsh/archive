#include <fstream>

/*+
 * class color
 *
 *//*=========================================================================*/

class color : public item
{
private:

  void construct(char *s)
  {
    red = (char) 16*hex_2_int((int)*s);
    s++;
    red += (char) hex_2_int((int)*s);
    s++;
    green = (char) 16*hex_2_int((int)*s);
    s++;
    green += (char) hex_2_int((int)*s);
    s++;
    blue = (char) 16*hex_2_int((int)*s);
    s++;
    blue += (char) hex_2_int((int)*s);
  }

public:
  // ba ^ "unsigned char"
  unsigned char red, green, blue;

  color(int red0, int green0, int blue0) : item(int id0, char *name0)
  {
    red = red0;
    green = green0;
    blue = blue0;
  }

  color(char *s) : item(int id0, char *name0)
  {
    construct(s);
  }

  color(int id0, ifstream in) : item(int id0)
  {
    char *s = new char[MAX_STRLEN];
    in >> s;
    contruct(s);
    in >> s;
    expand_name(s);
    delete s;
  }

  /**
   * bool equals(color *c)
   * Compares to another "color", returns true if r,g,b values are the same.
   */
  bool equals(color *c)
  {
    return ((c->red == red)&&(c->green == green)&&(c->blue == blue));
  }

  /**
   * char *print()
   * Note: s must have a length of at least 7
   */
  void print(char *s)
  {
    *s = int_2_hex(red/16);
    s++;
    *s = int_2_hex(red%16);
    s++;
    *s = int_2_hex(green/16);
    s++;
    *s = int_2_hex(green%16);
    s++;
    *s = int_2_hex(blue/16);
    s++;
    *s = int_2_hex(blue%16);
    s++;
    *s = '\0';
  }

  /* Write to a stream for storage */
  write(ofstream out)
  {
    char *s = new char[7];
    print(s);
    out << "\tcolor " << id() << " { " << s << "\t";
    write_formatted_name(out);
    out << " }\n";
    delete s;
  }

}; /*- end class color .......................................................*/
