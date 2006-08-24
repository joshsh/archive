#include <iostream>
using namespace std;

// Parse a char string (32-bit) word by word rather than char by char:
// 	char *ch = ...
// 	void *p = *((void **) ch);

// Output:
// 	4 foobar 1651470182 boof
int main()
{
  char *foo = "foobar";
  void *p = *((void **) foo);
  cout << sizeof(void *) << " "
       << foo << " " << (int) p << " "
       << (char) (((int) p / (256*256*256)))
       << (char) (((int) p / (256*256)) % 256)
       << (char) (((int) p / (256)) % (256*256))
       << (char) (((int) p) % (256*256*256))
       << endl;
}
