#include <iostream>

#include "U.h"

char *cin_string()
{
  char *s = new char[100];
  cout << "?: ";
  cin >> s;
  int len = strlen(s);
  char *s2 = new char[len+1];
  strcpy(s2, s);
  delete s;
  return s2;
}

//############################################################

universe *u;

/**
  [NULL] [paper]::fill_rect([frame],[color])
*/
/*
void func1(item **args)
{
  SDL_Rect *rect = new SDL_Rect();
  paper *p = (paper *) args[1];
  frame *f = (frame *) args[2];
  color *c = (color *) args[3];
  p->fill_rect(c, f);
}
*/

void new_ascii(item **args)
{
  // We really need universe to be an item derivative, so we can
  // pass a universe as an argument.
  //args[0] = new ascii((ascii *) args[1], cin_string());

  args[0] = (item *) new ascii(u, cin_string());
}



//############################################################

int main(int argc, char *argv[])
{
  cin.unsetf(ios::skipws);

  function *f = new function(new_ascii, "new_ascii", NULL, 0);

  u = new universe("function test");

  item **args = new (item *)[5];
  //args[1] = (item *) u;

  f->execute(args);

  ofstream out ("functest.U");
  u->serialize(out);
  out.close();

  delete u;
  delete f;
}
