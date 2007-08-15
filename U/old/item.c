/*+
 * class item
 *
 *//*=========================================================================*/

/*+ static formatting functions *//*========================================*/

/**
 * static char *copy(char *s)
 *
 * Creates a new copy of a C-string.
 * Note: will make an empty string out of a NULL char pointer.
 */
char *item::copy(char *s)
{
  char *s2;
  if ((s == NULL)||(*s == '\0')) {
    s2 = new char[1];
    *s2 = '\0';
  } else {
    s2 = new char[strlen(s)];
    strcpy(s2, s);
  }
  return s2;
}

/**
 * static int hex_2_dec(char ch)
 *
 * hex-digit char of range 0x00-0x0F maps to int of range 0-15
 * Note: only accepts hexadecimal digits in UPPERCASE (e.g. 2 or F, but not f)
 */
int item::hex_2_int(char ch)
{
  if ((ch>=48)&&(ch<58))
    return (int)ch - 48;
  else
    return (int)ch - 55;
}

/**
 * static char int_2_hex(int i)
 *
 * int of range 0-15 maps to hex-digit char of range 0x00-0x0F
 */
char item::int_2_hex(int i)
{
  if (i<10)
    return (char)(i+48);
  else
    return (char)(i+55);
}

/**
 * void write_block(ofstream &out, char *s)
 *
 * Formats and outputs a string as a "block" so that it can be read later as
 * a single token, in spite of any original whitespace or negative chars.
 * For instance the space character will appear as "\20", the tab as "\09".
 */
void item::write_block(ofstream &out, char *s)
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
 * static char *read_block(char *s)
 *
 * Read a string encoded as a "block".  s may not be null or empty, and
 * must be properly formatted as if produced by write_block().
 */
char *item::read_block(char *s)
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

item::item(int id, char *name)
{
  item_id = id;
  item_name = copy(name);
}

/* Note: this isn't a COMPLETE constructor; it is expected that the derived
   class which calls this constructor independently sets "name" */
item::item(int id0)
{
  item_id = id0;
}

item::~item()
{
  delete item_name;
}

void item::serialize(ofstream &out)
{/*
  for (int i=0; i<indent; i++)
    out << '\t';
  out << item_id << ' ';
  write_block(out, item_name);
  out << '\n';*/
}

void item::deserialize(ifstream &)
{
  //0
}

int item::id()
{
  return item_id;
}

void item::write_id(ofstream &out)
{
  out << item_id; //! ###
}

//! Get rid of this; we want to keep item_name protected.
char *item::name()
{
  return item_name;
}

/**
 * void write_formatted_name(ofstream out)
 *
 * Block-formats item_name for storage.
 */
void item::write_formatted_name(ofstream &out)
{
  write_block(out, item_name);
}

/**
 * void expand_name(char *s)
 */
void item::expand_name(char *s)
{
  item_name = read_block(s);
}








/*- end class item -----------------------------------------------------------*/
