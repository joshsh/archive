#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

#include "include/item.h"
#include "include/node.h"
#include "include/linked_list.h"
#include "include/list.h"
#include "include/classe.h"
#include "include/ascii.h"
#include "include/universe.h"

#include "include/color.h"
#include "include/frame.h"
#include "include/grid.h"

#include "include/item.c"
#include "include/node.c"
#include "include/linked_list.c"
#include "include/list.c"
#include "include/classe.c"
#include "include/ascii.c"
#include "include/universe.c"

#include "include/color.c"
#include "include/frame.c"
#include "include/grid.c"

int main() {

  ofstream out ("out.txt");

  universe *u = new universe("test");

  ascii *foo = new ascii(u, "Salut le Monde!");
  ascii *bar = new ascii(u, "another_list_item");
  list *l = new list(u);
  l->enqueue((item*)foo);
  l->enqueue((item*)bar);
  color *c = new color(u, 127, 0, 0);
  c = new color(u, 0, 255, 0);
  //grid *g = new grid(u, c, 20, 20);
  frame *f = new frame(u, 0, 0, 10, 10);

  //cout << a->get_classe()->name() << endl;

  u->serialize(out);
  delete u;

  out.close();

  ifstream in ("out.txt");

  universe *u2 = new universe(in);

  foo = new ascii(u, "new");
  bar = new ascii(u, "Let's put these on a stack.");
  l = new list(u);
  l->push((item*)foo);
  l->push((item*)bar);

  ofstream out2 ("out2.txt");
  u2->serialize(out2);
  out2.close();
  delete u2;

  in.close();

}
