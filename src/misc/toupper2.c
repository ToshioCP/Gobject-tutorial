#include <glib-object.h>
#include <ctype.h>

char *
string_toupper (const char *s) {
  char *t, *t1;

  t1 = t = g_strdup (s);
  for (; *t1 != '\0'; ++t1)
    if (islower (*t1))
      *t1 = (char) toupper ((int) *t1);
  return t;
}

int
main (int argc, char **argv) {
  char *s, *t;

  s = "abc123";
  t = string_toupper (s);
  g_print ("s is %s\n", s);
  g_print ("t is %s\n", t);
  g_free (t);
  return 0;
}

