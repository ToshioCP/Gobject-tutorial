#include <glib-object.h>
#include <ctype.h>

void
string_toupper (char *s) {
  for (; *s != '\0'; ++s)
    if (islower (*s))
      *s = (char) toupper ((int) *s);
}

int
main (int argc, char **argv) {
  char *s, *t;
  s = "abc123";
  t = g_strdup (s);
  string_toupper (t);
  g_print ("s is %s\n", s);
  g_print ("t is %s\n", t);
  g_free (t);
  return 0;
}

