/*+
 * FLE.c
 *
 * The FLE class ties the other "fuzzy" classes together and orchestrates the
 * various steps of (fuzzy) logic, the actual machinery of which is contained in the
 * fuzzy_set_family and real_tensor classes.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * FLE::FLE(int id0)
 *
 *
 */
FLE::FLE(int id0)
     : item(id0),
       FLE_name(NULL),
       input_families(NULL),
       output_families(NULL),
       rules(0),
       inference_rules(NULL)

{
  #ifdef FLE_DEBUG
    cout << "<> FLE::FLE(int)" << endl;
    cout.flush();
  #endif
}



/**
 * FLE::FLE(universe *u, char *name0)
 *
 *
 */
FLE::FLE(universe *u, char *name0)
     : item(u, item_classe),
       FLE_name(new ascii(u, name0)),
       input_families(new list(u)),
       output_families(new list(u)),
       rules(0),
       inference_rules(NULL)
{
  #ifdef FLE_DEBUG
    cout << "<> FLE::FLE(universe *, char *): \"" << FLE_name->key() << "\"" << endl;
    cout.flush();
  #endif
}



/**
 * FLE::~FLE()
 *
 *
 */
FLE::~FLE()
{
  #ifdef FLE_DEBUG
    cout << "<> FLE::~FLE(): \"" << FLE_name->key() << "\"" << endl;
    cout.flush();
  #endif
}



/**
 * list *FLE::input()
 *
 *
 */
list *FLE::input()
{
  return input_families;
}



/**
 * list *FLE::output()
 *
 *
 */
list *FLE::output()
{
  return output_families;
}



/**
 * void FLE::add(fuzzy_set_family *fsf)
 *
 *
 */
void FLE::add(fuzzy_set_family *fsf)
{
  if (fsf != NULL)
    input_families->enqueue((item *)fsf);
}



/**
 * void FLE::add(fuzzy_output_family *fof)
 *
 *
 */
void FLE::add(fuzzy_output_family *fof)
{
  if (fof != NULL)
    output_families->enqueue((item *)fof);
}



/**
 * node *FLE::find_input_family(char *famname)
 *
 *
 */
node *FLE::find_input_family(char *famname)
{
  node *n = input_families->first();
  while (n != NULL) {
    if (!strcmp(famname, ((fuzzy_set_family *) n->key())->name()))
      return n;
    n = n->next();
  }
  return NULL;
}



/**
 * node *FLE::find_output_family(char *famname)
 *
 *
 */
node *FLE::find_output_family(char *famname)
{
  node *n = output_families->first();
  while (n != NULL) {
    if (!strcmp(famname, ((fuzzy_output_family *) n->key())->name()))
      return n;
    n = n->next();
  }
  return NULL;
}


/**
 * int FLE::find_input_index(char *famname)
 *
 *
 */
int FLE::find_input_index(char *famname)
{
  node *n = input_families->first();
  int i=0;
  while (n != NULL) {
    if (!strcmp(famname, ((fuzzy_set_family *) n->key())->name()))
      return i;
    n = n->next();
    i++;
  }
  return -1;
}



/**
 * int FLE::find_output_index(char *famname)
 *
 *
 */
int FLE::find_output_index(char *famname)
{
  node *n = output_families->first();
  int i=0;
  while (n != NULL) {
    if (!strcmp(famname, ((fuzzy_output_family *) n->key())->name()))
      return i;
    n = n->next();
    i++;
  }
  return -1;
}



/**
 * void FLE::add_rule(char **in_fams, char **in_members, char **out_fams, char **out_members)
 *
 * You may only create inference rules AFTER all input and output families
 * have been added to the FLE.  After you've added a rule (or deserialized
 * an FLE with an existing inference tensor) you may NOT add additional
 * input or output families (attempting to do so will result in unpredictable
 * behavior, and probable segmentation faults).
 */
void FLE::add_rule(char **in_fams, char **in_members, char **out_fams, char **out_members)
{
  if (inference_rules == NULL) {
    int rank0 = input_families->size(), i=0;
    int *dims0 = new int[rank0];
    node *n = input_families->first();
    while (n != NULL) {
      dims0[i] = ((fuzzy_set_family *) n->key())->size();
      n = n->next();
      i++;
    }
    inference_rules = new integer_tensor(item_universe, dims0, rank0);
    create_tensors(); /**137*/
    delete dims0;
  }

  int in_indices[input_families->size()], out_indices[output_families->size()];
  int out_dimensions[output_families->size()];
  for (int i=0; i<input_families->size(); i++) {
	int i1 = find_input_index(in_fams[i]);
	int i2 = ((fuzzy_set_family *) find_input_family(in_fams[i])->key())->find_index(in_members[i]);
    in_indices[i1] = i2;
  }

  for (int i=0; i<output_families->size(); i++) {
    fuzzy_output_family *fam = ((fuzzy_output_family *) find_output_family(out_fams[i])->key());
    out_dimensions[i] = fam->size();
    out_indices[find_output_index(out_fams[i])]
      = fam->find_index(out_members[i]);
  }

  inference_rules->set(in_indices,
                       integer_tensor::get_compound_index(output_families->size(),
                                                          out_dimensions,
                                                          out_indices));
  rules++;
}



