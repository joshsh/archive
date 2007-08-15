/*+
 * key_map.h
 *
 * The "class" maps keyboard events (and eventually other SDL events) to a set of
 * (char *)s which may then be used as hashing keys (i.e. instead of getting a
 * new, unique (char *) on each key press, you'll get one of KEY_MAP_SIZE
 * predetermined (char *) items.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

#ifndef KEY_MAP_H
#define KEY_MAP_H



#define KEY_MAP_SIZE 256

#include "../snpl.h"  // So the constructor can register all new (char *)s.
#include <SDL/SDL.h>
#include <cstdlib>
using namespace std;

// Pretty minimal at the moment...
struct SNPL_key_map
{
  int size;
  char **keys;
};



void SNPL_define_key(struct SNPL_key_map *, char *, int);



struct SNPL_key_map *SNPL_construct_key_map(int size);

struct SNPL_key_map *SNPL_key_map_constructor(void *);

void SNPL_key_map_destructor(struct SNPL_key_map *);

char *SNPL_get_next_keydown(struct SNPL_key_map *);

char *SNPL_map_char(char *, struct SNPL_key_map *);


#endif

/*- end of file --------------------------------------------------------------*/
