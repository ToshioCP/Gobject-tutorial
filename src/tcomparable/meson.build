project ('tcomparable', 'c')

gobjdep = dependency('gobject-2.0')

sourcefiles=files('main.c', 'tnumber.c', 'tint.c', 'tdouble.c', 
                  '../tptr/tptr.c', 'tstr.c', 'tcomparable.c')

executable('tcomparable', sourcefiles, dependencies: gobjdep, install: false)

sourcefiles=files('main.c', 'tnumber.c', 'tint_without_macro.c', 'tdouble.c', 
                  '../tptr/tptr.c', 'tstr.c', 'tcomparable_without_macro.c')

executable('tcomparable_without_macro', sourcefiles, dependencies: gobjdep, install: false)

