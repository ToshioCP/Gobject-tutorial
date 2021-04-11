Up: [Readme.md](../Readme.md),  Prev: [Section 7](sec7.md), Next: [Section 9](sec9.md)

# String and memory management

The example in this section is TStr object.
TStr has a string type value and is a child object of TPtr.

It is similar to TInt or TDouble but C language String is more complex than int and double.
When you make TStr program, you need to be careful about memory management, which is not necessary in TInt and TDouble.
First two subsections are topics about string and memory.

## String and memory

String is an array of characters that is terminated with '\0'.
String is not a C type such as char, int, float or double.
But the pointer to an array behaves like a string type of other languages.
So, we often call pointers string when it points a character array.

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
The size of "Hello." is 6.
- Requests the system to allocate 6 bytes memory.
- Copies the string to the memory.
- Returns the pointer to the newly-allocated memory.

If the string `s` is no longer useless, `s` must be freed, which means the allocated 6 bytes are returned to the system.
`g_free` frees the memory.

Strings bounded by double quotes like "Hello." is string literal.
It is an array of characters, but the contents of the array is not allowed to change usually.
And it mustn't be freed.

I wrote "usually".
There's an exception.
If an array is initialized with string literal, the array can be changed.

~~~C
char a[]="Hello!";
a[1]='a';
g_print ("%s\n", a);
~~~

First, the compiler calculates the length of "Hello!".
It is 6.
Then 6 bytes memory is allocated in static memory (static class array) or stack memory (auto class array).
The memory is initialized with "Hello!".
So, the string in the array can me changed.
This program displays `Hallo!`, not `Hello!`.

The following program is wrong and if you execute it, some bad things will probably happen.

~~~C
char *a = "Hello";
*(a+1) = `a`;
g_print ("%s\n", a);
~~~

In this program, "Hello" is not a static class array or an auto class array.
It might be in the program code area or some other non-writable area.
It depends on the implementation of your compiler.
Anyway, don't program like this.

## Copying string

There are two ways to copy a string.
First, copy the pointer.

~~~C
char *s = "Hello";
char *t = s;
~~~

Two pointers `s` and `t` points the same address.
This program is OK because string literal above never changes and never be freed.
But if the string is mutable or can be freed, a problem might happen.

~~~C
 1 #include <glib-object.h>
 2 
 3 void
 4 func (char *string) {
 5   char *s = string+7;
 6   char *t = "Bar";
 7 
 8   while (*t)
 9     *s++ = *t++;
10 }
11 
12 int
13 main (int argc, char **argv) {
14   char *s = g_strdup ("Hello, Foo!");
15 
16   func (s);
17   g_print ("%s\n", s); /* Hello, Foo! is expected, but ... */
18   g_free (s);
19 }
20 
~~~

When the function `func` is called, the copy of the pointer `s` is made and the copy is assigned to the parameter `string`.
Therefore, `string` is a copy of `s` and both pointer point the same string "Hello Foo!".
The function replaces `Foo` with `Bar` in the string pointed by `string`.
The modified string is also pointed by `s`.
Therefore, the output is "Hello Bar!", not "Hello Foo!".

If the string `s` is copied before the function call, "Hello Foo!" will be shown.

~~~C
 1 #include <glib-object.h>
 2 
 3 void
 4 func (char *string) {
 5   char *s = string+7;
 6   char *t = "Bar";
 7 
 8   while (*t)
 9     *s++ = *t++;
10 }
11 
12 int
13 main (int argc, char **argv) {
14   char *s = g_strdup ("Hello, Foo!");
15   char *t = g_strdup (s);
16 
17   func (t);
18   g_print ("%s\n", s); /* Hello, Foo! is shown */
19   g_free (s);
20   g_free (t);
21 }
22 
~~~

The difference between copying a pointer and duplicating a string (an array of characters) is the same as shallow copy and deep copy in object oriented languages.
It's useful to learn shallow copy and deep copy.

The problem above often happens in function call.
Better designed function uses const qualifier.
It ensures that the string isn't changed in the function.

~~~C
 1 #include <glib-object.h>
 2 
 3 char *
 4 func (const char *string) {
 5   char *str = g_strdup (string);
 6   char *s = str;
 7   char *t = "Bar";
 8 
 9   for (s += 7; *t; *s++ = *t++)
10     ;
11   return str;
12 }
13 
14 int
15 main (int argc, char **argv) {
16   char *s = g_strdup ("Hello, Foo!");
17   char *t;
18 
19   t = func (s);
20   g_print ("%s\n", s); /* Hello, Foo! */
21   g_print ("%s\n", t); /* Hello, Bar! */
22   g_free (s);
23   g_free (t);
24 }
25 
~~~

## TStr object

