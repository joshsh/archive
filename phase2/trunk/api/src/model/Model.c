/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

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

*******************************************************************************/

#include <Model.h>


struct Model
{
    Name *uri;
    char *url;

    Environment *e;

    ModelTensor *tensor;
    ModelVector *root_vector;
};


Model *
model__create_empty( Name *name, Environment *e )
{

}


void
model__walk( Model *m, Visitor v )
{
    ModelVector *visited;

    visited = vector__create();


}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
