#include "files.h"

//changes a "*.map" file to a fancy-looking whatchamacallit file
void makeCool(char *name1){
  fstream file1, file2;
  file1.open(name1, ios::in);
  char ch=file1.get();
  int iii=0;
  while (!file1.eof()){
    if (ch == 10) iii++;
    ch=file1.get();
  };
  file1.close();
  file1.open(name1, ios::in);
  file2.open(addStrings(stem(name1),".cool"), ios::out);
  int jjj;
  while( !file1.eof() ){
    for( jjj=0; jjj<iii; jjj++) file2.put(32);
    iii=iii-1;
    ch=file1.get();
    while ((!file1.eof())&&(ch != 10)){
      if(ch == '*'){ file2.put('_'); file2.put('/'); };
      if(ch == 32){ file2.put(32); file2.put(32); };
      ch=file1.get();
    };
    if (ch == 10) file2.put(10);
  };  
}

int main(){
  char name1[20];
  cout << endl << "File = " ; cin >> name1; cout << endl;
  makeCool(name1);
  cout << endl;
}