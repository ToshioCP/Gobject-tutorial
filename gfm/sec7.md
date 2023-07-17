Up: [Readme.md](../Readme.md),  Prev: [Section 6](sec6.md), Next: [Section 8](sec8.md)

# Derivable and non-abstract type

It is more common to make a non-abstract derivable type than abstract type.
This section covers how to make non-abstract derivable type objects.
A derivable type example is an object for string.
It is TStr.
And its child is an object for numeric string.
A numeric string is a string that expresses a number.
For example, "0", "-100" and "123.45".
The child object (numeric string) will be explained in the next section.

I'd like to write about strings before derivable objects.
Because TStr is an object of a string and we need to be careful to write a program with strings.

## String and memory management

TStr has a string type value.
It is similar to TInt or TDouble but string is more complex than int and double.
When you make TStr program, you need to be careful about memory management, which is not necessary to TInt and TDouble.

### String and memory

String is an array of characters that is terminated with '\0'.
String is not a C type such as char, int, float or double.
But the pointer to a character array behaves like a string type of other languages.
So, we often call the pointer string.

If the pointer is NULL, it points nothing.
So, the pointer is not a string.
Programs with string will include bugs if you aren't careful about NULL pointer.

Another annoying problem is memory allocation.
Because string is an array of characters, memory allocation is necessary to create a new string.
We don't forget to allocate memory, but often forget to free the memory.
It causes memory leak.

~~~C
char *s;
s = g_strdup ("Hello.");
... ... ... do something with s
g_free (s);
~~~

`g_strdup` duplicates a string.
It does:

- Calculates the size of the string.
The size of "Hello." is 7 because strings are zero-terminated.
The string is an array 'H', 'e', 'l', 'l', 'o', '.' and zero ('\0').
- Requests the system to allocate 7 bytes memories.
- Copies the string to the memory.
- Returns the pointer to the newly-allocated memory.
- If the argument is NULL, then g\_strdup returns NULL.

If the string `s` is no longer in use, `s` must be freed, which means the allocated 7 bytes must be returned to the system.
`g_free` frees the memory.

Strings bounded by double quotes like "Hello." are string literals.
They are an array of characters, but the contents of the array are not allowed to change.
And they mustn't be freed.
If you write a character in a string literal or free a string literal, the result is undefined.
Maybe bad things will happen, for example, a segmentation fault error.

There's a difference between arrays and pointers when you initialize them with a string literal.
If an array is initialized with a string literal, the array can be changed.

~~~C
char a[]="Hello!";
a[1]='a';
g_print ("%s\n", a); /* Hallo will appear in your display. */
~~~

The first line initializes an array `a`.
The initialization is not simple.
First, the compiler calculates the length of "Hello!".
It is 7 because the string literal has '\0' at the end of it.
Then 7 bytes memory is allocated in static memory or stack memory.
It depends on the class of the array, whether `static` or `auto`.
The memory is initialized with "Hello!".
So, the string in the array can be changed.
This program successfully displays `Hello!`.

The first line of the program above is the same as follows.

~~~C
char a[] = {'H', 'e', 'l', 'l', 'o', '!', '\0'};
~~~

If you define a pointer with string literal, you can't change the string pointed by the pointer.

~~~C
char *a = "Hello";
*(a+1) = 'a'; /* This is illegal. */
g_print ("%s\n", a);
~~~

The first line just assigns the address of the string literal to the variable `a`.
String literal is an array of characters but it's read-only.
It might be in the program code area or some other non-writable area.
It depends on the implementation of your compiler.
Therefore, the second line tries to write a char 'a' to the read-only memory and the result is undefined, for example, a segmentation error happens.
Anyway, don't write a program like this.

In conclusion, a string is an array of characters and it is placed in one of the following.

- Read-only memory.
A string literal is read-only.
- stack.
If the class of an array is `auto`, then the array is placed in the stack.
stack is writable.
If the array is defined in a function, its default class is `auto`.
The stack area will disappear when the function returns to the caller.
- static area.
If the class of an array is `static`, then the array is placed in the static area.
It keeps its value and remains for the life of the program.
This area is writable.
- heap.
You can allocate and free memory for a string.
For allocation, `g_new` or `g_new0` is used.
For freeing, `g_free` is used.

### Copying string

