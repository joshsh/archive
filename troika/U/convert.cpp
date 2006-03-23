#include "U.h"



// Note: multiple exit points
color *find_or_create(universe *u, list *l, color24 *c24)
{
  node *n = l->first();
  while (n != NULL) {
    if (((color *) n->key())->equals(c24))
      return (color *) n->key();
    n = n->next();
  }
  color *c = new color(u, c24);
  l->enqueue((item *) c);
  return c;
}



int main()
{
  universe *u = new universe("patterns");
  list *colors = new list(u);
  list *names = new list(u);
  list *grids = new list(u);
  grid *g;
  color24 *c24;

  char *s2 = new char[100], *s, *s3, *s4 = new char[100];
  int rows, cols;
  strcpy(s2, "../A/patterns/");
  s = s2 + strlen(s2);
  ifstream in ("../A/data/patterns");
  in >> s;

  while (!in.eof()) {
    names->enqueue((item *) new ascii(u, s));
    s3 = s + strlen(s);
    strcpy(s3, ".A");
    ifstream in2 (s2);
    in2 >> rows >> cols >> s4;
cout << "Opening \"" << s2 << "\" (" << rows << ", " << cols << ", " << s4 << ")." << endl;
cout.flush();
    c24 = new color24(s4);
    g = new grid(u, find_or_create(u, colors, c24), rows, cols);
    grids->enqueue((item *) g);
    delete c24;
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
        in2 >> s4;
        c24 = new color24(s4);
        g->set(i, j, find_or_create(u, colors, c24));
        delete c24;
      }
    }
    in2.close();
    in >> s;
  }
  in.close();

  //*/

  ofstream out ("patterns");
  u->serialize(out);
  out.close();

  //*/

  delete u;
}
