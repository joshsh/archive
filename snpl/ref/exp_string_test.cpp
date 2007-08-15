/** A little testing routine for the "SNPL_expanding_string" class. */

#include <iostream>
#include "../include/util/expanding_string.h"

using namespace std;



int main()
{
  SNPL_expanding_string *es = new SNPL_expanding_string(10,2);
  es->append('c');
  es->append('h');
  es->append("aracter");
  cout << es->copy() << "[" << es->get(3) << "]" << endl;
  es->minimize();
  es->clear();
  es->append("yippee!");
  es->append('o');
  cout << es->copy() << "[" << es->get(3) << "]" << endl;
  delete es;

}
