Up: [Readme.md](../Readme.md),  Prev: [Section 1](sec1.md), Next: [Section 3](sec3.md)

# GObject

## Class and instance

GObject instance is created with `g_object_new` function.
GObject has not only instance but class.
It is GObjectClass.

- GObjectClass is created at the first call of `g_object_new`.
And only one GObjectClass exists.
- GObject instance is created whenever `g_object_new` is called.
So, two or more GObject can exist.

GObject and GObjectClass are C structures.
They are declared in `gobject.h`, which is one of the glib source files.
You can download the source file from [Gnome download page](https://download.gnome.org/sources/glib/).

~~~C
typedef struct _GObject                  GObject;
typedef struct _GObjectClass             GObjectClass;
~~~

The members of the structures are also declared in `gobject.h`, but it is not important now.

There are sample programs in [src/misc](../src/misc) directory.
You can compile them by:

~~~
$ cd src/misc
$ meson _build
$ ninja -C _build
~~~

One of the programs is `example1.c`.
Its code is as follows.

~~~C
 1 #include <glib-object.h>
 2 
 3 int
 4 main (int argc, char **argv) {
 5   GObject *instance1, *instance2;
 6   GObjectClass *class1, *class2;
 7   
 8   instance1 = g_object_new (G_TYPE_OBJECT, NULL);
 9   instance2 = g_object_new (G_TYPE_OBJECT, NULL);
10   g_print ("The address of instance1 is %p\n", instance1);
11   g_print ("The address of instance2 is %p\n", instance2);
12 
13   class1 = G_OBJECT_GET_CLASS (instance1);
14   class2 = G_OBJECT_GET_CLASS (instance2);
15   g_print ("The address of the class of instance1 is %p\n", class1);
16   g_print ("The address of the class of instance2 is %p\n", class2);
17 
18   g_object_unref (instance1);
19   g_object_unref (instance2);
20 
21   return 0;
22 }
23 
~~~

- 5: `instance1` and `instance2` are pointers that points GObject instance.
`class1` and `class2` points GObjecectClass.
- 8-11: A function `g_object_new` creates a GObject instance.
GObject instance is a chunk of memory which is GObject structure (`struct _GObject`).
The argument `G_TYPE_OBJECT` represents the type of GObject.
This type is different from C language type like `char`  or `int`.
There is *type system* which is a base system of gobject system.
Every data type such as GObject must be registered to the type system.
The type system has series of functions for the registration.
If one of the functions is called, then the type system determines `GType` type value for the object and returns it to the caller.
`GType` is an unsigned long integer on my computer but it depends on the hardware.
`g_object_new` allocates necessary memory to GObject instance and returns the pointer to the top address of the memory.
After the creation, this program displays the addresses of instances.
- 13-16: A macro `G_OBJECT_GET_CLASS` returns the pointer to the class of the argument.
Therefore, `class1` points the class (GObjectClass) of `instance1` and `class2` points the class of `instance2` respectively.
The addresses of the two classes are displayed.
- 18-19: `g_object_unref` will be explained in the next subsection.
It destroys the objects and the memory are freed.

Now, execute it.

~~~
$ cd misc; _build/example1
The address of instance1 is 0x55cc216aa600
The address of instance2 is 0x55cc216aa620
The address of the class of instance1 is 0x55cc216aa370
The address of the class of instance2 is 0x55cc216aa370
~~~

The locations of two instances `instance1` and `instance2` are different.
Each instance has its own memory.
The locations of two classes `class1` and `class2` are the same.
Two GObject instances share the same GObjectClass.

![Class and Instance](../image/class_instance.png)

## Reference count

GObject instance has its own memory.
They are allocated by the system when it is created.
If it becomes useless, the memory must be freed.
However, how can we determine whether it is useless?
GObject system provides reference count to solve the problem.

Instance is created and used by someone (some other objects, some functions or something like that).
We say someone refers to the instance.
If the instance is referred by A and B, then the number of the reference is two.
This is reference count.
Let's think about a scenario like this: 

- A calls `g_object_new` and owns an instance G.
A refers G, so the reference count of G is 1.
- B wants to use G too.
B calls `g_object_ref` and increases the reference count by 1.
Now the reference count is 2.
- A no longer uses G.
A calls `g_object_unref` and decreases the reference count by 1.
Now the reference count is 1.
- B no longer uses G.
B calls `g_object_unref` and decreases the reference count by 1.
Now the reference count is 0.
- Because the reference count is zero, G knows that no one refers to it.
G begins finalizing process by itself.
G disappears and the memory are freed.

A program `example2.c` is based on the scenario above.

~~~C
 1 #include <glib-object.h>
 2 
 3 static void
 4 show_ref_count (GObject *instance) {
 5   if (G_IS_OBJECT (instance))
 6     /* Users should not use ref_count member in their program. */
 7     /* This is only for demonstration. */
 8     g_print ("Reference count is %d.\n", instance->ref_count);
 9   else
10     g_print ("Instance is not GObject.\n");
11 }
12 
13 int
14 main (int argc, char **argv) {
15   GObject *instance;
16   
17   instance = g_object_new (G_TYPE_OBJECT, NULL);
18   g_print ("Call g_object_new.\n");
19   show_ref_count (instance);
20   g_object_ref (instance);
21   g_print ("Call g_object_ref.\n");
22   show_ref_count (instance);
23   g_object_unref (instance);
24   g_print ("Call g_object_unref.\n");
25   show_ref_count (instance);
26   g_object_unref (instance);
27   g_print ("Call g_object_unref.\n");
28   show_ref_count (instance);
29   
30   return 0;
31 }
32 
~~~

Now execute it.

~~~
$ cd misc; _build/example2
Call g_object_new.
Reference count is 1.
Call g_object_ref.
Reference count is 2.
Call g_object_unref.
Reference count is 1.
Call g_object_unref.
Instance is not GObject.
~~~

`example2` shows:

- `g_object_new` creates a new GObject instance and sets its reference count to 1.
- `g_object_ref` increases the reference count by 1.
- `g_object_unref` decreases the reference count by 1.
If the reference count drops to zero, the instance destroys itself.

## Initialization and destruction process

The actual process of GObject initialization and destruction process is very complex.
The following is simplified description without details.

Initialization

1. Registers GObject type with the type system.
This is done in the glib initialization process before the function `main` is called.
(If the compiler is gcc, then `__attribute__ ((constructor))` is used to qualify the initialization function.
Refer to [GCC manual](https://gcc.gnu.org/onlinedocs/gcc-10.2.0/gcc/Common-Function-Attributes.html#Common-Function-Attributes).)
2. Allocates memory for GObjectClass and GObject structure.
3. Initializes GObjectClass structure.
4. Initializes GObject structure.

This initialization process is carried out when `g_object_new` function is called for the first time.
At the second and subsequent call for `g_object_new`, it performs only two processes: (1) memory allocation for GObject structure (2) initialization for GObject structure.

Destruction

1. Destroys GObject structure. The memory for GObject is freed.

GObject type is a static type.
Static type never destroys its class.
So, even if the destroyed instance is the last instance, the class still remains.

When you write code to define a child object of GObject, It is important to understand the process above.
The detailed process will be explained in the later sections.


Up: [Readme.md](../Readme.md),  Prev: [Section 1](sec1.md), Next: [Section 3](sec3.md)
