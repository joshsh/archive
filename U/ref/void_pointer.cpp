/* A void pointer can be cast as an int, and vice versa. */

#include <iostream>
using namespace std;

int main() {
  void *p = NULL;
  int i = (int) p;
  cout << "i = " << i << endl;
  p = (void *) i;
}
