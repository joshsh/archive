/*+
 * ascii.c
 *
 * For ascii text strings.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * ascii::ascii(int id0)
 *
 *
 */
ascii::ascii(int id0)
       : item(id0),
         ascii_key(NULL)
{
  #ifdef ASCII_DEBUG
    cout << "<> ascii::ascii(int)" << endl;
    cout.flush();
  #endif
}



/**
 * ascii::ascii(universe *u, char *s)
 *
 *
 */
ascii::ascii(universe *u, char *s)
       : item(u, item_classe),
         ascii_key(copy(s))
{
  #ifdef ASCII_DEBUG
    cout << "<> ascii::ascii(universe *, char *)" << endl;
    cout.flush();
  #endif
}



/**
 * ascii::~ascii()
 *
 *
 */
ascii::~ascii()
{
  #ifdef ASCII_DEBUG
    cout << "<> ascii::~ascii()" << endl;
    cout.flush();
  #endif

  if (ascii_key != NULL)
    delete ascii_key;
}



/**
 * int ascii::compare_to(item *i)
 *
 * Perform a strcmp() on the two asciis' key values.
 */
int ascii::compare_to(item *i)
{
  if (i->get_classe() == item_classe)
    return strcmp(ascii_key, ((ascii *) i)->ascii_key);
}



/**
 * void ascii::serialize(ofstream &out)
 *
 *
 */
void ascii::serialize(ofstream &out)
{
  #ifdef ASCII_DEBUG
    cout << "<> void ascii::serialize(ofstream &): \"" << ascii_key << "\"" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t";
  write(out, ascii_key);
  out << '\n';
}



/**
 * void ascii::deserialize(ifstream &in)
 *
 *
 */
void ascii::deserialize(ifstream &in)
{
  char *s = new char[MAX_STRLEN];
  in >> s >> s;
  ascii_key = read(s);

  #ifdef ASCII_DEBUG
    cout << "<> void ascii::deserialize(ifstream &): \"" << ascii_key << "\"" << endl;
    cout.flush();
  #endif

  delete s;
}



/**
 * char *ascii::key()
 *
 * Return the ascii's key value.
 */
char *ascii::key()
{
  return ascii_key;
}



/*+ static formatting functions *//*========================================*/

/**
 * static char *ascii::copy(char *s)
 *
 * Creates a new copy of a C-string.
 * Note: will make an empty string out of a NULL char pointer.
 */
char *ascii::copy(char *s)
{
  char *s2;
  if ((s == NULL)||(*s == '\0')) {
    s2 = new char[1];
    *s2 = '\0';
  } else {
    s2 = new char[strlen(s)+1];
    strcpy(s2, s);
  }
  return s2;
}



/**
 * static int ascii::hex_2_int(char ch)
 *
 * hex-digit char of range 0x00-0x0F maps to int of range 0-15
 * Note: only accepts hexadecimal digits in UPPERCASE (e.g. 2 or F, but not f)
 */
int ascii::hex_2_int(char ch)
{
  if ((ch>=48)&&(ch<58))
    return (int)ch - 48;
  else
    return (int)ch - 55;
}



/**
 * static char ascii::int_2_hex(int i)
 *
 * int of range 0-15 maps to hex-digit char of range 0x00-0x0F
 */
char ascii::int_2_hex(int i)
{
  if (i<10)
    return (char)(i+48);
  else
    return (char)(i+55);
}



/**
 * static void ascii::write(ofstream &out, char *s)
 *
 * Formats and outputs a string as a "block" so that it can be read later as
 * a single token, in spite of any original whitespace or negative chars.
 * For instance the space character will appear as "\20", the tab as "\09".
 */
void ascii::write(ofstream &out, char *s)
{
  while (*s != '\0') {
    int ch = (((int)*s) + 128) % 128;
    if (ch > 32) {
      if (ch == 92)
        out << "\\\\";
      else
        out << (char) ch;
    } else
      out << '\\' << int_2_hex(ch/16) << int_2_hex(ch%16);
    s++;
  }
}



/**
 * static char *ascii::read(char *s)
 *
 * Read a string encoded as a "block".  s may not be null or empty, and
 * must be properly formatted as if produced by write_block().
 */
char *ascii::read(char *s)
{
  char *s2 = new char[strlen(s)+1], *s3 = s2;  //Note: some of s2 may go unused
  while (*s != '\0') {
    if (*s == '\\') {
      s++;
      if (*s == '\\')
        *s2 = '\\';
      else {
        int ch = 16*hex_2_int(*s);
        s++;
        ch += hex_2_int(*s);
        *s2 = (char) ch;
      }
    } else
      *s2 = *s;
    s++;
    s2++;
  }
  *s2 = '\0';
  return s3;
}

/*- formatting ---------------------------------------------------------------*/

/*- end of file --------------------------------------------------------------*/
