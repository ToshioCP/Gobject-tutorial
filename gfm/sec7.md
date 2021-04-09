Up: [Readme.md](../Readme.md),  Prev: [Section 6](sec6.md), Next: [Section 8](sec8.md)

# Derivable and non-abstract type

It is more common to make a non-abstract derivable type than abstract type.
This section covers how to make non-abstract derivable type objects.
An example in this section is an object for general pointer.
Children of the object can be string, array and so on, which are objects that use pointer.
The object has a property of a pointer.

## Header file

The name of the object is TPtr.
Its header file is as follows.

~~~C
 1 #ifndef __T_PTR_H__
 2 #define __T_PTR_H__
 3 
 4 #include <glib-object.h>
 5 
 6 #define T_TYPE_PTR             (t_ptr_get_type ())
 7 G_DECLARE_DERIVABLE_TYPE (TPtr, t_ptr, T, PTR, GObject)
 8 
 9 struct _TPtrClass {
10   GObjectClass parent_class;
11 };
12 
13 /* setter and getter */
14 void
15 t_ptr_set_pointer (TPtr *self, gpointer p);
16 
17 gpointer
18 t_ptr_get_pointer (TPtr *self);
19 
20 /* create a new TPtr instance */
21 TPtr *
22 t_ptr_new_with_pointer (gpointer pointer);
23 
24 TPtr *
25 t_ptr_new (void);
26 
27 #endif /* __T_PTR_H__ */
28 
~~~

- 7: Uses `G_DECLARE_DERIVABLE_TYPE` like the example in the previous section.
- 9-11: TPtrClass doesn't have its own class area.
- 13-18: Setter and getter.
- 21-25: Functions to create a TPtr object.

## C file

The C file of TPtr object is as follows.

~~~C
 1 #include "tptr.h"
 2 
 3 #define PROP_PTR 1
 4 static GParamSpec *ptr_property = NULL;
 5 
 6 typedef struct {
 7   gpointer pointer;
 8 } TPtrPrivate;
 9 
10 G_DEFINE_TYPE_WITH_PRIVATE(TPtr, t_ptr, G_TYPE_OBJECT)
11 
12 static void
13 t_ptr_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
14   TPtr *self = T_PTR (object);
15   TPtrPrivate *priv = t_ptr_get_instance_private (self);
16 
17   if (property_id == PROP_PTR) {
18     priv->pointer = g_value_get_pointer (value);
19   } else
20     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
21 }
22 
23 static void
24 t_ptr_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
25   TPtr *self = T_PTR (object);
26   TPtrPrivate *priv = t_ptr_get_instance_private (self);
27 
28   if (property_id == PROP_PTR)
29     g_value_set_pointer (value, priv->pointer);
30   else
31     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
32 }
33 
34 static void
35 t_ptr_class_init (TPtrClass *class) {
36   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
37 
38   gobject_class->set_property = t_ptr_set_property;
39   gobject_class->get_property = t_ptr_get_property;
40   ptr_property = g_param_spec_pointer ("pointer", "ptr", "pointer to anything", G_PARAM_READWRITE);
41   g_object_class_install_property (gobject_class, PROP_PTR, ptr_property);
42 }
43 
44 static void
45 t_ptr_init (TPtr *inst) {
46   TPtrPrivate *priv = t_ptr_get_instance_private (inst);
47   priv->pointer = NULL;
48 }
49 
50 /* setter and getter */
51 void
52 t_ptr_set_pointer (TPtr *self, const gpointer p) {
53   g_object_set (self, "pointer", p, NULL);
54 }
55 
56 /* t_ptr_get_pointer doesn't use g_object_get */
57 /* Because this function is possibly used in finalize method of its child. */
58 /* g_object_get cannot be used in the finalization process. */
59 gpointer
60 t_ptr_get_pointer (TPtr *self) {
61   TPtrPrivate *priv = t_ptr_get_instance_private (self);
62 
63   return priv->pointer;
64 }
65 
66 TPtr *
67 t_ptr_new_with_pointer (gpointer p) {
68   TPtr *ptr;
69 
70   ptr = g_object_new (T_TYPE_PTR, "pointer", p, NULL);
71   return ptr;
72 }
73 
74 TPtr *
75 t_ptr_new (void) {
76   TPtr *p;
77 
78   p = g_object_new (T_TYPE_PTR, NULL);
79   return p;
80 }
81 
~~~

