#include <glib-object.h>
#include <ctype.h>

char *
string_toupper (const char *s) {
  char *t;

  t = g_strdup (s);
  for (; *t != '\0'; ++t)
    if (islower (*t))
      *t = (char) toupper ((int) *t);
  return t;
}

int
main (int argc, char **argv) {
  char *s, *t;

  s = "abc123";
  t = string_toupper (t);
  g_print ("s is %s\n", s);
  g_print ("t is %s\n", t);
  g_free (t);
  return 0;
}

