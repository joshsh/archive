/*+
 * key_map.cpp
 *
 * The "class" maps keyboard events (and eventually other SDL events) to a set of
 * (char *)s which may then be used as hashing keys (i.e. instead of getting a
 * new, unique (char *) on each key press, you'll get one of KEY_MAP_SIZE
 * predetermined (char *) items.
 *
 * author: Josh Shinavier
 * last edited: 12/15/04
 */

#include "key_map.h"
using namespace std;



void SNPL_define_key(struct SNPL_key_map *keymap, char *ch, int index)
{
  if ((index >= 0)&&(index < keymap->size))
    keymap->keys[index] = ch;
}



struct SNPL_key_map *SNPL_construct_key_map(int size)
{
  SNPL_key_map *keymap
    = (struct SNPL_key_map *) malloc(sizeof(struct SNPL_key_map));
  keymap->size = size;
  keymap->keys = (char **) malloc(size*sizeof(char *));
  return keymap;
}



/* You will have to pass actual addresses in the deserializing version */
// Note: the "void *p" argument is there only because SNPL can't yet interface
// with a function with no parameters.
struct SNPL_key_map *SNPL_key_map_constructor(void *p)
{
  SNPL_data_type *dt = SNPL_reference_type("char");
  int i;
  char *s;
  SNPL_key_map *keymap
    = (struct SNPL_key_map *) malloc(sizeof(struct SNPL_key_map));
  keymap->size = KEY_MAP_SIZE;
  keymap->keys = (char **) malloc(KEY_MAP_SIZE*sizeof(char *));
  for (i=0; i<KEY_MAP_SIZE; i++) {
    s = (char *) malloc(sizeof(char));
    if (i > 127)
      *s = (char) i-256;
    else
      *s = (char) i;
    keymap->keys[i] = s;
    SNPL_register_item((void *) s, dt);
  }
  return keymap;
}



void SNPL_key_map_destructor(struct SNPL_key_map *keymap)
{
  // Note: you may or may not want to free the keys along with the map as I'm
  // doing here...
  //int i;
  //for (i=0; i<KEY_MAP_SIZE; i++) {
  //  free(keymap->keys[i]);
  //}
  
  free(keymap->keys);
  free(keymap);
}



char *SNPL_get_next_keydown(struct SNPL_key_map *keymap)
{
  int ch;
  SDL_Event event;

  do {
    SDL_PollEvent(&event);     
    if ( event.type == SDL_QUIT )
      return NULL; 
  } while (event.type != SDL_KEYDOWN);
 
  ch = event.key.keysym.sym;
  
  // Map SDL's >256 sym values to the 128-255 range.
  if (ch > KEY_MAP_SIZE/2)
    ch -= KEY_MAP_SIZE/2;
  ch = (ch+KEY_MAP_SIZE)%KEY_MAP_SIZE;  // are there any larger than 383?

//cout << "ch = " << (int) ch << " (" << (char) ch << ")" << endl; cout.flush();
  if (ch >=0 && ch < KEY_MAP_SIZE) {
//cout << "returning keymap->keys[" << ch << "] ( = " << *(keymap->keys[ch]) << ")" << endl; cout.flush();    
    return keymap->keys[ch];
  } else {
//cout << "returning NULL" << endl; cout.flush();    
    return NULL;
  }
}



/* Note: "un-signs" chars */
char *SNPL_map_char(char *p, struct SNPL_key_map *keymap)
{
  int ch = (int) *p;
  if (ch < 0)
    ch += 256;
  return keymap->keys[ch];
}



/*- end of file --------------------------------------------------------------*/
