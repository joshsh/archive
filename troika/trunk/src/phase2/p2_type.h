/*+

  p2_type.h

  last edited: 6/1/05

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

*///////////////////////////////////////////////////////////////////////////////

#ifndef P2_TYPE_H
#define P2_TYPE_H



typedef void * P2_type;

#include "p2_error.h"



#define ENCODE_FORMAT    void (*)(void *, char *)
#define DECODE_FORMAT    void *(*)(char *)
#define DESTROY_FORMAT   void (*)(void *)
#define CLONE_FORMAT     void *(*)(void *)



P2_error P2_type_init();

P2_error P2_type_end();



// [!] Re-name to P2_type__new.
P2_type P2_register_type(

    char *name,
    void (*encode)(void *, char *),
    void *(*decode)(char *),
    void (*destroy)(void *),
    void *(*clone)(void *));



P2_type P2_type_lookup(char *name);

char *P2_type_name(P2_type type_index);



void P2_encode(void *p, P2_type type_index, char *buffer);

void *P2_decode(P2_type type_index, char *buffer);

void P2_destroy(void *p, P2_type type_index);

void *P2_clone(void *p, P2_type type_index);



#endif

/*- end of file */
