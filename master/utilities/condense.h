#include <fstream.h>
#include "/home/jshinavi/master/char.h"

  //practical for files with lots of repeating digits
  //no repeats longer than 245 bytes allowed!
//general file condensation procedure======================================

void condense(char *name1, char *name2){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  char ch=file1.get(), ch2=ch;
  ch=file1.get();
  int dlen;
  while (!file1.eof()){
    dlen=0;
    while (ch2 == ch){ dlen++; ch2=ch; ch=file1.get(); };
    dlen++;
    file2.put(ch2);
    file2.put(dlen+10);
    while (ch == 10){ ch=file1.get(); file2.put(10);};
    ch2=ch;
    ch=file1.get();
  };
}

void expand(char *name1, char *name2){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  char ch=file1.get();
  while (ch == 10){ file2.put(10); ch=file1.get(); };
  char dlen=file1.get();
  int iii;
  while (!file1.eof()){
    for (iii=0; iii<dlen-10; iii++) file2.put(ch);
    ch=file1.get();
    while (ch == 10){ file2.put(10); ch=file1.get(); };
    dlen=file1.get();
  };
}

  //for files with only one resp. two characters (besides the line feed)
  //always very practical unless the file looks more or less like a 
  //checkerboard, or a vertical American flag, even then it's not much longer
//unary and binary file condensation procedures============================

void condenseUnary(char *name1){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(addStrings(stem(name1), ".unaC"), ios::out);
  char ch=file1.get();
  int count=0;
  int feeds=0;
  while (ch == 10){ feeds++; ch=file1.get(); };
  file2.put(ch);
  for (int iii=0; iii<feeds; iii++) file2.put(10);
  while (!file1.eof()){
    count=1;
    while ((ch != 10)&&(!file1.eof())){ ch=file1.get(); count++; };
    file2.put(count+10);
    if (!file1.eof()) file2.put(10);
    while (ch == 10){ file2.put(10); ch=file1.get(); };
  };
}

void expandUnary(char *name1){
  if (!endsWith(name1, ".unaC")) return;
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(addStrings(stem(name1), ".len"), ios::out);
  char chX=file1.get();
  char ch=file1.get();
  while (ch == 10){ file2.put(10); ch=file1.get(); };
  int count;
  while (!file1.eof()){
    for(count=0; count < ch-10; count++) file2.put(chX);
    ch=file1.get(); ch=file1.get();
    while(ch == 10){ file2.put(10); ch=file1.get(); };
  }; 
}

void condenseBinary(char *name1){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(addStrings(stem(name1), ".binC"), ios::out);
  char ch=file1.get();
  while (ch == 10){ ch=file1.get(); };
  char ch1=ch, ch2, chX;
  while ((ch == ch1)||(ch == 10)) ch=file1.get();
  ch2=ch;
  if(file1.eof()){ condenseUnary(name1); return; };
  file1.close();
  file1.open(name1, ios::in);
  file2.put(ch1);
  file2.put(ch2);
  file2.put(10);
  ch=file1.get();
  int count;
  while (!file1.eof()){
    while (ch == 10){ file2.put(10); ch=file1.get(); };
    chX=ch;
    file2.put(ch);
    while ((ch != 10)&&(!file1.eof())){
      count=0;
      while (ch == chX){ count++; ch=file1.get(); };
      file2.put(count+10);
      chX=ch;       
    };
  };
}

void expandBinary(char *name1){
  int iii;
  if (!endsWith(name1, ".binC")) return;
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(addStrings(stem(name1), ".map"), ios::out);
  char ch, ch1=file1.get(), ch2=file1.get(), chX, chY, chZ;
  ch=file1.get(); ch=file1.get();
  while (!file1.eof()){
    chX=ch; chY=ch1;
    if (chY == chX) chY=ch2;
    ch=file1.get();
    while ((!file1.eof())&&(ch != 10)){
      for (iii=0; iii < ch-10; iii++) file2.put(chX);
      ch=file1.get();
      chZ=chX; chX=chY; chY=chZ;
    };
    if (ch == 10) file2.put(10);
    ch=file1.get();
    while (ch == 10){ file2.put(10); ch=file1.get(); };
  };
}