TStr is a child of TPtr.
It holds a string value.
The pointer of the string is a "pointer" property of TPtr, which is a parent of TStr.
It has  a function `t_str_concat` which concatenates two strings of TStr objects and creates a new TStr object.

The header file `tstr.h` is as follows.

~~~C
 1 #ifndef __T_STR_H__
 2 #define __T_STR_H__
 3 
 4 #include <glib-object.h>
 5 #include "../tptr/tptr.h"
 6 
 7 #define T_TYPE_STR  (t_str_get_type ())
 8 G_DECLARE_FINAL_TYPE (TStr, t_str, T, STR, TPtr)
 9 
10 TStr *
11 t_str_concat (TStr *self, TStr *other);
12 
13 /* setter and getter */
14 void
15 t_str_set_string (TStr *self, const char *s);
16 
17 char *
18 t_str_get_string (TStr *self);
19 
20 /* create a new TStr instance */
21 TStr *
22 t_str_new_with_string (const char *s);
23 
24 TStr *
25 t_str_new (void);
26 #endif /* __T_STR_H__ */
27 
~~~

- 8: TStr is a final type.
`G_DECLARE_FINAL_TYPE` is used.
- 10-11: `t_str_concat` concatenates two strings of `self` and `other`, then returns the newly created TStr instance.
- 14-18: Setter and getter.
- 21-22: `t_str_new_with_string (const char *s)` creates a new TStr object and set its value with `s`.
Because the parameter has const qualifier, it ensures that the string `s` never changes in the function.
- 24-25: `t_str_new` creates TStr that holds NULL pointer.

`tstr.c` is a C file of TStr object.

~~~C
 1 #include "tstr.h"
 2 
 3 struct _TStr {
 4   TPtr parent;
 5 };
 6 
 7 G_DEFINE_TYPE (TStr, t_str, T_TYPE_PTR)
 8 
 9 static void
10 t_str_init (TStr *inst) {
11 }
12 
13 static void
14 t_str_finalize (GObject *object) {
15   char *s = (char *) t_ptr_get_pointer (T_PTR (object));
16 
17   if (s)
18     g_free (s);
19   G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
20 }
21 
22 static void
23 t_str_class_init (TStrClass *class) {
24   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
25 
26   gobject_class->finalize = t_str_finalize;
27 }
28 
29 /* setter and getter */
30 void
31 t_str_set_string (TStr *self, const char *s) {
32   char *t = g_strdup (s);
33 
34   t_ptr_set_pointer (T_PTR (self), t);
35 }
36 
37 char *
38 t_str_get_string (TStr *self) {
39   char *s;
40 
41   s = (char *) t_ptr_get_pointer (T_PTR (self));
42   if (s)
43     return g_strdup (s);
44   else
45     return s; /* NULL */
46 }
47 
48 TStr *
49 t_str_concat (TStr *self, TStr *other) {
50   char *s1, *s2, *s3;
51   TStr *str;
52 
53   s1 = t_str_get_string (self);
54   s2 = t_str_get_string (other);
55   if (s1 && s2)
56     s3 = g_strconcat (s1, s2, NULL);
57   else if (s1)
58     s3 = g_strdup (s1);
59   else if (s2)
60     s3 = g_strdup (s2);
61   else
62     s3 = NULL;
63   str = t_str_new_with_string (s3);
64   if (s1) g_free (s1);
65   if (s2) g_free (s2);
66   if (s3) g_free (s3);
67   return str;
68 }
69 
70 TStr *
71 t_str_new_with_string (const char *s) {
72   TStr *str;
73 
74   str = g_object_new (T_TYPE_STR, NULL);
75   t_str_set_string (str,s);
76   return str;
77 }
78 
79 TStr *
80 t_str_new (void) {
81   TStr *str;
82 
83   str = g_object_new (T_TYPE_STR, NULL);
84   return str;
85 }
86 
~~~

- 3-5: TStr instance structure.
It only has its parent instance.
No its own area exists.
- 7: `G_DEFINE_TYPE` is used.
- 9-11: `t_str_init` does nothing.
The parent instance of TPtr initializes its pointer to NULL.
Therefore, the pointer of TStr instance is also NULL just after it is created.
- 13-20: `t_str_finalize` finalizes the instance.
This function is assigned to `class->finalize` in class initialization.
`t_str_finalize` frees the string pointed by the pointer of TPtr if it is not NULL.
After that it calls its parent's finalization method.
This is called "chain up to its parent".
This process is complicated.
It will explained later.
- 22-27: `t_str_class_init` initializes TStrClass.
`t_str_finalize` overrides `gobject_class->finalize`.
- 30-35: Setter.
`s` is owned by the caller.
`self` needs to own its string so it duplicates `s`.
After the duplication, `s` and `t` points different address.
So, the string pointed by `s` and another string pointed by `t` are different but the contents of both the string are the same.
Set the pointer of `self` with `t`.
- 37-46: Getter.
Gets the pointer from `self` and assigns it to `s`.
If `s` is string (not NULL), then duplicates it and returns it.
Otherwise, returns NULL.
- 48-68: `t_str_concat`.
It concatenates two strings held by `self` and `other`.
Because `t_str_get_string` allocates memory for strings `s1` and `s2`, they needs to be freed when they become useless.
- 55-62: Creates `s3`.
`g_str_concat` concatenates two strings and creates a new string.
- 63: Creates a new Tstr object with `s3`.
- 64-66: `s1`, `s2` and `s3` are now useless, so they are freed if they aren't NULL.
- 70-85: Creates new TStr instance.
Const qualifier is used in line 71.
This ensures the parameter `s` never changes in the function.

