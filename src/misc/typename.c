#include <glib-object.h>

int
main (int argc, char **argv) {
#define MAXTYPE 24
  GType types[MAXTYPE] = {
    G_TYPE_INVALID,
    G_TYPE_NONE,
    G_TYPE_INTERFACE,
    G_TYPE_CHAR,
    G_TYPE_UCHAR,
    G_TYPE_BOOLEAN,
    G_TYPE_INT,
    G_TYPE_UINT,
    G_TYPE_LONG,
    G_TYPE_ULONG,
    G_TYPE_INT64,
    G_TYPE_UINT64,
    G_TYPE_ENUM,
    G_TYPE_FLAGS,
    G_TYPE_FLOAT,
    G_TYPE_DOUBLE,
    G_TYPE_STRING,
    G_TYPE_POINTER,
    G_TYPE_BOXED,
    G_TYPE_PARAM,
    G_TYPE_OBJECT,
    G_TYPE_GTYPE,
    G_TYPE_VARIANT,
    G_TYPE_CHECKSUM
  };
  char *type_s[MAXTYPE] = {
    "G_TYPE_INVALID",
    "G_TYPE_NONE",
    "G_TYPE_INTERFACE",
    "G_TYPE_CHAR",
    "G_TYPE_UCHAR",
    "G_TYPE_BOOLEAN",
    "G_TYPE_INT",
    "G_TYPE_UINT",
    "G_TYPE_LONG",
    "G_TYPE_ULONG",
    "G_TYPE_INT64",
    "G_TYPE_UINT64",
    "G_TYPE_ENUM",
    "G_TYPE_FLAGS",
    "G_TYPE_FLOAT",
    "G_TYPE_DOUBLE",
    "G_TYPE_STRING",
    "G_TYPE_POINTER",
    "G_TYPE_BOXED",
    "G_TYPE_PARAM",
    "G_TYPE_OBJECT",
    "G_TYPE_GTYPE",
    "G_TYPE_VARIANT",
    "G_TYPE_CHECKSUM"
  };
  int i;
  char *s;

  for (i=0; i<MAXTYPE; ++i) {
    s = (char *) g_type_name (types[i]);
    g_print ("The name of %s is %s.\n", type_s[i], s); 
  }
  return 0;
}

