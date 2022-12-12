/* test for t_num_str_string_type function */

#include "tnumstr.c"

int
main (int argc, char **argv) {

  char *t_name[] = {
    "t_none",
    "t_int",
    "t_double"
  };

  struct str_type {
    char *s;
    int type;
  } sample[] = {
    {"123", t_int},
    {"-45", t_int},
    {"+0", t_int},
    {"123.456", t_double},
    {"+123.456", t_double},
    {"-123.456", t_double},
    {".456", t_double},
    {"123.", t_double},
    {"0.0", t_double},
    {"abc", t_none},
    {NULL, t_none}
  };
  int a, j, max;

  max = sizeof(sample)/sizeof(struct str_type);
  
  for (j=0; j<max; ++j) {
    if ((a = t_num_str_string_type (sample[j].s)) != sample[j].type)
      g_print ("%s is %s, but t_num_str_string_type returns %s.", sample[j].s, t_name[sample[j].type], t_name[a]);
  }
}