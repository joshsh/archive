


argument::argument(classe *classe0, bool readwrite0)
          : argument_classe(classe0),
	    argument_readwrite(readwrite0)
{}



classe *argument::get_classe()
{
  return argument_classe;
}



bool argument::readwrite()
{
  return argument_readwrite;
}