- 3-4: Property id and a static pointer for GParamSpec.
- 6-8: TPtrPrivate is a C structure.
It is a private data area for TPtr.
If TPtr were a final type, then no descendant would exist and TPtr instance could be a private data area.
But TPtr is derivable so you can't store such private data in TPtr instance that is open to the descendants.
The name of this structure is "<object name\>Private" like `TPtrPrivate`.
The structure must be defined before `G_DEFINE_TYPE_WITH_PRIVATE`.
- 10: `G_DEFINE_TYPE_WITH_PRIVATE` macro.
This macro expands to:
  - Declares `t_ptr_class_init` which is a class initialization function.
  - Declares `t_ptr_init` which is an instance initialization function.
  - Defines `t_ptr_parent_class` static variable.
It points to the parent class of TPtr.
  - Adds private instance data to the type.
It is a C structure and its name is `TPtrPrivate`. (See above).
  - Defines `t_ptf_get_type ()` function.
This function registers the type in its first call.
  - Defines a private instance getter `t_ptr_get_instance_private ()`.
This function has a parameter which is the pointer to the instance.
- 12-21: `t_ptr_set_property`.
This is similar to `t_int_set_property`, but the property value is stored in the private area.
- 15: Gets the pointer to `TPtrPrivate`.
- 18: Stores the pointer in `priv->pointer`.
- 23-32: `t_ptr_get_property`.
Gets the pointer to `TPtrPrivate` and retrieves the property value from `Priv->pointer`.
- 34-42: `t_ptr_class_init`
Initializes property related methods and GParamSpec.
Then installs the property.
- 44-48: `t_ptr_init`
It assigns NULL to `priv->pointer`.
- 50-64: Setter and getter functions.
They are convenient to set or get a pointer which is also a property.
`g_object_set` and `g_object_get` are fine.
But we have two reasons to have setter and getter.
  - Setter and getter is convenient.
  - It's impossible to use `g_object_get` in finalization process.
Children of this object need to free memory owned by them in their finalization process.
They use `t_ptr_get_pointer` to get the pointer to free it.
`t_ptr_get_pointer` doesn't use `g_object_get`.
- 51-54: `t_ptr_set_pointer` is a setter of "pointer" property.
It uses `g_object_set` so that "notify" signal is emitted.
- 59-64: `t_ptr_get_pointer` is a getter of "pointer" property.
It doesn't use `g_object_get`.
- 66-80: `t_ptr_new_with_pointer` and `t_ptr_new` create a new TPtr instance.

## main.c

`main.c` is a simple test program for TPtr object.

~~~C
 1 #include <glib-object.h>
 2 #include "tptr.h"
 3 
 4 static void
 5 notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
 6   const char *name;
 7   gpointer p;
 8 
 9   name = g_param_spec_get_name (pspec);
10   if (T_IS_PTR (gobject) && strcmp (name, "pointer") == 0) {
11     g_object_get (T_PTR (gobject), "pointer", &p, NULL);
12     g_print ("Property \"%s\" is set to %p.\n", name, p);
13   }
14 }
15 
16 int
17 main (int argc, char **argv) {
18   TPtr *ptr;
19 
20   ptr = t_ptr_new ();
21 
22   g_signal_connect (G_OBJECT (ptr), "notify::pointer", G_CALLBACK (notify_cb), NULL);
23 
24   t_ptr_set_pointer (ptr, NULL);
25   t_ptr_set_pointer (ptr, ptr);
26   g_print ("Get a pointer %p.\n", t_ptr_get_pointer (ptr));
27 
28   g_object_unref (ptr);
29 
30   return 0;
31 }
32 
~~~

- 4-14: "notify" signal handler.
It just shows that the value of the property is set.
- 16-31: `main` function.
- 24-25: Uses the setter function to check if the "notify::pointer" signal is emitted.
- 26: Checks the getter function.

## Compilation and execution

Change your current directory to `src/tptr` directory and type the following as usual.

~~~
$ meson _build
$ ninja -C _build
~~~

Execute `tptr`.

~~~
$ cd tptr; _build/tptr
Property "pointer" is set to (nil).
Property "pointer" is set to 0x55fc14ca8810.
Get a pointer 0x55fc14ca8810.
~~~

## How to write a derivable type

- Use `G_DECLARE_DERIVABLE_TYPE` macro in the header file.
You need to write a macro like `#define T_TYPE_PTR (t_ptr_get_type ())` before `G_DECLARE_DERIVABLE_TYPE`.
- Declare your class structure in the header file.
The contents of the class are open to the descendants.
Most of the members are class methods.
- Use `G_DEFINE_TYPE_WITH_PRIVATE` in the C file.
You need to define the private area before `G_DEFINE_TYPE_WITH_PRIVATE`.
It is a C structure and the name is "<object name\>Private" like "TPtfPrivate".
- Define class and instance initialization functions.


Up: [Readme.md](../Readme.md),  Prev: [Section 6](sec6.md), Next: [Section 8](sec8.md)
