#include "../U.h"



void get_line(char *s)
{
  char *runner = s;
  *runner = cin.get();
  while (*runner != '\n')
  {
    runner++;
    *runner = cin.get();
  }
  *runner = '\0';
  //cout << "s = " << s << "." << endl;
}



int main()
{
//system("clear");
//cout << (int) '/' << " " << (int) 'a' << endl; exit(0);
//cout << strcmp("a", "b") << endl; exit(0);  
  create_classes();
  create_functions();

  universe *u;
  list *l, *l0;
  ascii *a;
  ifstream in;
  ofstream out;
  
/*// Initialization stuff
  u = new universe("first command-line universe");
  a = new ascii(u, "phalansterianism");
  l = new list(u);
  l->enqueue((item *) a);
  u->add_special_reference((item *) l, "asciis");  
  out.open("local.U");
  u->serialize(out);
  out.close();
  delete u;
//*/

///*
//cout << "Horst" << endl; cout.flush();
  in.open("local.U");
  u = new universe(in);
  in.close();
//cout << "Weimar" << endl; cout.flush();
//procedure *p = new procedure(u, "input_new_ascii"
  l0 = (list *) u->reference_special("asciis");
  l = l0;
//cout << "imipramine" << endl; cout.flush();
  char *s = new char[100], *runner;
  cout << endl << "Input new strings.  '\\q' exits." << endl;
  get_line(s); //getline(cin, s); //cin >> s;
//cout << "Marrakesh" << endl; cout.flush();
  while (true) {
    if (*s != '\\') {
      ascii *a = new ascii(u, s);
      l->enqueue((item *) a);    
    } else {
      if (!strcmp(s,"\\q")) {
        break;  
      } else if (!strcmp(s,"\\-")) {
        l = l0;
      } else if (!strcmp(s,"\\system")) {
        get_line(s);
        system(s);
      } else if (!strcmp(s,"\\lists")) {
        u->display_special_references();
      } else if (!strcmp(s,"\\all")) {
        node *n = l->first();
        while (n != NULL) {
          cout << ((ascii *) n->key())->key() << endl;
          n = n->next();
        }
      } else {
        s++;
        l = (list *) u->reference_special(s);
        if (l == NULL) {
          l = new list(u);
          u->add_special_reference((item *) l, s);
        }
	s--;
      }
    }
    //cin >> s;
    get_line(s);  
  }
  delete s;
  
  out.open("local.U");
  u->serialize(out);
  out.close();
  delete u;
//*/
  
  delete functions;
  delete classes;
}
