#include <iostream>
//#include <fstream>
//#include <typeinfo>

using namespace std;



class caste //: item
{
  void (*foo_ptr)(class A *, char *);

public:

  caste(void (*)(class A *, char *));

  void foo(class A *, char *);

};



class A
{

public:

  class caste *item_caste;

  static void foo(A *, char *);

  void foo(char *);

  A();

};



void A::foo(A *a, char *s)
{
  cout << "An A says: " << s << endl;
}

void A::foo(char *s)
{
  item_caste->foo(this, s);
}

A::A()
{

}

//void (*A::foo_ptr)(A *, char *) = NULL;
//void (*A::bar_wrapper_ptr)(void *, char *) = NULL;


caste::caste(void (*foo_ptr0)(A *, char *))
{
  foo_ptr = foo_ptr0;
}

void caste::foo(A *a, char *s)
{
  (foo_ptr)(a, s);
}


int main()
{
  caste *A_caste = new caste(A::foo);


  //A::bar_wrapper_ptr = A::bar_wrapper;

  A *a = new A();
  //a->foo("hi");
  //A::foo(a, "hi there");

  a->foo("hello world");

}