There are two ways to copy a string.
First way is just copying the pointer.

~~~C
char *s = "Hello";
char *t = s;
~~~

Two pointers `s` and `t` points the same address.
Therefore, you can't modify `t` because `t` points a string literal, which is read-only.

Second way is creating memory and copying the string to the memory.

~~~C
char *s = "Hello";
char *t = g_strdup (s);
~~~

`g_strdup` allocates memory and initializes it with "Hello", then returns the pointer to the memory.
The function `g_strdup` is almost same as the `string_dup` function below.

~~~C
#include <glib-object.h>
#include <string.h>

char *
string_dup (char *s) {
  int length;
  char *t;

  if (s == NULL)
    return NULL;
  length = strlen (s) + 1;
  t = g_new (char, length);
  strcpy (t, s);
  return t;
}
~~~

If `g_strdup` is used, the two pointers `s` and `t` point different memories.
You can modify `t` because it is placed in the memory allocated from the heap area.

It is important to know the difference between assigning pointers and duplicating strings.

### const qualifier

The qualifier `const` makes a variable won't change its value.
It can also be applied to an array.
Then, the elements of the array won't be changed.

~~~C
const double pi = 3.1415926536;
const char a[] = "read only string";
~~~

An array parameter in a function can be qualified with `const` to indicate that the function does not change the array.
In the same way, the return value (a pointer to an array or string) of a function can be qualified with `const`.
The caller mustn't modify or free the returned array or string.

~~~C
char *
string_dup (const char *s) {
  ... ...
}

const char *
g_value_get_string (const GValue *value);
~~~

The qualifier `const` indicates who is the owner of the string when it is used in the function of objects.
"Owner" is an object or a caller of the function that has the right to modify or free the string.

For example, `g_value_get_string` is given `const GValue *value` as an argument.
The GValue pointed by `value` is owned by the caller and the function doesn't change or free it.
The function returns a string qualified with `const`.
The returned string is owned by the object and the caller mustn't change or free the string.
It is possible that the string will be changed or freed by the object later.

## Header file

The rest of this section is about TStr.
TStr is a child of GObject and it holds a string.
The string is a pointer and an array of characters.
The pointer points the array.
The pointer can be NULL.
If it is NULL, TStr has no array.
TStr has a string type property.

The header file `tstr.h` is as follows.

~~~C
 1 #ifndef __T_STR_H__
 2 #define __T_STR_H__
 3 
 4 #include <glib-object.h>
 5 
 6 #define T_TYPE_STR  (t_str_get_type ())
 7 G_DECLARE_DERIVABLE_TYPE (TStr, t_str, T, STR, GObject)
 8 
 9 struct _TStrClass {
10   GObjectClass parent_class;
11   /* expect that descendants will override the setter */
12   void (*set_string)  (TStr *self, const char *s);
13 };
14 
15 TStr *
16 t_str_concat (TStr *self, TStr *other);
17 
18 /* setter and getter */
19 void
20 t_str_set_string (TStr *self, const char *s);
21 
22 char *
23 t_str_get_string (TStr *self);
24 
25 /* create a new TStr instance */
26 TStr *
27 t_str_new_with_string (const char *s);
28 
29 TStr *
30 t_str_new (void);
31 #endif /* __T_STR_H__ */
32 
~~~

- 7: Uses `G_DECLARE_DERIVABLE_TYPE` like the example in the previous section.
- 9-13: TStrClass has one class method.
It is `set_string`.
This will be overridden by the child class `TNumStr`.
The class method is called by the public function `t_str_set_string`.
It has a string parameter and sets the instance string with the string (argument).
TNumStr class holds a string like TStr, but it holds the type of the string as well.
The class method `set_string` will be overridden and it will set not only the string but also the type in the child class.
The detailed explanation will be in the later part of this section and the next section. 
- 15-16: `t_str_concat` connects two strings of TStr instances and returns a new TStr instance.
- 19-23: Setter and getter.
- 26-30: Functions to create a TStr object.

## C file

The C file of TStr object is as follows.
It is `tstr.c`.

