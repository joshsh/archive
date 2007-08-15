#include <iostream>

using namespace std;

#include "item.h"
#include "node.h"
#include "list.h"
#include "caste.h"

#include "item.c"
#include "node.c"
#include "list.c"
#include "caste.c"

/*
class classe
{

};
*/

class A
{

public:

  static caste *item_caste;

  A() { cout << "Created an A." << endl; }

  void non_virtual() { cout << "This is a call to a non-virtual function." << endl; }

  int static_non_virtual(int i) {
    cout << "I am a static non-virtual function." << endl;
    return i;
  }

  virtual void foo(char *s) {
    cout << "An A says: " << s << endl;
  }

  virtual void print_stat() { cout << item_caste->id() << endl; }
  virtual void set_caste(caste *c) { item_caste = c; }

  //void caste() { cout << "not legal." << endl; }

};

caste *A::item_caste = NULL;

class B : public A
{
public:

  static caste *item_caste;

  B() { cout << "Created a B." << endl; }

  virtual void foo(char *s) {
    cout << "A B says: " << s << endl;
  }

  virtual void print_stat() { cout << item_caste->id() << endl; }
  virtual void set_caste(caste *c) { item_caste = c; }

};

caste *B::item_caste = NULL;

int main() {

  //void (*class_constructor)(void);// = A::A;
  int (*ptr)(int);
  //int (TMyClass::*pt2Member)(float, char, char);
  ptr = A::static_non_virtual;
  //A *(*constructor)(void); // = A::A;
  void (*set_class_caste)(caste *);

  //caste *A_caste = new caste(0, A::A, A::set_caste);
  //caste *B_caste = new caste(1, B::B, B::set_caste);
  //caste *A_caste = new caste(0, NULL, NULL);
  //caste *B_caste = new caste(1, NULL, NULL);

  A *a = new A();
  A *b = new B();

  a->non_virtual();
  a->foo("hello");
  b->foo("world");

  a->print_stat();
  b->print_stat();

}
