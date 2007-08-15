/*+
 * strutil.cpp
 *
 * Functions for copying C strings and reading from / writing to specially
 * formatted text blocks for the de-/serializers.
 *
 * author: Josh Shinavier
 * last edited: 12/14/04
 */

// For view_bytes()
#include "strutil.h"
using namespace std;


// Temporary
void view_bytes(void *start_address, int n)
{
  cout << n << " bytes beginning at " << (int) start_address << ":";
  char *s = (char *) start_address;
  for (int i=0; i<n; i++) {
    cout << " " << (int) *s;
    s++;
  }
  cout << endl;
}



/**
 * char *SNPL_copy_cstring(char *s)
 *
 * Creates a new copy of a C-string.
 * Note: will make an empty string out of a NULL char pointer.
 */
char *SNPL_copy_cstring(char *s)
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
 * int SNPL_hex_2_int(char ch)
 *
 * hex-digit char of range 0x00-0x0F maps to int of range 0-15
 * Note: only accepts hexadecimal digits in UPPERCASE (e.g. 2 or F, but not f)
 */
int SNPL_hex_2_int(char ch)
{
  if ((ch>=48)&&(ch<58))
    return (int)ch - 48;
  else
    return (int)ch - 55;
}



/**
 * char SNPL_int_2_hex(int i)
 *
 * int of range 0-15 maps to hex-digit char of range 0x00-0x0F
 */
char SNPL_int_2_hex(int i)
{
  if (i<10)
    return (char)(i+48);
  else
    return (char)(i+55);
}



/**
 * char *SNPL_read_cstring(char *s)
 *
 * Read a string encoded as a "block".  s may not be null or empty, and
 * must be properly formatted as if produced by SNPL_write_cstring().
 */
char *SNPL_read_cstring(char *s)
{
  char *s2 = new char[strlen(s)+1], *s3 = s2;  //Note: some of s2 may go unused
  while (*s != '\0') {
    if (*s == '\\') {
      s++;
      if (*s == '\\')
        *s2 = '\\';
      else {
        int ch = 16*SNPL_hex_2_int(*s);
        s++;
        ch += SNPL_hex_2_int(*s);
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



/**
 * void SNPL_write_cstring(ofstream &out, char *s)
 *
 * Formats and outputs a string as a "block" so that it can be read later as
 * a single token, in spite of any original whitespace or negative chars.
 * For instance the space character will appear as "\20", the tab as "\09".
 */
void SNPL_write_cstring(ofstream &out, char *s)
{
  while (*s != '\0') {
    int ch = (((int)*s) + 128) % 128;
    if (ch > 32) {
      if (ch == 92)
        out << "\\\\";
      else
        out << (char) ch;
    } else
      out << '\\' << SNPL_int_2_hex(ch/16) << SNPL_int_2_hex(ch%16);
    s++;
  }
}



/*- end of file ---------------------------------------------------------------*/

