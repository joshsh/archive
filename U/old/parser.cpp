



void input_family(FLE *fle, ifstream &in)
{
  char *s = new char[100], *s2;// = new char[100];
  in >> s >> s;
  fuzzy_set_family *fsf = new fuzzy_set_family(((item *) fle)->get_universe(), s);
  in >> s;
  bool stop = false, first = true;
  double min=0, max=0;
  while (!stop) {
    if (!first)
      in >> min;
    in >> s;
    s2 = s + strlen(s) - 1;
    if (*s2 == ';') {
      *s2 = '\0';
      max = min;
      stop = true;
    } else {
      s2++;
      in >> s2;
      int len = strlen(s2);
      s2 += len-1;
      *s2 = '\0';
      s2 -= len-1;
      max = atof(s2);
    }
    if (first)
      min = max;
    fsf->add(s, min, max);
  }
  delete s;
  delete s2;
  fle->add(fsf);
}

in
{
	temp : cool 60, 65 warm 70, 80 hot;
	volume : small 100, 200 medium 200, 300 large;
	time : morning 5, 7 day 17, 20 evening;
}
void in_segment(FLE *fle, ifstream &in)
{
  char *s = new char[1000];
  in >> s;
  while (
  delete s;
}




  FLE *fle = new FLE(u, "test");
  char *s = new char[10];
  for (int i=0; i<4; i++) {
    in >> s;
    if (!strcmp(s, "in"))
      in_segment(fle, in);
    else if (!strcmp(s, "out"))
      out_segment(fle, in);
    else if (!strcmp(s, "rules"))
      rules_segment(fle, in);
    else
      data_segment(fle, in);
  }

fle_file:	in_segment out_segment rules_segment data_segment {}
		;

in_segment:	BEGIN_IN OPEN_BRACE input_families CLOSE_BRACE {}
		;

out_segment:	BEGIN_OUT OPEN_BRACE output_families CLOSE_BRACE {}
		;

rules_segment:	BEGIN_RULES OPEN_BRACE rules CLOSE_BRACE {}
		;

data_segment:	BEGIN_DATA OPEN_BRACE data CLOSE_BRACE {}
		;

/** input segment *//*========================================================*/

input_families:	input_families input_family {}
		| input_family {}
		;

input_family:	ID COLON input_args SEMICOLON {}
		;

input_args:	ID {}
		| input_args DOUBLE COMMA DOUBLE ID {}
		;

/** output segment *//*=======================================================*/

output_families:	output_families output_family {}
			| output_family {}
			;

output_family:		ID COLON output_args SEMICOLON {}
			;

output_args:		ID {}
			| output_args COMMA ID {}
			;

/** rules segment *//*========================================================*/

rules:		rule {}
		| rules rule {}
		;

rule:		rule_args COLON rule_args SEMICOLON {}
		;

rule_args:	state {}
		| rule_args COMMA state {}
		;

/** data segment *//*=========================================================*/

data:		data_unit {}
		| data data_unit {}
		;

data_unit:	ID COLON data_args SEMICOLON {}
		;

data_args:	crisp_state {}
		| data_args COMMA crisp_state {}
		;	

/** misc *//*=================================================================*/

crisp_state:	ID OPEN_PAREN DOUBLE CLOSE_PAREN {}
		;

state:		ID OPEN_PAREN ID CLOSE_PAREN {}
