#include <iostream>
using namespace std;

#include "include/utilities/expanding_array.h"

void display(SNPL_expanding_array *array) {
  cout << array->size() << ":";
  for (int i=0; i<array->size(); i++) {
    cout << " " << (int) array->get(i);
  }
  cout << endl;
}

int main()
{
  SNPL_expanding_array *array = new SNPL_expanding_array(5,2);
  array->enqueue((void *) 1); display(array);
  array->enqueue((void *) 2); display(array);
  array->push((void *) 3); display(array);
  int i = (int) array->dequeue(); display(array);
  array->push((void *) 4); display(array);
  array->push((void *) 5); display(array);
  array->push((void *) 6); display(array);
  array->push((void *) 7); display(array);
  array->push((void *) 8); display(array);
  cout << "induzierte Spannung ----------" << endl;
  array->minimize();
  array->set(0, (void *) 9); display(array);
  //array->set(-1, (void *) 137);// display(array);
  //array->set(42, (void *) 1331);// display(array);
  cout << "array->get(1) = " << (int) array->get(1) << endl;
  //cout << "array->get(-1) = " << (int) array->get(-1) << endl;
  //cout << "array->get(42) = " << (int) array->get(42) << endl;
  cout << "Schweppes -----------" << endl;
  array->enqueue((void *) 10); display(array);
  array->enqueue((void *) 11); display(array);
  array->enqueue((void *) 12); display(array);
  cout << "Santraginus IV ------" << endl;
  array->remove(6); display(array);
  array->insert_after(6, (void *) 13); display(array);
  array->insert_before(6, (void *) 14); display(array);
  cout << "Grossmuenster ------" << endl;
  while (array->size()) {
    if (i%2)
      i = (int) array->pop();
    else
      i = (int) array->dequeue();
    display(array);
  }
  cout << "foo ------------" << endl;
  i = (int) array->dequeue(); display(array);
  array->minimize();
  array->minimize();
  cout << "This array is A. O. K." << endl;
  delete array;
}
