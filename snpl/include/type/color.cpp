/*+
 * color.cpp
 *
 * A helper class (not user-accessible) to store and compare color values for
 * SDL graphics.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

 #include "color.h"
 using namespace std;
 
 

/**
 * SNPL_color::SNPL_color()
 *
 *
 */
SNPL_color::SNPL_color()
         : color_red(0),
           color_green(0),
           color_blue(0) {}



/**
 * SNPL_color::SNPL_color(char *s)
 *
 * Create a color from a string.
 */
SNPL_color::SNPL_color(char *s)
{
  decode(s);
}



/**
 * SNPL_color::SNPL_color(int red0, int green0, int blue0)
 *
 *
 */
SNPL_color::SNPL_color(int red0, int green0, int blue0)
     : color_red((unsigned char) red0),
       color_green((unsigned char) green0),
       color_blue((unsigned char) blue0) {}



/**
 * SNPL_color::~SNPL_color()
 *
 *
 */
SNPL_color::~SNPL_color() {}



/**
 * int SNPL_color::red()
 *
 * Return the color's red component.
 */
int SNPL_color::red()
{
  return (int) color_red;
}



/**
 * int SNPL_color::green()
 *
 * Return the color's green component.
 */
int SNPL_color::green()
{
  return (int) color_green;
}



/**
 * int SNPL_color::blue()
 *
 * Return the color's blue component.
 */
int SNPL_color::blue()
{
  return (int) color_blue;
}



/**
 * bool SNPL_color::equals(SNPL_color *c)
 *
 * Compare to another "color", return true if r,g,b values are the same.
 */
bool SNPL_color::equals(SNPL_color *c)
{
  return ((c->color_red == color_red)
        &&(c->color_green == color_green)
        &&(c->color_blue == color_blue));
}



/**
 * void SNPL_color::decode(char *s)
 *
 * Read the color's r,g,b values from a string, e.g. FFFFFF for white.
 */
void SNPL_color::decode(char *s)
{
  color_red = (char) 16*SNPL_hex_2_int((int)*s);
  s++;
  color_red += (char) SNPL_hex_2_int((int)*s);
  s++;
  color_green = (char) 16*SNPL_hex_2_int((int)*s);
  s++;
  color_green += (char) SNPL_hex_2_int((int)*s);
  s++;
  color_blue = (char) 16*SNPL_hex_2_int((int)*s);
  s++;
  color_blue += (char) SNPL_hex_2_int((int)*s);
  s -= 5;
}



/**
 * char *SNPL_color::encode()
 *
 * ASCII-encode the color's r,g,b values (HTML-compatible).
 */
char *SNPL_color::encode()
{
  char *s = new char[7];

  *s = SNPL_int_2_hex(color_red/16);
  s++;
  *s = SNPL_int_2_hex(color_red%16);
  s++;
  *s = SNPL_int_2_hex(color_green/16);
  s++;
  *s = SNPL_int_2_hex(color_green%16);
  s++;
  *s = SNPL_int_2_hex(color_blue/16);
  s++;
  *s = SNPL_int_2_hex(color_blue%16);
  s++;
  *s = '\0';
  s -= 6;

  return s;
}



// Interface methods ############################################################

SNPL_color *new_color(char *s)
{
  return new SNPL_color(s);
}


/*- end of file --------------------------------------------------------------*/
