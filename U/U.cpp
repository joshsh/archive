#define DEBUG

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

  ifstream in ("data/in.U");
  universe *u = new universe(in);
  in.close();

  /*
  color *white_c = new color(u, 255, 255, 255);
  color *black_c = new color(u, 0, 0, 0);
  grid *g = new grid(u, white_c, 20, 20);
  g->fill(black_c);
  //*/
  
  
  ofstream out2 ("data/out.U");
  u->serialize(out2);
  out2.close();
  //*/

  //delete u;

}
