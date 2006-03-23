#include "../U.h"

int main()
{
  create_classes();
  create_functions();

  universe *u = new universe("new test universe");
  ascii *a = new ascii(u, "howdy ho!");
  list *l = new list(u);
  l->enqueue((item *) a);
  u->add_special_reference((item *) l, "asciis");
  procedure *p = new procedure(u, "input_new_ascii");
  l = new list(u);
  l->enqueue((item *) p);
  u->add_special_reference((item *) l, "procedures");
  //p->display();
  
  ofstream out ("local.U");
  u->serialize(out);
  out.close();
  delete u;

///*
  ifstream in ("local.U");
  u = new universe(in);
  in.close();
  a = new ascii(u, "analgesic nephrosis");
  l = (list *) u->reference_special("asciis");
  l->enqueue((item *) a);
  out.open("local.U");
  u->serialize(out);
  out.close();
  delete u;
//*/
  
  delete functions;
  delete classes;
}
