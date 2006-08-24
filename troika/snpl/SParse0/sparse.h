/**
 * SParse.h
 *
 * ! Should probably be called "sparse_primitives.h" or something, as SParse
 *   can't include it directly.
 */

#ifndef SPARSE_H
#define SPARSE_H

#include "../include/snpl.h"



void *SParse_import(char *filename)
{
  return SNPL_import_graph(filename);
}



void SParse_export(void *p, char *s)
{
  //char *s = new char[100];
  //cout << "Export to file: ";
  //cin >> s;
  //cout << endl;
  SNPL_export_graph(p, s);
  //delete s;
}

void display_lookup_table(SNPL_lookup_table *lt)
{
  cout << endl;
  char **l = lt->list_all();
  int size=lt->size();
  for (int i=0; i<size; i++) {
    cout << l[i] << endl;
    delete l[i];
  }
  cout << endl;
  delete l;
}

void SParse_system_call(char *s)
{
  system(s);
}

#endif
