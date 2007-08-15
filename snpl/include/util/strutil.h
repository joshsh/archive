#ifndef STRUTIL_H
#define STRUTIL_H

/*+
 * strutil.h
 *
 * Functions for copying C strings and reading from / writing to specially
 * formatted text blocks for the de-/serializers.
 *
 * author: Josh Shinavier
 * last edited: 9/20/04
 */
 
#include <iostream>
#include <fstream>
using namespace std;  // Note: apparently the
// "namespace" directive is necessary even in header
// files (compilation gave me an error message before I
// added this line).


void view_bytes(void *, int n);

char *SNPL_copy_cstring(char *);
int SNPL_hex_2_int(char);
char SNPL_int_2_hex(int);
char *SNPL_read_cstring(char *);
void SNPL_write_cstring(ofstream &, char *);



#endif

/*- end of file ---------------------------------------------------------------*/
