/**
 * This test program illustrates that a void C++ function may be treated as a
 * function which returns a pointer, provided the argument structure is the
 * same.
 */

#include <iostream>

using namespace std;

void *returner(void *p1, void *p2)
{
  cout << "returner:"
       << (int) p1 << " " << (int) p2
       << endl;
  return (void *) 137;
}

void nonreturner(void *p1, void *p2)
{
  cout << "nonreturner:"
       << (int) p1 << " " << (int) p2
       << endl;
  //return (void *) 42;
}

int args = 2;

void execute(void *(*p)(void *, void *), void **registers)
{
  void *p1 = *registers;
  registers++;
  void *p2 = *registers;
  registers--;
  int x = 0;
  //void *(*q)(void **) = (void *(*)(void **)) p;
  //x = (int) q(registers);
  void *(*q)(void *) = (void *(*)(void *)) p;
  x = (int) q(*registers);
  //x = (int) p(p1, p2);
  
  switch (args) {
    case 0:
      ((void *(*)()) p)();
      break;
    case 1:
      ((void *(*)(void *)) p)(
        registers[0]);
      break;
    case 2:
      ((void *(*)(void *, void *)) p)(
        registers[0],
	registers[1]);
      break;
    case 3:
      ((void *(*)(void *, void *, void *)) p)(
        registers[0],
	registers[1],
	registers[2]);    
      break;
  }
  
  cout << "returned = " << x << endl;
  cout << endl;
}

int main()
{
  void **registers = new (void *)[2], **v = registers;
  *v = (void *) 20;
  v++;
  *v = (void *) 26;
  //void *t = (void *) 20, *z = (void *) 26;
  execute(returner,registers);
  //execute((void *(*)(void *, void *)) nonreturner,registers);
}
