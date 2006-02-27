#include <iostream.h>
#include <stdio.h>

int main(){
  FILE *file1;//, *fopen();
  file1 = fopen("pro.c", "r");
  char ch=getc(file1);
  cout << "<" << ch << ">" << endl;
  char *Ch2;
  fgets(Ch2, 10, file1);
  //cout << "<" << Ch << ">" << endl;
  cout << "<" << Ch2 << ">" << endl;
  FILE *file2=fopen("pro.c", "w");
  fputs("HIHIHIHIHIHIHIHIHI", file2);
  fgets(Ch2, 10, file1);
  cout << "<" << Ch2 << ">" << endl;
  fseek(file2, 5, 0);
  fputs("..........", file2);
  FILE *file3=fopen("pro.c", "a");
  fseek(file3, 8, 0);
  fputs("donteraseanything", file3);
}