~~~C
  1 #include "tstr.h"
  2 
  3 enum {
  4   PROP_0,
  5   PROP_STRING,
  6   N_PROPERTIES
  7 };
  8 
  9 static GParamSpec *str_properties[N_PROPERTIES] = {NULL, };
 10 
 11 typedef struct {
 12   char *string;
 13 } TStrPrivate;
 14 
 15 G_DEFINE_TYPE_WITH_PRIVATE(TStr, t_str, G_TYPE_OBJECT)
 16 
 17 static void
 18 t_str_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 19   TStr *self = T_STR (object);
 20 
 21   if (property_id == PROP_STRING)
 22     t_str_set_string (self, g_value_get_string (value));
 23   else
 24     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 25 }
 26 
 27 static void
 28 t_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 29   TStr *self = T_STR (object);
 30 
 31   if (property_id == PROP_STRING)
 32     g_value_set_string (value, t_str_get_string(self));
 33   else
 34     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 35 }
 36 
 37 static void
 38 t_str_real_set_string (TStr *self, const char *s) {
 39   TStrPrivate *priv = t_str_get_instance_private (self);
 40 
 41   if (priv->string)
 42     g_free (priv->string);
 43   priv->string = g_strdup (s);
 44 }
 45 
 46 static void
 47 t_str_finalize (GObject *object) {
 48   TStr *self = T_STR (object);
 49   TStrPrivate *priv = t_str_get_instance_private (self);
 50 
 51   if (priv->string)
 52     g_free (priv->string);
 53   G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
 54 }
 55 
 56 static void
 57 t_str_init (TStr *self) {
 58   TStrPrivate *priv = t_str_get_instance_private (self);
 59 
 60   priv->string = NULL;
 61 }
 62 
 63 static void
 64 t_str_class_init (TStrClass *class) {
 65   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
 66 
 67   gobject_class->finalize = t_str_finalize;
 68   gobject_class->set_property = t_str_set_property;
 69   gobject_class->get_property = t_str_get_property;
 70   str_properties[PROP_STRING] = g_param_spec_string ("string", "str", "string", "", G_PARAM_READWRITE);
 71   g_object_class_install_properties (gobject_class, N_PROPERTIES, str_properties);
 72 
 73   class->set_string = t_str_real_set_string;
 74 }
 75 
 76 /* setter and getter */
 77 void
 78 t_str_set_string (TStr *self, const char *s) {
 79   g_return_if_fail (T_IS_STR (self));
 80   TStrClass *class = T_STR_GET_CLASS (self);
 81 
 82   class->set_string (self, s);
 83 }
 84 
 85 char *
 86 t_str_get_string (TStr *self) {
 87   g_return_val_if_fail (T_IS_STR (self), NULL);
 88   TStrPrivate *priv = t_str_get_instance_private (self);
 89 
 90   return g_strdup (priv->string);
 91 }
 92 
 93 TStr *
 94 t_str_concat (TStr *self, TStr *other) {
 95   g_return_val_if_fail (T_IS_STR (self), NULL);
 96   g_return_val_if_fail (T_IS_STR (other), NULL);
 97 
 98   char *s1, *s2, *s3;
 99   TStr *str;
100 
101   s1 = t_str_get_string (self);
102   s2 = t_str_get_string (other);
103   if (s1 && s2)
104     s3 = g_strconcat (s1, s2, NULL);
105   else if (s1)
106     s3 = g_strdup (s1);
107   else if (s2)
108     s3 = g_strdup (s2);
109   else
110     s3 = NULL;
111   str = t_str_new_with_string (s3);
112   if (s1) g_free (s1);
113   if (s2) g_free (s2);
114   if (s3) g_free (s3);
115   return str;
116 }
117 
118 /* create a new TStr instance */
119 TStr *
120 t_str_new_with_string (const char *s) {
121   return T_STR (g_object_new (T_TYPE_STR, "string", s, NULL));
122 }
123 
124 TStr *
125 t_str_new (void) {
126   return T_STR (g_object_new (T_TYPE_STR, NULL));
127 }
128 
~~~

