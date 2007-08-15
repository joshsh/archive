#include <iostream>

void *global_ptr;

void ptr_test(void *global_ptr0)
{
  global_ptr = global_ptr0;
  //cout << "result = " << ((void (*)(int, int)) global_ptr)(1, 2) << endl;
  printf("result = %i\n", ((int (*)(int, int)) global_ptr)(1, 2));
}

void ptr_test2(void *global_ptr0)
{
  global_ptr = global_ptr0;
  ((void (*)(void)) global_ptr)();
}

int function(int a, int b)
{
  printf("stumpig.\n");
  return a+b;
}

void func2() { printf("hi there\n"); }

int main()
{
  int (*func_ptr)(int, int) = function;

  void (*func2_ptr)(void) = func2;

  ptr_test((void *) func_ptr);

  ptr_test2((void *) func2);

}