/**
 * char ***FLE::input_name_array()
 *
 *
 */
char ***FLE::input_name_array()
{
  char ***ret = new (char **)[input_families->size()], ***dim = ret;
  node *n = input_families->first();
  while (n != NULL) {
    *dim = new (char *)[((fuzzy_set_family *) n->key())->members()->size()];
    char **val = *dim;
    node *n2 = ((fuzzy_set_family *) n->key())->members()->first();
    while (n2 != NULL) {
	  *val = ((fuzzy_set *) n2->key())->name();
	  val++;
	  n2 = n2->next();
	}
    n = n->next();
    dim++;
  }
  return ret;
}



/**
 * char ***FLE::output_name_array()
 *
 *
 */
char ***FLE::output_name_array()
{
  char ***ret = new (char **)[output_families->size()], ***dim = ret;
  node *n = output_families->first();
  while (n != NULL) {
    *dim = new (char *)[((fuzzy_output_family *) n->key())->members()->size()];
    char **val = *dim;
    node *n2 = ((fuzzy_output_family *) n->key())->members()->first();
    while (n2 != NULL) {
	  *val = ((ascii *) n2->key())->key();
	  val++;
	  n2 = n2->next();
	}
    n = n->next();
    dim++;
  }
  return ret;
}



/*
void FLE::calculate(double *crisp_array)
{
  if ((inference_rules == NULL)||(!rules))
    return;

  int imax, jmax;
  node *n;

  int out_size = 1;
  imax = output_families->size();
  double **out_doms = new (double *)[imax];
  n = output_families->first();
  for (int i=0; i<imax; i++) {
    jmax = ((fuzzy_output_family *) n->key())->size();
    out_doms[i] = new double[jmax];
    out_size *= jmax;
    for (int j=0; j<jmax; j++)
      out_doms[i][j] = 0;
    n = n->next();
  }

  imax = input_families->size();
  double **doms = new (double *)[imax];
  n = input_families->first();
  for (int i=0; i<imax; i++) {
    jmax = ((fuzzy_set_family *) n->key())->size();
    doms[i] = new double[jmax];

    double d = crisp_array[i];
    double *dd = doms[i];
    ((fuzzy_set_family *) n->key())->fuzzify(d, dd);

    n = n->next();
  }

  for (int k=0; k<rules; k++) {
    int r = k;
    int factor = rules;

    // Find the truth "product" for this rule.
    double product = 1;
    n = input_families->first();
    imax = input_families->size();
    for (int i=0; i<imax; i++) {
      factor /= ((fuzzy_set_family *) n->key())->size();
      int j = r / factor;
      product *= doms[i][j];
      r -= (j*factor);
      n = n->next();
    }

    // Use "product" to augment the output doms.
    n = output_families->first();
    r = inference_rules[k];
    factor = out_size;
    imax = output_families->size();
    for (int i=0; i<imax; i++) {
      factor /= ((fuzzy_output_family *) n->key())->size();
      int j = r / factor;
      out_doms[i][j] += product;
      r -= (j*factor);
      n = n->next();
    }
  }

  imax = output_families->size();
  n = output_families->first();
  for (int i=0; i<imax; i++) {
    node *n2 = ((fuzzy_output_family *) n->key())->members()->first(), *nmax = n2;
    double max = 0;
    jmax = ((fuzzy_output_family *) n->key())->size();
    for (int j=0; j<jmax; j++) {
      if (out_doms[i][j] >= max) {
        max = out_doms[i][j];
        nmax = n2;
      }
      n2 = n2->next();
    }
    cout << ((fuzzy_output_family *) n->key())->name() << "( ";
    cout << ((ascii *) nmax->key())->key() << " )" << endl;
    n = n->next();
  }
  cout << endl;
}
*/



/**
 * int *FLE::calculate(double *crisp_array)
 *
 *
 */
int *FLE::calculate(double *crisp_array)
{
  double **factor_array = get_doms(crisp_array);
  apply_inference(factor_array);
  double **composition = compose_output();
  return defuzzify_using_maximum(composition);
}



/**
 * void FLE::calculate(char **in_dims,
                    double *unordered_array,
                    char **out_dims,
                    char **out_vals)
 *
 *
 */
