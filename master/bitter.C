#include <iostream.h>
#include "graphics.h"
#include "rand48.h"

double rval;

void srand(double val){
  if (val < 0) val=-val;
  while (val > 1) val=val-1;
  rval=val;
}

double random(){
  rval=100*rval*rval;
  while (rval > 1) rval=rval-1;
  return rval;
}

int main(){
  Window win=createWindow(60, 60, 60, 60, "bitmapper");
  double seeder;
  double zen;
  cout << "seed value = "; cin >> seeder; cout << endl;
  srand(seeder);
  int i, j;
  for (i=0; i<5; i++){
    for (j=0; j<5; j++){
      zen=random();
      if (zen < .5) setColor(1);
      else setColor(40);
      fillRectangle(win, 5+(i*5), 5+(j*5), 5+((i+1)*5), 5+((j+1)*5));
    };
  };
  cout << "are you finished? "; cin >> seeder; cout << endl;
}