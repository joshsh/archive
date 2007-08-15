/*+
 * FLE.h
 *
 * The FLE class ties the other "fuzzy" classes together and orchestrates the
 * various steps of (fuzzy) logic, the actual machinery of which is contained in the
 * fuzzy_set_family and real_tensor classes.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class FLE : item
{
  static class classe *item_classe;

  class list *input_families, *output_families;
  class ascii *FLE_name;

  class integer_tensor *inference_rules;
  int rules;

  class real_tensor *input_tensor, *output_tensor;

public:

  FLE(int);

  FLE(class universe *, char *);

  virtual ~FLE();

  class list *input();

  class list *output();

  void add(fuzzy_set_family *);

  void add(fuzzy_output_family *);

  class node *find_input_family(char *);

  class node *find_output_family(char *);

  int find_input_index(char *);

  int find_output_index(char *);

  void add_rule(char **, char **, char **, char **);

  char ***input_name_array();

  char ***output_name_array();

  //void calculate(double *);

  int *calculate(double *);

  void calculate(char **, double *, char **, char **);

  void create_tensors();

  double **get_doms(double *);

  double **compose_output();

  void apply_inference(double **);

  int *defuzzify_using_maximum(double **);

  virtual void serialize(ofstream &);

  virtual void deserialize(ifstream &);

  char *name();

  static void set_classe(classe *c) { item_classe = c; }
  virtual classe *get_classe() { return item_classe; }
  static FLE *constructor_wrapper(int id0) { return new FLE(id0); }

};

classe *FLE::item_classe = NULL;

/*- end of file --------------------------------------------------------------*/