void FLE::calculate(char **in_dims,
                    double *unordered_array,
                    char **out_dims,
                    char **out_vals)
{
  double *crisp_array = new double[input_families->size()];
  for (int i=0; i<input_families->size(); i++)
    crisp_array[find_input_index(in_dims[i])] = unordered_array[i];
  int *indices = calculate(crisp_array);
  int i=0;
  node *n = output_families->first();
  fuzzy_output_family *outfam;
  while (n != NULL) {
    outfam = (fuzzy_output_family *) n->key();
    out_dims[i] = ascii::copy(outfam->name());
    out_vals[i] = ascii::copy(((ascii *) outfam->members()->find_by_index(indices[i]))->key());
    n = n->next();
    i++;
  }
}



/**
 * void FLE::create_tensors()
 *
 *
 */
void FLE::create_tensors()
{
  if (input_tensor == NULL) {
    int rank0 = input_families->size(), i=0;
    int *dims0 = new int[rank0];
    node *n = input_families->first();
    while (n != NULL) {
      dims0[i] = ((fuzzy_set_family *) n->key())->size();
      n = n->next();
      i++;
    }
    input_tensor = new real_tensor(item_universe, dims0, rank0);
    delete dims0;
  }
  if (output_tensor == NULL) {
    int rank0 = output_families->size(), i=0;
    int *dims0 = new int[rank0];
    node *n = output_families->first();
    while (n != NULL) {
      dims0[i] = ((fuzzy_output_family *) n->key())->size();
      n = n->next();
      i++;
    }
    output_tensor = new real_tensor(item_universe, dims0, rank0);
    delete dims0;
  }
}



/**
 * double **FLE::get_doms(double *crisp_values)
 *
 *
 */
double **FLE::get_doms(double *crisp_values)
{
  int input_rank = input_families->size();

  double **dom_array = new (double *)[input_rank], **dim = dom_array;
  node *n = input_families->first();
  double *crisp = crisp_values;
  for (int i=0; i<input_rank; i++) {
    *dim = new double[((fuzzy_set_family *) n->key())->size()];
    ((fuzzy_set_family *) n->key())->fuzzify(*crisp, *dim);
    dim++;
    crisp++;
    n = n->next();
  }

  return dom_array;

}



/**
 * void FLE::apply_inference(double **dom_array)
 *
 *
 */
void FLE::apply_inference(double **dom_array)
{
  input_tensor->apply_product_rule(dom_array);
  input_tensor->use_inference(inference_rules, output_tensor);
}



/**
 * double **FLE::compose_output()
 *
 *
 */
double **FLE::compose_output()
{
  if (output_tensor != NULL)
    return output_tensor->compose_by_dimension();
}



/**
 * int *FLE::defuzzify_using_maximum(double **composition)
 *
 *
 */
/* Returns a 1-D array of output indices */
int *FLE::defuzzify_using_maximum(double **composition)
{
  int output_rank = output_tensor->rank();
  double **dim = composition;
  double *value;
  int *ret = new int[output_rank], *runner = ret;
  for (int i=0; i<output_rank; i++) {
    value = *dim;
    double max = *value;
    for (int j=0; j<output_tensor->dimension(i); j++) {
      if (*value >= max) {
        max = *value;
        *runner = j;
      }
      value++;
    }
    runner++;
    dim++;
  }

  return ret;
}



/**
 * void FLE::serialize(ofstream &out)
 *
 *
 */
void FLE::serialize(ofstream &out)
{
  #ifdef FLE_DEBUG
    cout << "<> void FLE::serialize(ofstream &): \"" << FLE_name->key() << "\"" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t" << rules << '\t';
  ((item *) FLE_name)->write_id(out);
  out << '\t';
  ((item *) input_families)->write_id(out);
  out << '\t';
  ((item *) output_families)->write_id(out);
  out << '\t';
  if (inference_rules != NULL)
    ((item *) inference_rules)->write_id(out);
  else
    out << '.';
  out << '\n';
}



/**
 * void FLE::deserialize(ifstream &in)
 *
 *
 */
void FLE::deserialize(ifstream &in)
{
  #ifdef FLE_DEBUG
    cout << "<> void FLE::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[MAX_STRLEN];
  in >> s >> rules >> s;
  FLE_name = (ascii *) item_universe->reference(s);
  in >> s;
  input_families = (list *) item_universe->reference(s);
  in >> s;
  output_families = (list *) item_universe->reference(s);
  in >> s;
  if (*s == '.')
    inference_rules = NULL;
  else
    inference_rules = (integer_tensor *) item_universe->reference(s);
  delete s;
}

/*- end of file --------------------------------------------------------------*/

