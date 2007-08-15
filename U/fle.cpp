/**
 * fle.cpp
 *
 * Last edit: 6/6/04
 *
 * This is a test routine for the "fuzzy logic" classes:
 *   {fuzzy_set, fuzzy_set_family, fuzzy_output_family, FLE}
 *
 * It includes a new "substitute" parser as I couldn't get Bison, Flex and GNU C++
 * to be friends.

Below we are working off of these input and output families:

	temp : cool 60, 65 warm 70, 80 hot;
	volume : small 100, 200 medium 200, 300 large;
	time : morning 5, 7 day 17, 20 evening;

	power : off, low, med, high;
	mood : contrary, obsequious, apathetic;

*/

#include "U.h"



/**
 * void display_FLE(FLE *fle)
 *
 * For debugging purposes.
 */
void display_FLE(FLE *fle)
{
  char ***input_name_array = fle->input_name_array();
  char ***output_name_array = fle->output_name_array();
  int ii=0;
  node *n = fle->input()->first();
  while (n != NULL) {
    cout << ((fuzzy_set_family *) n->key())->name() << ":" << endl;
    for (int jj=0; jj<((fuzzy_set_family *) n->key())->members()->size(); jj++)
      cout << "\t" << input_name_array[ii][jj] << endl;
    ii++;
    n = n->next();
  }
  ii=0;
  n = fle->output()->first();
  while (n != NULL) {
    cout << ((fuzzy_output_family *) n->key())->name() << ":" << endl;
    for (int jj=0; jj<((fuzzy_output_family *) n->key())->members()->size(); jj++)
      cout << "\t" << output_name_array[ii][jj] << endl;
    ii++;
    n = n->next();
  }
  delete input_name_array;
  delete output_name_array;
}



/**
 * int main(int argc, char *argv[])
 *
 * Two arguments are expected:
 * 1) the name of the Fuzzy Logic Engine.  Include the path name if the input
 *    files are not in the same directory as the executable.
 * 2) a name for the "universe" file the program will save to
 *
 * E.g.
 *   ./fle heater heater.U
 *
 * The parser will look for the FLE definition, inference rules, and input data
 * in the files fle_def, fle_inf, and fle_data respectively.
 */
int main(int argc, char *argv[]) {

  if (argc != 3) {
    cout << "Wrong number of arguments.";
    exit(1);
  }

  ifstream in;
  ofstream out;

  universe *u = new universe("fle.cpp output");

  char *a = argv[1], *temp;
  int len = strlen(a);
  FLE *fle = new FLE(u, a);
  char *fle_def = new char[len+5];
  strcpy(fle_def, a);
  temp = fle_def+len;
  strcpy(temp, "_def");
  char *fle_inf = new char[len+5];
  strcpy(fle_inf, a);
  temp = fle_inf+len;
  strcpy(temp, "_inf");
  char *fle_data = new char[len+6];
  strcpy(fle_data, a);
  temp = fle_data+len;
  strcpy(temp, "_data");



  //LOAD FLE ###################################################################

  char *s = new char[100];
  int in_rank, out_rank;

///*
  in.open(fle_def);
  in >> in_rank >> out_rank;

  for (int i=0; i<in_rank; i++) {
	in >> s;
    fuzzy_set_family *fsf = new fuzzy_set_family(u, s);
    fle->add(fsf);
    in >> s;
    while (*s != ';') {
	  double min, max;
	  in >> min >> s >> max;
	  fsf->add(s, min, max);
	  in >> s;
    }
  }

  for (int i=0; i<out_rank; i++) {
	in >> s;
	fuzzy_output_family *fof = new fuzzy_output_family(u, s);
	fle->add(fof);
	in >> s;
	while (*s != ';') {
      in >> s;
      fof->add(s);
      in >> s;
    }
  }
  in.close();
//*/

  char ***input_name_array = fle->input_name_array();
  char ***output_name_array = fle->output_name_array();

//display_FLE(fle);

  //LOAD INFERENCE RULES #######################################################

  in.open(fle_inf);
  int lines;
  in >> lines >> in_rank >> out_rank;
  char **in_dims = new (char *)[in_rank];
  char **in_vals = new (char *)[in_rank];
  char **out_dims = new (char *)[out_rank];
  char **out_vals = new (char *)[out_rank];
  char **dim, **val;

  for (int i=0; i<lines; i++) {
	dim = in_dims;
	val = in_vals;
    for (int j=0; j<in_rank; j++) {
	  in >> s;
	  int len = strlen(s);
	  *dim = new char[len+1];
	  strcpy(*dim, s);
	  in >> s >> s;
	  len = strlen(s);
	  *val = new char[len+1];
	  strcpy(*val, s);
	  in >> s;
	  dim++;
	  val++;
    }

    in >> s;
    dim = out_dims;
    val = out_vals;

    for (int j=0; j<out_rank; j++) {
	  in >> s;
	  int len = strlen(s);
	  *dim = new char[len+1];
	  strcpy(*dim, s);
	  in >> s >> s;
	  len = strlen(s);
	  *val = new char[len+1];
	  strcpy(*val, s);
	  in >> s;
	  dim++;
	  val++;
    }

    fle->add_rule(in_dims, in_vals, out_dims, out_vals);
  }
  in.close();
  for (int i=0; i<in_rank; i++) {
    delete in_dims[i];
    delete in_vals[i];
  }
  delete in_dims;
  delete in_vals;

  //LOAD DATA ##################################################################

  char *data_id = new char[100];
  in.open(fle_data);
  in >> lines >> in_rank >> out_rank;
  in_dims = new (char *)[in_rank];
  double *in_crisp = new double[in_rank];
  double *crisp;
  for (int i=0; i<lines; i++) {
	dim = in_dims;
	crisp = in_crisp;
	in >> data_id;
	cout << data_id << endl;
    for (int j=0; j<in_rank; j++) {
	  in >> s;
	  int len = strlen(s);
	  *dim = new char[len+1];
	  strcpy(*dim, s);
	  in >> s >> *crisp;
      in >> s;
cout << *dim << " = " << *crisp << endl;
	  dim++;
	  crisp++;
    }
    int *indices = fle->calculate(in_crisp);
    for (int i=0; i<out_rank; i++) {
	  cout << "\t" << out_dims[i] << " = " << output_name_array[i][indices[i]] << endl;
    }
  }
  in.close();
  for (int i=0; i<in_rank; i++) {
    delete in_dims[i];
  }

  delete s;
  delete in_dims;
  delete in_crisp;

  for (int i=0; i<out_rank; i++) {
    delete out_dims[i];
    delete out_vals[i];
  }
  delete out_dims;
  delete out_vals;

  delete input_name_array;
  delete output_name_array;

  //############################################################################



  out.open(argv[2]);
  u->serialize(out);
  out.close();

  delete u;

  in.open(argv[2]);
  u = new universe(in);
  in.close();

  out.open(argv[2]);
  u->serialize(out);
  out.close();

}
