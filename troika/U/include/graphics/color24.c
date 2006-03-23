/*+
 * color24.c
 *
 * A helper class (not user-accessible) to store and compare color values for
 * SDL graphics.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * color24::color24()
 *
 *
 */
color24::color24()
         : color_red(0),
           color_green(0),
           color_blue(0) {}



/**
 * color24::color24(char *s)
 *
 * Create a color from a string.
 */
color24::color24(char *s)
{
  decode(s);
}



/**
 * color24::color24(int red0, int green0, int blue0)
 *
 *
 */
color24::color24(int red0, int green0, int blue0)
     : color_red((unsigned char) red0),
       color_green((unsigned char) green0),
       color_blue((unsigned char) blue0) {}



/**
 * color24::~color24()
 *
 *
 */
color24::~color24() {}



/**
 * int color24::red()
 *
 * Return the color's red component.
 */
int color24::red()
{
  return (int) color_red;
}



/**
 * int color24::green()
 *
 * Return the color's green component.
 */
int color24::green()
{
  return (int) color_green;
}



/**
 * int color24::blue()
 *
 * Return the color's blue component.
 */
int color24::blue()
{
  return (int) color_blue;
}



/**
 * bool color24::equals(color24 *c)
 *
 * Compare to another "color", return true if r,g,b values are the same.
 */
bool color24::equals(color24 *c)
{
  return ((c->color_red == color_red)
        &&(c->color_green == color_green)
        &&(c->color_blue == color_blue));
}



/**
 * void color24::decode(char *s)
 *
 * Read the color's r,g,b values from a string, e.g. FFFFFF for white.
 */
void color24::decode(char *s)
{
  color_red = (char) 16*ascii::hex_2_int((int)*s);
  s++;
  color_red += (char) ascii::hex_2_int((int)*s);
  s++;
  color_green = (char) 16*ascii::hex_2_int((int)*s);
  s++;
  color_green += (char) ascii::hex_2_int((int)*s);
  s++;
  color_blue = (char) 16*ascii::hex_2_int((int)*s);
  s++;
  color_blue += (char) ascii::hex_2_int((int)*s);
  s -= 5;
}



/**
 * char *color24::encode()
 *
 * ASCII-encode the color's r,g,b values (HTML-compatible).
 */
char *color24::encode()
{
  char *s = new char[7];

  *s = ascii::int_2_hex(color_red/16);
  s++;
  *s = ascii::int_2_hex(color_red%16);
  s++;
  *s = ascii::int_2_hex(color_green/16);
  s++;
  *s = ascii::int_2_hex(color_green%16);
  s++;
  *s = ascii::int_2_hex(color_blue/16);
  s++;
  *s = ascii::int_2_hex(color_blue%16);
  s++;
  *s = '\0';
  s -= 6;

  return s;
}



/*- end of file --------------------------------------------------------------*/