The "chain up to its parent" process is illustrated with the diagram below.
There are three classes, GObjectCLass, TPtrClass and TStrClass.
Each class has finalize method pointer which points finalize functions in gobject program or tstr program.
The finalize method of TStrClass finalize its own part of the TStr instance.
At the end of the function, it calls its parent's finalize method.
It is the finalize method of TPtrClass.
The method hasn't been overridden, so it points the finalize method in gobject program.
It finalizes the GObject part of the TStr instance.

![Finalize process](../image/finalize.png)

`main.c` tests TStr object.

~~~C
 1 #include <glib-object.h>
 2 #include "tstr.h"
 3 
 4 
 5 static void
 6 notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
 7   const char *name;
 8   char *s;
 9 
10   name = g_param_spec_get_name (pspec);
11   if (T_IS_STR (gobject) && strcmp (name, "pointer") == 0) {
12     g_object_get (T_PTR (gobject), "pointer", &s, NULL);
13     g_print ("Property \"%s\" is set to %s.\n", name, s);
14   }
15 }
16 
17 int
18 main (int argc, char **argv) {
19   const char *one = "one";
20   const char *two = "two";
21   TStr *str1, *str2, *str3;
22   char *s1, *s2, *s3;
23 
24   str1 = t_str_new ();
25   str2 = t_str_new ();
26 
27   g_signal_connect (G_OBJECT (str1), "notify::pointer", G_CALLBACK (notify_cb), NULL);
28   g_signal_connect (G_OBJECT (str2), "notify::pointer", G_CALLBACK (notify_cb), NULL);
29 
30   t_str_set_string (str1, one);
31   t_str_set_string (str2, two);
32 
33   str3 = t_str_concat (str1, str2);
34    s1 = t_str_get_string (str1);
35    s2 = t_str_get_string (str2);
36    s3 = t_str_get_string (str3);
37   if (s1)
38     g_print ("str1 is \"%s\".\n", s1);
39   else
40     g_print ("str1 is NULL.\n");
41   if (s2)
42     g_print ("str2 is \"%s\".\n", s2);
43   else
44     g_print ("str2 is NULL.\n");
45   if (s3)
46     g_print ("str3 is \"%s\".\n", s3);
47   else
48     g_print ("str3 is NULL.\n");
49 
50   g_object_unref (str1);
51   g_object_unref (str2);
52   g_object_unref (str3);
53   g_free (s1);
54   g_free (s2);
55   g_free (s3);
56 
57   return 0;
58 }
59 
~~~

- 5-15: Notify signal handler is almost same as before.
- 17-58: `main` function.
This function creates two TStr objects and concatenates them.
- 24-31: Creates `str1` and `str2`.
Connects "notify::pointer" signal to `notify_cb`.
Sets string of the instances with `one` and `two` respectively.
At the same time, "pointer" properties are set.
- 33: Concatenates `str1` and `str2`, then creates new object `str3`.
- 34-36: Gets strings from the TStr instances.
- 38-48: shows the strings.
- 50-55: Frees instances and strings.

## Compilation and execution

Compilation is done by usual way.
First, change your current directory to `src/tstr`.

~~~
$ meson _build
$ ninja -C _build
~~~

Then, execute it.

~~~
$ cd tstr; _build/tstr
Property "pointer" is set to one.
Property "pointer" is set to two.
str1 is "one".
str2 is "two".
str3 is "onetwo".
~~~

The last line shows that "one" and "two" are concatenated.

You have probably been aware that strings make things complicated because of memory management.
However, strings are one of the most useful data structure in computers.
If you make child objects of GObject, the technique to manage strings will be necessary.
The important things are:

- Memory allocation and freeing.
Don't forget freeing.
- Think about what object or function owns a string and are responsible to free the string.
- Understand how to initialize and finalize an instance.
- Use const qualifier in your function if string parameters doesn't change in your function.
This is useful for users to know whether they should duplicate their strings.


Up: [Readme.md](../Readme.md),  Prev: [Section 7](sec7.md), Next: [Section 9](sec9.md)
