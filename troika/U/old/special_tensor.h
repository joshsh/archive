class special_tensor
{
  int tensor_rank, tensor_size;
  int *tensor_dimensions;
  int *tensor_values;
  char **dim_names;
  char ***dim_values;

public:

  special_tensor(class list *);

  int get(char **, char **);

  void set(char **, char **, int);

};



special_tensor::special_tensor(list *l)
{
  tensor_rank = l->size();
  dim_names = new (char *)[tensor_rank];
  dim_values = new (char **)[tensor_rank];
  tensor_dimensions = new int[tensor_rank];
  tensor_size = 1;

  list *members;
  node *n = l->first(), *n2;
  int i=0, j, sz;
  if (!strcmp(n->key()->get_classe()->name(), "fuzzy_output_family")) {
    while (n != NULL) {
      members = ((fuzzy_output_family *) n->key())->members();
      sz = members->size();
      tensor_dimensions[i] = sz;
      tensor_size *= sz;
      dim_names[i] = ((fuzzy_output_family *) n->key())->name();
      dim_values[i] = new (char *)[sz];
      n2 = members->first();
      j=0;
      while (n2 != NULL) {
        dim_values[i][j] = ascii::copy(((ascii *) n->key())->key());
        j++;
        n2 = n2->next();
      }
      n = n->next();
      i++;
    }
  } else {
    while (n != NULL) {
      members = ((fuzzy_set_family *) n->key())->members();
      sz = members->size();
      tensor_dimensions[i] = sz;
      tensor_size *= sz;
      dim_names[i] = ((fuzzy_set_family *) n->key())->name();
      dim_values[i] = new (char *)[sz];
      n2 = members->first();
      j=0;
      while (n2 != NULL) {
        dim_values[i][j] = ascii::copy(((fuzzy_set *) n->key())->name());
        j++;
        n2 = n2->next();
      }
      n = n->next();
      i++;
    }
  }

  tensor_values = new int[tensor_size];
}



int special_tensor::get(char **dim, char **pos)
{
  int index=0, factor = tensor_size;
  for (int i=0; i<tensor_rank; i++) {
    int j;
    factor /= tensor_dimensions[i];
    if (!strcmp(dim[i], dim_names[i])) {
      char *pos_i = pos[i];
      char **dim_values_i = dim_values[i];
      for (j=0; j<tensor_dimensions[i]; j++) {
        if (!strcmp(pos_i, dim_values_i[j]))
          break;
      }
      index += factor*j;
    }
  }
  return index;
}



void special_tensor::set(char **dim, char **pos, int i)
{
  tensor_values[get(dim, pos)] = i;
}
