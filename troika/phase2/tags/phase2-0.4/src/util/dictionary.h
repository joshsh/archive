/*+


*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

*////////////////////////////////////////////////////////////////////////////////

#ifndef DICTIONARY_H
#define DICTIONARY_H


#include <stdlib.h>

struct JS_dictionary {
  HASH_TABLE *table;
  
  int size, buffer_size, capacity;
  double expansion, sparsity;
  void **buffer;
  int (*hashing_function) (void *key);
  int (*compare_to) (void *key1, void *key2);
};

#define DICTIONARY struct JS_dictionary



int compare_addresses(void *key1, void *key2);
int hash_address(void *key);
#define ADDRESS_DEFAULTS hash_address, compare_addresses

int compare_strings(void *key1, void *key2);
int hash_string(void *key);
#define STRING_DEFAULTS hash_string, compare_strings



// Constructor
HASH_TABLE *hash_table__new(
  int buffer_size,
  double expansion,
  double sparsity,
  int (*hashing_function) (void *),
  int (*compare_to) (void *, void *));

// Destructor
void hash_table__delete(HASH_TABLE *h);

// Look up a key to obtain a target
void *hash_table__lookup(HASH_TABLE *h, void *key);

// Add a key/target pair.  Return value is the target which is displaced by the
// new pair (if any).
void *hash_table__add(HASH_TABLE *h, void *key, void *target);

// Remove the key and its target.  Return value is the target.
void *hash_table__remove(HASH_TABLE *h, void *key);

void hash_table__forall(HASH_TABLE *h, void (*func)(void *, void *));
void hash_table__forall_keys(HASH_TABLE *h, void (*func)(void *));
void hash_table__forall_targets(HASH_TABLE *h, void (*func)(void *));



#endif

/*- end of file */
