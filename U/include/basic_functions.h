// r:universe w:ascii
void input_new_ascii(item **registers)
{
  char *s = new char[100];
  cout << "?: ";
  cin >> s;  //### Beware of whitespace...
  cout << endl;
  registers[1] = (item *) new ascii((universe *) *registers, s);
  delete s;
}
