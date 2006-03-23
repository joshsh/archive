/*+
 * color.c
 *
 * A derivative of "color24"; the user may define, destroy and modify these as
 * needed.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * color::color(int id0)
 *
 *
 */
color::color(int id0)
       : item(id0),
         color24()
{
  #ifdef COLOR_DEBUG
    cout << "<> color::color(int)" << endl;
    cout.flush();
  #endif
}



/**
 * color::color(universe *u, int red0, int green0, int blue0)
 *
 *
 */
color::color(universe *u, int red0, int green0, int blue0)
     : item(u, item_classe),
       color24(red0, green0, blue0)
{
  #ifdef COLOR_DEBUG
    cout << "<> color::color(universe *, int, int, int)" << endl;
    cout.flush();
  #endif
}



/**
 * color::color(universe *u, color24 *c24)
 *
 *
 */
color::color(universe *u, color24 *c24)
       : item(u, item_classe),
         color24(c24->red(), c24->green(), c24->blue())
{
  #ifdef COLOR_DEBUG
    cout << "<> color::color(universe *, color24 *)" << endl;
    cout.flush();
  #endif
}



/**
 * color::~color()
 *
 *
 */
color::~color()
{
  #ifdef COLOR_DEBUG
    cout << "<> color::~color()" << endl;
    cout.flush();
  #endif
}



/**
 * bool color::equals(color *c)
 *
 * Compares to another "color", returns true if r,g,b values are the same.
 */
bool color::equals(color *c)
{
  return ((color24 *) c)->equals((color24 *) this);
}



/**
 * bool color::equals(color24 *c)
 *
 * Compares to another "color", returns true if r,g,b values are the same.
 */
bool color::equals(color24 *c)
{
  return c->equals((color24 *) this);
}



/**
 * void color::serialize(ofstream &out)
 *
 *
 */
/* Write to a stream for storage */
void color::serialize(ofstream &out)
{
  #ifdef COLOR_DEBUG
    cout << "<> void color::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  char *s = encode();

  out << '\t' << id() << ")\t" << s << '\n';

  delete s;
}



/**
 * void color::deserialize(ifstream &in)
 *
 *
 */
void color::deserialize(ifstream &in)
{
  #ifdef COLOR_DEBUG
    cout << "<> void color::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[7];
  in >> s >> s;
  decode(s);
  delete s;
}



/*- end of file --------------------------------------------------------------*/