- 3-9: `enum` defines a property id.
`PROP_STRING` is the id of "string" property.
Only one property is defined here, so you can define it without `enum`.
However, `enum` can be applied to define two or more properties.
So, this way is more recommended.
In the same way, an array `str_properties` is used.
it stores a static pointer for GParamSpec.
- 11-13: TStrPrivate is a C structure.
It is a private data area for TStr.
If TStr were a final type, then no descendant would exist and TStr instance could be a private data area.
But TStr is derivable so you can't store such private data in TStr instance that is open to the descendants.
The name of this structure is "<object name\>Private" like `TStrPrivate`.
The structure must be defined before `G_DEFINE_TYPE_WITH_PRIVATE`.
- 15: `G_DEFINE_TYPE_WITH_PRIVATE` macro.
This macro expands to:
  - Declares `t_str_class_init` which is a class initialization function.
  - Declares `t_str_init` which is an instance initialization function.
  - Defines `t_str_parent_class` static variable.
It points to the parent class of TStr.
  - Adds private instance data to the type.
It is a C structure and its name is `TStrPrivate`. (See above).
  - Defines `t_str_get_type ()` function.
This function registers the type in its first call.
  - Defines a private instance getter `t_str_get_instance_private ()`.
This function has a parameter which is the pointer to the instance.
- 17-25: `t_str_set_property`.
This is similar to `t_int_set_property`, but the property value is stored in the private area.
- 22: It uses `t_str_set_string` function to set the private data area with the string from the GValue.
It is important because `t_str_set_string` calls the class method `set_string`, which will be overridden by the child class.
Therefore, the `t_str_set_property` function will set the string and also its type in the child class.
The function `g_value_get_string` returns the pointer to the string that GValue owns.
You can't own the string.
That means you can't change or free the string.
Therefore, it is necessary to duplicate the string before it is stored.
The duplication is done in the function `t_str_set_string`.
- 27-35: `t_str_get_property`.
It uses `t_str_get_string` function.
String duplication is done in the function.
- 37-44 `t_str_real_set_string` function.
This is pointed by `set_string` in the class.
So, it is the body of the class method.
First, it gets the pointer to the private area with `t_str_get_instance_private` function.
If the instance holds a string, free it before setting it with a new string.
It copies the string and put it to `priv->string`.
- 46-54: `t_str_finalize` is called when TStr instance is destroyed.
This function frees the string `priv->string`.
After that, it calls the parent's finalize method.
This is called "chain up to its parent" and it will be explained later in this section.
- 56-61: `t_str_init` initializes `priv->string`.
- 63-74: `t_str_class_init` function initializes the class of TStr object.
- 67: Overrides `finalize` method.
This method is called in the destruction process.
- 68-69: Overrides `set_property` and `get_property` method.
- 70-71: Creates GParamSpec.
Then installs the property.
- 73: The class method `set_string` points to `t_str_real_set_string`.
This method is expected  to be replaced in the child class.
- 77-91: Setter and getter.
They are used by property set/get functions `t_str_set_property` and `t_str_get_property`.
The setter `t_str_set_string` just calls the class method.
So its behavior will change in the child class.
- 93-116: `t_str_concat` function.
It concatenates the string of `self` and `other` and creates a new TStr.
- 119-127: `t_str_new_with_string` and `t_str_new` create a new TStr instances.

## Chaining up to its parent

The "chain up to its parent" process is illustrated with the diagram below.

![Chaining up process in GObject and TStr](../image/chainup.png)

There are two classes, GObjectClass and TStrClass.
Each class has their finalize methods (functions) pointed by the pointers in the class structures.
The finalize method of TStrClass finalizes its own part of the TStr instance.
At the end of the function, it calls its parent's finalize method.
It is the finalize method of GObjectClass.
It calls its own finalize function and finalizes the GObject private data.

If the GObjectClass has two or more descendant classes, the number of the finalize functions may be the same as the number of the descendants.
And they are connected by "chain up to its parent" way.  

![Chaining up process](../image/chainup3.png)

## How to write a derivable type

- Use `G_DECLARE_DERIVABLE_TYPE` macro in the header file.
You need to write a macro like `#define T_TYPE_STR (t_str_get_type ())` before `G_DECLARE_DERIVABLE_TYPE`.
- Declare your class structure in the header file.
The contents of the class are open to the descendants.
Most of the members are class methods.
- Use `G_DEFINE_TYPE_WITH_PRIVATE` in the C file.
You need to define the private area before `G_DEFINE_TYPE_WITH_PRIVATE`.
It is a C structure and the name is "<object name\>Private" like "TStrPrivate".
- Define class and instance initialization functions.


Up: [Readme.md](../Readme.md),  Prev: [Section 6](sec6.md), Next: [Section 8](sec8.md)
