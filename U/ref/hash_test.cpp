//Note: this test requires hash_table's private members to be made public.

#include <iostream>
using namespace std;

#include "include/utilities/hash_table.h"

int main()
{
  cout << sizeof(int) << " " << sizeof(void *) << endl;
  cout << ((-101)%100) << endl;
  
  hash_table *ht = new hash_table(17,0,0);

  int big = 2345209834757, big2;
  for (int i=0; i<10; i++) {
    big2 = ~big+137;
    cout << i << "]\t" << big << "\t" << big2 << endl;
    ht->add((void *) big, (void *) big2);
    big = (big2*2)-234867;
  }
  
  for (int i=0; i<ht->table_size; i++) {
    cout << i << ")\t" << (int) ht->entries[i*2]
              << "\t" << (int) ht->entries[(i*2)+1] << endl;
  }
  
  cout << "Retrieve(157691141) = " << (int) ht->retrieve((void *) 157691141) << endl;
  cout << "Retrieve(630999159) = " << (int) ht->retrieve((void *) 630999159) << endl;  
  cout << "Retrieve(42) = " << (int) ht->retrieve((void *) 42) << endl;
  
  delete ht;
}
